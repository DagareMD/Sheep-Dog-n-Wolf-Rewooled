/*
 * Every other function here compiles the same either way.
 *
 * Two DirectSound StreamSounds are double-buffered: the playing one is `current` (= &streams[activeSlot]) and a loader
 * thread (StreamPlayer_LoaderThread) opens the next clip into the idle one, (activeSlot - 1) & 1, under the mutex
 * "SeekMutex". The clip names come from References\MusicVoice.BSV (Load_MusicVoiceBank): music is Musics\<name>, one track
 * per level (g_levelMusicClip); a voice line is Voices\<Language>/<name>.
 * The state machine (state, +0x33c):
 *   0 idle (the silent loop keeps the sound device busy)   1 music playing   2 music loading
 *   5 fading the music out for a voice line (1 s)          6 waiting for the voice to load (at least 2 s after the request)
 *   3 voice playing: when it ends the level's music is reloaded (-> 2)           4 is never set (IsBusy still tests it)
 * */
#define SDW_MEMBERS_WaveFile WaveFile();         /* WaveFile_Construct */
#define SDW_MEMBERS_StreamSound StreamSound();   /* StreamSound_Construct */
#define SDW_MEMBERS_StreamPlayer StreamPlayer(); /* StreamPlayer_Construct */
#include "sdw_enums.h"
#include "../sdk/win32.h"
#include "sdw_classes.h"

#include "../sdk/crt.h"

/* ---- globals ---- */
#include "progress.h"
#include "draw2d.h"
#include "time.h"
#include "sound_mgr.h"
#include "bs_io.h"
#include "../platform/platform.h"
#include "langpack.h"
/* (g_streamWaves itself is defined below, with its initialiser.) */
StreamPlayer *g_pStreamPlayer;
char *g_streamLoadPath;           /* the path the loader thread is opening */
u32 g_streamActiveSlot = 0;       /* mirror of StreamPlayer.activeSlot */
HANDLE g_hStreamLoaderThread = 0;
HANDLE g_hSeekMutex = 0;          /* "SeekMutex", held by the loader while it opens a clip */
s32 g_streamLoadStatus = 0;       /* 0 pending, 1 loaded, -1 WaveFile_Open failed */

/* This file's initialised data: the music clip of each level (1-based index into the clip bank, read as a word), then the
 * loader's music/voice switch. */
s32 g_levelMusicClip[18] = {
    VOICE_JUNGLE_V1,        VOICE_TRIP_V1,           VOICE_JAMI_FULL,    VOICE_MIX_JAMI_V2, VOICE_JAZZ90_V3,
    VOICE_TRIP_V1,          VOICE_LUNEY_V2,          VOICE_LUNEY_V2,     VOICE_ZILLA_V2,    VOICE_JUNGLERHODES01_V2,
    VOICE_ROAD_MARECAGE_V2, VOICE_ROAD_MARECAGE_V2,  VOICE_ZILLA_V2,     VOICE_JAZZ90_V3,   VOICE_DNBASS_V2,
    VOICE_NEWS1LAST_V2,     VOICE_JUNGLETUTORIAL_V2, VOICE_FRONZYLAST_V3};
u8 g_streamLoadIsMusic = 1; /* 1: the clip being loaded is music (looping, volume +0x95), 0: a voice */

DWORD __stdcall StreamPlayer_LoaderThread(void *path);

/* Close the loader thread's handle (the thread itself is left to run to its end). */
static inline void CloseLoader()
{
    CloseHandle(g_hStreamLoaderThread);
    g_hStreamLoaderThread = 0;
}

/* If the loader thread has let go of the mutex, take its result (1 loaded, -1 failed) and close its handle; 0 while it is
 * still busy (or has not started yet: the status is reset to 0 before each thread starts). */
static inline s32 LoaderResult()
{
    s32 status = STREAM_LOAD_PENDING;
    if (WaitForSingleObject(g_hSeekMutex, 0) == WAIT_OBJECT_0) {
        status = g_streamLoadStatus;
        ReleaseMutex(g_hSeekMutex);
        CloseLoader();
    }
    return status;
}

