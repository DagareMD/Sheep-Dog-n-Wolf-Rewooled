#ifndef SDW_ENGINE_STREAM_PLAYER_H
#define SDW_ENGINE_STREAM_PLAYER_H

/* The functions and globals stream_player.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

class StreamPlayer;

extern StreamPlayer *g_pStreamPlayer;

/* the folder under Voices/ the game plays language `language`'s voices from (Progress.language), with its slash
 * ("English/"): StreamPlayer::PlayVoice's switch */
const char *StreamPlayer_VoiceFolder(s32 language);

#endif