WaveFile g_streamWaves[2]; /* one WaveFile per stream slot */

/* the loader thread: open the clip into the idle slot's WaveFile, (re)create that slot's stream on it and set
 * its volume and looping from g_streamLoadIsMusic; the result goes to g_streamLoadStatus, all under the mutex. */
DWORD __stdcall StreamPlayer_LoaderThread(void *path)
{
    g_streamLoadPath = (char *)path;
    u32 slot = (g_streamActiveSlot - 1) & 1;
    while (WaitForSingleObject(g_hSeekMutex, INFINITE) != WAIT_OBJECT_0)
        ;
    g_streamWaves[slot].Close();
    if (g_streamWaves[slot].Open((char *)path, 0, 0) >= 0) {
        StreamSound *s = &g_pStreamPlayer->streams[(g_pStreamPlayer->activeSlot - 1) & 1];
        s->Free();
        s->CreateFromWaveEx(g_pSoundSystem, &g_streamWaves[slot], 2.0f, 16);
        if (g_streamLoadIsMusic == 1) {
            s->Sound_SetBufferVolume(g_pProgress->streamVolumeA * (1.0f / 255.0f));
            s->looping = 1;
        } else {
            s->Sound_SetBufferVolume(g_pProgress->streamVolumeB * (1.0f / 255.0f));
            s->looping = 0;
        }
        g_streamLoadStatus = STREAM_LOAD_OK;
    } else {
        g_streamLoadStatus = STREAM_LOAD_FAILED;
    }
    ReleaseMutex(g_hSeekMutex);
    return 1;
}

StreamPlayer::StreamPlayer()
{
    bankLoaded = 0;
    currentClip = 0;
    clipCount = 0;
    clipNames = 0;
    pausedState = STREAM_ST_IDLE;
    state = STREAM_ST_IDLE;
    stateStartMs = 0;
    activeSlot = 0;
    current = &streams[activeSlot];
    autoPlay = 1;
}

/* (StreamPlayer_ScalarDeletingDtor, compiler-generated) - the destructor is inline, so its body appears only
 * there: free the clip bank, then the two streams are destroyed. */
inline StreamPlayer::~StreamPlayer()
{
    if (clipNames) {
        for (u16 i = 0; i < clipCount; i++)
            delete clipNames[i];
        delete clipNames;
        clipNames = 0;
    }
}

/* read References\MusicVoice.BSV: a 12-byte magic "BSV_FILEV2.5", then 116 {u32 size; char name[size]}
 * entries, each copied to its own allocation (NULL for size 0). */
void StreamPlayer::Load_MusicVoiceBank()
{
    char path[SDW_PATH_MAX];
    u32 size = 0;
    char *magic;
    strcpy(path, g_dirReference);
    strcat(path, "MusicVoice.BSV");
    char *file = (char *)Bs_LoadFile(path, &size);
    clipCount = 0;
    if ((s32)size > 0) {
        magic = new char[12];
        strncpy(magic, file, 12);
        if (strncmp(magic, "BSV_FILEV2.5", 12) != 0) {
            MessageBoxA(0, "Could not load file, check version", "BSV File Error", MB_ICONHAND);
        } else {
            u32 pos = 12;
            clipCount = 0x74;
            clipNames = new char *[0x74];
            for (u16 i = 0; i < clipCount; i++) {
                u32 len = *(u32 *)(file + pos);
                pos += 4;
                if (len != 0) {
                    clipNames[i] = new char[len];
                    memcpy(clipNames[i], file + pos, len);
                    pos += len;
                } else {
                    clipNames[i] = 0;
                }
            }
            activeSlot = 0;
            g_streamActiveSlot = 0;
            current = &streams[activeSlot];
            bankLoaded = 1;
        }
    }
    if (magic)
        delete magic;
    if (file)
        delete file;
    state = STREAM_ST_IDLE;
}

/* stop and release the current stream (Video_PlaySequence, before it destroys the sound device). */
void StreamPlayer::StopAndFree()
{
    current->Stop();
    current->Free();
    state = STREAM_ST_IDLE;
}

/* request a voice line: build .\Voices\<Language>/<clip name> and start the loader on it; the music fades out
 * meanwhile (state 5). The language argument is not used - the switch reads the progress object again. */
const char *StreamPlayer_VoiceFolder(s32 language)
{
    switch (language) {
        case GAME_LANG_FRENCH:
            return "French/";
        case GAME_LANG_SPANISH:
            return "Spanish/";
        case GAME_LANG_ITALIAN:
            return "Italian/";
        case GAME_LANG_GERMAN:
            return "German/";
        case GAME_LANG_DUTCH:
            return "Dutch/";
        case GAME_LANG_BRAZILIAN:
            return "Brazilian/";
        case GAME_LANG_ENGLISH:
        default:
            return "English/";
    }
}

void StreamPlayer::PlayVoice(u32 clip, u8 language, u32 unused, u32 lipSync)
{
    /* clip is 1-based into the voice bank; an object's record can hold anything there (the PlayStation's .WAR keeps
     * disc positions in the voice properties), so a clip the bank does not have is not played */
    if (!bankLoaded || clip == 0 || clip > clipCount || !clipNames[clip - 1]) {
        Platform_Log("StreamPlayer: no voice clip %u (the bank has %u)", clip, (unsigned)clipCount);
        return;
    }
    currentClip = clip;
    if (g_pSoundSystem->initialized != 1)
        return;
    if (g_pProgress->optionFlags & OPT_SOUNDMODE_MASK)
        return;
    /* Rewooled: the clip of the voice language (langpack.h), in place of Voices/<the config language>/
     * (StreamPlayer_VoiceFolder) */
    LangPack_VoicePath(clipNames[currentClip - 1], path, sizeof(path));
    g_pSoundSystem->UnregisterStreamEvent(current);
    g_streamLoadIsMusic = 0;
    g_streamLoadStatus = STREAM_LOAD_PENDING;
    g_hSeekMutex = CreateMutexA(0, 0, "SeekMutex");
    DWORD threadId;
    g_hStreamLoaderThread = CreateThread(0, 0, StreamPlayer_LoaderThread, path, 0, &threadId);
    SetPriorityClass(g_hStreamLoaderThread, NORMAL_PRIORITY_CLASS);
    SetThreadPriority(g_hStreamLoaderThread, THREAD_PRIORITY_ABOVE_NORMAL);
    stateStartMs = g_rawTimeMs;
    state = STREAM_ST_VOICE_FADE_MUSIC;
    playArg = lipSync;
}

/* the voice half of the per-frame update: fade the music out (5), wait for the voice and swap it in (6),
 * and when the voice has finished reload the level's music (3). Returns 1 while it is fading or has just swapped. */
s32 StreamPlayer::UpdateVoice()
{
    s32 result = 0;
    switch (state) {
        case STREAM_ST_VOICE_PLAYING:
            if ((u32)(g_rawTimeMs - stateStartMs) > 2000) {
                if (LoaderResult() == STREAM_LOAD_PENDING) {
                    Time_Pause();
                    while (LoaderResult() == STREAM_LOAD_PENDING)
                        ;
                    Time_Resume();
                }
                if (LoaderResult() == STREAM_LOAD_OK) {
                    CloseLoader();
                    activeSlot = (activeSlot - 1) & 1;
                    current = &streams[activeSlot];
                    g_streamActiveSlot = activeSlot;
                    Sound_StopSilentLoop();
                    if (autoPlay == 1)
                        current->Play(playArg);
                    result = 1;
                    state = STREAM_ST_VOICE_DONE;
                } else if (LoaderResult() == STREAM_LOAD_FAILED) {
                    current->Stop();
                    state = STREAM_ST_VOICE_DONE;
                }
            }
            break;
        case STREAM_ST_VOICE_FADE_MUSIC: {
            u32 elapsed = g_rawTimeMs - stateStartMs;
            if (elapsed < 1000) {
                u8 vol = g_pProgress->streamVolumeA;
                u8 faded = vol - vol * elapsed / 1000;
                current->Sound_SetBufferVolume(faded * (1.0f / 255.0f));
                result = 1;
            } else {
                current->Stop();
                current->Free();
                result = 1;
                state = STREAM_ST_VOICE_PLAYING;
            }
            break;
        }
        case STREAM_ST_VOICE_DONE:
            if (!current->IsPlaying()) {
                current->Free();
                s8 level = g_pProgress->currentLevel;
                if (g_pSoundSystem->initialized == 1 && !(g_pProgress->optionFlags & OPT_SOUNDMODE_MASK)) {
                    if (level < SCENE_LVL_00 || level > SCENE_LVL_17)
                        level = SCENE_LVL_00;
                    current->Stop();
                    currentClip = g_levelMusicClip[level] - 1;
                    strcpy(path, g_musicsPath);
                    strcat(path, clipNames[currentClip]);
                    stateStartMs = g_rawTimeMs;
                    state = STREAM_ST_LOADING;
                    g_streamLoadIsMusic = 1;
                    g_streamLoadStatus = STREAM_LOAD_PENDING;
                    g_hSeekMutex = CreateMutexA(0, 0, "SeekMutex");
                    DWORD threadId;
                    g_hStreamLoaderThread = CreateThread(0, 0, StreamPlayer_LoaderThread, path, 0, &threadId);
                    SetPriorityClass(g_hStreamLoaderThread, NORMAL_PRIORITY_CLASS);
                    SetThreadPriority(g_hStreamLoaderThread, THREAD_PRIORITY_ABOVE_NORMAL);
                    current->looping = 1;
                }
            }
            break;
    }
    return result;
}

/* cut a voice line short (voice states 3, 5, 6 only): drop the loader's thread handle if it has not finished,
 * stop the current stream and go to 3, from where the next update reloads the music. The loader thread itself is not
 * stopped. */
void StreamPlayer::StopVoice()
{
    if (state == STREAM_ST_VOICE_PLAYING || state == STREAM_ST_VOICE_FADE_MUSIC || state == STREAM_ST_VOICE_DONE) {
        if (LoaderResult() == STREAM_LOAD_PENDING)
            CloseLoader();
        current->Stop();
        state = STREAM_ST_VOICE_DONE;
    }
}

/* (re)start the level's music: stop the current stream and load the level's track in the background. */
void StreamPlayer::RestartLevelMusic()
{
    s8 level = g_pProgress->currentLevel;
    if (g_pSoundSystem->initialized == 1 && !(g_pProgress->optionFlags & OPT_SOUNDMODE_MASK)) {
        if (level < SCENE_LVL_00 || level > SCENE_LVL_17)
            level = SCENE_LVL_00;
        current->Stop();
        currentClip = g_levelMusicClip[level] - 1;
        strcpy(path, g_musicsPath);
        strcat(path, clipNames[currentClip]);
        stateStartMs = g_rawTimeMs;
        state = STREAM_ST_LOADING;
        g_streamLoadIsMusic = 1;
        g_streamLoadStatus = STREAM_LOAD_PENDING;
        g_hSeekMutex = CreateMutexA(0, 0, "SeekMutex");
        DWORD threadId;
        g_hStreamLoaderThread = CreateThread(0, 0, StreamPlayer_LoaderThread, path, 0, &threadId);
        SetPriorityClass(g_hStreamLoaderThread, NORMAL_PRIORITY_CLASS);
        SetThreadPriority(g_hStreamLoaderThread, THREAD_PRIORITY_ABOVE_NORMAL);
        current->looping = 1;
    }
}

/* the same at level load (Load_DAVnWAR), but it starts the silent loop, releases the current stream instead
 * of stopping it, and waits for the loader to finish. */
void StreamPlayer::LoadLevelMusic()
{
    s8 level = g_pProgress->currentLevel;
    if (g_pSoundSystem->initialized == 1 && !(g_pProgress->optionFlags & OPT_SOUNDMODE_MASK)) {
        Sound_StartSilentLoop();
        if (level < SCENE_LVL_00 || level > SCENE_LVL_17)
            level = SCENE_LVL_00;
        current->Free();
        currentClip = g_levelMusicClip[level] - 1;
        strcpy(path, g_musicsPath);
        strcat(path, clipNames[currentClip]);
        stateStartMs = g_rawTimeMs;
        state = STREAM_ST_LOADING;
        g_streamLoadIsMusic = 1;
        g_streamLoadStatus = STREAM_LOAD_PENDING;
        g_hSeekMutex = CreateMutexA(0, 0, "SeekMutex");
        DWORD threadId;
        g_hStreamLoaderThread = CreateThread(0, 0, StreamPlayer_LoaderThread, path, 0, &threadId);
        SetPriorityClass(g_hStreamLoaderThread, NORMAL_PRIORITY_CLASS);
        SetThreadPriority(g_hStreamLoaderThread, THREAD_PRIORITY_ABOVE_NORMAL);
        current->looping = 1;
        while (LoaderResult() == STREAM_LOAD_PENDING)
            ;
    }
}

/* pause (1: remember the state, go idle, stop the stream) or resume (restore it and replay). */
void StreamPlayer::SetPaused(s32 pause)
{
    if (pause == 1) {
        pausedState = state;
        state = STREAM_ST_IDLE;
        autoPlay = 0;
        current->Stop();
    } else {
        state = pausedState;
        autoPlay = 1;
        if (state != STREAM_ST_IDLE)
            current->Play(0);
    }
}

/* stop the current stream and go idle. */
void StreamPlayer::Halt()
{
    current->Stop();
    state = STREAM_ST_IDLE;
}

/* the per-frame update (Game_Frame): the voice states first, then idle keeps the silent loop going and a
 * finished music load is swapped in and played (-> 1). */
void StreamPlayer::Update()
{
    UpdateVoice();
    switch (state) {
        case STREAM_ST_LOADING:
            if (LoaderResult() == STREAM_LOAD_OK) {
                CloseLoader();
                activeSlot = (activeSlot - 1) & 1;
                current = &streams[activeSlot];
                g_streamActiveSlot = activeSlot;
                Sound_StopSilentLoop();
                if (autoPlay == 1)
                    current->Play(0);
                state = STREAM_ST_PLAYING;
            }
            break;
        case STREAM_ST_IDLE:
            Sound_StartSilentLoop();
            break;
    }
}

/* re-apply the volume setting to the current stream: music volume in state 1, voice volume in 3 and 6. */
void StreamPlayer::ApplyVolume()
{
    switch (state) {
        case STREAM_ST_VOICE_DONE:
        case STREAM_ST_VOICE_PLAYING:
            current->Sound_SetBufferVolume(g_pProgress->streamVolumeB * (1.0f / 255.0f));
            break;
        case STREAM_ST_PLAYING:
            current->Sound_SetBufferVolume(g_pProgress->streamVolumeA * (1.0f / 255.0f));
            break;
    }
}

/* 1 in the music states (1, 2). */
#pragma warning(disable : 4715)
s32 StreamPlayer::IsMusic()
{
    if (state == STREAM_ST_LOADING || state == STREAM_ST_PLAYING)
        return 1;
}

/* 1 while a voice line owns the player (states 3..6): Game_Frame keeps the menus shut then. */
s32 StreamPlayer::IsBusy()
{
    if (state == STREAM_ST_VOICE_DONE || state == STREAM_ST_UNUSED_4 || state == STREAM_ST_VOICE_FADE_MUSIC ||
        state == STREAM_ST_VOICE_PLAYING)
        return 1;
    return 0;
}
