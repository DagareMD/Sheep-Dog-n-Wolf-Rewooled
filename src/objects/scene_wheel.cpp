

#define SDW_MEMBERS_ScnObject            \
    static void *operator new(uptr size); \
    void SetUpdateMode(u8 mode);
#define SDW_MEMBERS_ScnBody                                                         \
    AnimJointPose *GetJointPose(s32 index)                                          \
    {                                                                               \
\
        return ((AnimJointPose *)anim.bufC) + index;                                \
    }

#define SDW_MEMBERS_Progress   \
    void SetLanguage(u8 value) \
    {                          \
        language = value;      \
    }
#include "sdw_types.h"
#include "sdw_enums.h"
#include "scenaric_props.h"
class Instance;
struct Animator;
u32 Anim_Start(Instance *inst, Animator *animator, u16 id, u32 opts);
#include "sdw_classes.h"
#include "../sdk/crt.h"
#define SDW_INLINE_INSTANCE_INST 1
#include "instance_inlines.h"
#undef SDW_INLINE_INSTANCE_INST
#define SDW_INLINE_SCNOBJECT_GETCLASSID 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_GETCLASSID
#define SDW_INLINE_SCNBODY_ANIMFLAGS_U16 1
#define SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32 1
#include "../engine/scn_body_inlines.h"
#undef SDW_INLINE_SCNBODY_ANIMFLAGS_U16
#undef SDW_INLINE_SCNBODY_PLAYANIM_U16_S32_S32

extern Wolf *g_pWolf;
#include "../engine/progress.h"
#include "../engine/scenaric.h"
#include "../engine/screen.h"
#include "../engine/input.h"
#include "../engine/fade.h"
#include "../engine/text.h"
#include "../engine/langpack.h"
#include "scene_wheel_disc.h"
extern s32 g_dtRawMs;
extern s32 g_dtMs;
extern u32 g_gameFlags;
float g_sceneWheelAngle; /* the wheel's angle, kept across the level's reloads */
/* The map gives them to this object (they could also be SignPost's, or any object's from Seaweed to SheepCostume); what
 * they held is unknown, so they are opaque here. */
u8 g_sceneWheelUnreferenced[8] = {0};

/* calls with six stack arguments. */

/* Original data at VA and VA . Byte escapes retain the
 * original single-byte Portuguese/Spanish characters and Italian case. Rewooled: the Brazilian Portuguese is named
 * Português Brasileiro (the original: Português), so that Português is the Portuguese of Portugal. */
const char *g_sceneWheelLanguageNames[] = {"english",
                                           "Portugu\xEA"
                                           "s Brasileiro",
                                           "espa\xF1"
                                           "ol",
                                           "italIAno"};
u8 g_sceneWheelLanguageIds[] = {GAME_LANG_ENGLISH, GAME_LANG_BRAZILIAN, GAME_LANG_SPANISH, GAME_LANG_ITALIAN};
/* Rewooled: the table's length, for the language list (langpack.cpp: the game's own languages) */
extern const u32 g_sceneWheelLanguageCount = sizeof(g_sceneWheelLanguageIds) / sizeof(g_sceneWheelLanguageIds[0]);

#define SDW_INLINE_FREE_GAME_CLEARFLAGS_U32 1
#include "../engine/game_state_inlines.h"
#undef SDW_INLINE_FREE_GAME_CLEARFLAGS_U32
#define SDW_INLINE_FREE_SCREENHEIGHTS16 1
#include "../engine/screen_inlines.h"
#undef SDW_INLINE_FREE_SCREENHEIGHTS16
/* reads g_screen+0x84 (VA ), the generated scratch60 allocation that contains this screen's 24 four-byte layer handles. */
inline u32 *SceneWheel_GetLayer(u16 index)
{
    return (u32 *)g_screen.scratch60 + index;
}

#define SDW_INLINE_SCNOBJECT_SETUPDATEMODE_U8 1
#include "../engine/scenaric_inlines.h"
#undef SDW_INLINE_SCNOBJECT_SETUPDATEMODE_U8

/* The divisor is the original decimal constant, whose double
 * representation is 0x401921fb54442d11. The caller clamps a result of four. */
s8 Scene_Wheel_AngleToIndex(float angle)
{
    double normalized = (float)fmod(angle, 6.28318530717958);
    if (!_isnan(normalized))
        return (s8)((float)normalized * 4.0f / 6.2831855f + 0.5f);
    return 0;
}

/* Rewooled: a stop per language (langpack.h), not the original's four, the languages Settings lists: the model's disc
 * has a slice per language (scene_wheel_disc.cpp), language k's at k stops, and the stop's angle is
 * index * SceneWheel_Step(), as index * pi/2 was. */
static s32 SceneWheel_Stops()
{
    s32 n = LangPack_Count();
    return n > 0 ? (n < 64 ? n : 64) : 1;
}

static float SceneWheel_Step()
{
    return 6.2831855f / SceneWheel_Stops();
}

/* Rewooled: the language's name, under Daffy where the original's is (a window from x 64 to 320, its line at the
 * screen's height - 40), the window widened about its middle to keep a longer name on one line, as far as the screen's
 * left edge allows; a name longer still wraps, its window raised so that every line is on the screen */
static void SceneWheel_PrintName(const char *name)
{
    s32 middle = 64 + 256 / 2, glyph = g_pCurFont->glyphWidth, line = g_pCurFont->lineHeight;
    s32 w = (s32)strlen(name) * glyph, y = ScreenHeightS16() - 40, lines;
    if (w < 256)
        w = 256;
    if (w > 2 * middle)
        w = 2 * middle;
    Text_SetWindow(SceneWheel_GetLayer(6), middle - w / 2, y, w, 24, 1);
    lines = Text_CountWrappedLines(name);
    if (lines > 1)
        Text_SetWindow(SceneWheel_GetLayer(6), middle - w / 2, y - (lines - 1) * line, w, 24 + (lines - 1) * line, 1);
    Text_PrintfStyled(TEXTALIGN_CENTER, 1, "%s", name);
}

/* the stop the wheel's angle is nearest */
static s8 SceneWheel_AngleToStop(float angle)
{
    s32 n = SceneWheel_Stops();
    double normalized = (float)fmod(angle, 6.28318530717958);
    if (_isnan(normalized))
        return 0;
    if (normalized < 0)
        normalized += 6.28318530717958;
    return (s8)((s32)((float)normalized * n / 6.2831855f + 0.5f) % n);
}

void Scene_Wheel::PostLoadInit()
{
    Reset();
    SceneWheel_BuildDisc(this); /* Rewooled: the model's disc, a slice per language */
}

void Scene_Wheel::Update()
{
    s8 selected;
    AnimJointPose *pose = 0;
    idleMs += (s16)g_dtRawMs;
    Game_ClearFlags(GF_TRANSITION_IDLE);
    if (state == SCENEWHEEL_ST_SELECT || state == SCENEWHEEL_ST_CONFIRM || state == SCENEWHEEL_ST_UNUSED_2) {
        pose = GetJointPose(1);
        if (pose && pose - 1)
            pose->rot[0] = g_sceneWheelAngle;
    }
    switch (state) {
        case SCENEWHEEL_ST_START:
            if (g_pWolf && !wolfFrozen)
                wolfFrozen = g_pWolf->HandleMessage(this, MSG_FREEZE, 0);
            state = SCENEWHEEL_ST_PUSH;
            PlayAnim(ALANGUE1_ANIM_PUSH, 0, 1);
            break;
        case SCENEWHEEL_ST_PUSH:
            if (AnimFlags(ANIM_F_FINISHED))
                state = SCENEWHEEL_ST_SETUP;
            break;
        case SCENEWHEEL_ST_SETUP:
            idleMs = 0;
            snapping = 0;
            state = SCENEWHEEL_ST_SELECT;
            targetAngle = index * SceneWheel_Step();
            pose = GetJointPose(1);
            if (pose && pose - 1)
                pose->rot[0] = targetAngle;
            break;
        case SCENEWHEEL_ST_SELECT:
            selected = index;
            pose = GetJointPose(1);
            if (Pad_MenuRepeat((u16)~PAD_RIGHT)) {
                index++;
                if (index >= SceneWheel_Stops())
                    index = 0;
                snapping = 1;
                targetAngle += SceneWheel_Step();
                Scenaric_SendToClass(CLASSID_DAFFYWHEEL, MSG_DAFFYWHEEL_TURN, 0);
                idleMs = 0;
            }
            if (Pad_MenuRepeat((u16)~PAD_LEFT)) {
                index--;
                if (index < 0)
                    index = SceneWheel_Stops() - 1;
                snapping = 1;
                targetAngle -= SceneWheel_Step();
                Scenaric_SendToClass(CLASSID_DAFFYWHEEL, MSG_DAFFYWHEEL_TURN, 0);
                idleMs = 0;
            }
            if (pose && pose - 1) {
                if (pose->rot[0] > 6.2831855f && targetAngle > 6.2831855f) {
                    pose->rot[0] = (double)(float)fmod(pose->rot[0], 6.28318530717958);
                    targetAngle = index * SceneWheel_Step();
                }
                if (pose->rot[0] < 0.0f && targetAngle < 0.0f) {
                    pose->rot[0] += 6.2831855f;
                    targetAngle = index * SceneWheel_Step();
                }
                angVel = (targetAngle - pose->rot[0]) * 2.0f;
                if ((angVel >= 0.0f ? (float)(double)angVel : -angVel) > 5.497786998748779f)
                    angVel = (angVel >= 0.0f ? 1 : -1) * 5.497786998748779f;
                if (snapping) {
                    if ((angVel > 0.0f && pose->rot[0] >= targetAngle) ||
                        (angVel < 0.0f && pose->rot[0] <= targetAngle)) {
                        snapping = 0;
                        angVel = 0;
                        pose->rot[0] = index * SceneWheel_Step();
                    }
                    pose->rot[0] = ((float)g_dtMs * angVel) / 1000.0f + pose->rot[0];
                }
                selected = SceneWheel_AngleToStop(pose->rot[0]);
            }
            if (selected >= SceneWheel_Stops() || selected < 0)
                selected = 0;
            if (Pad_MenuPressed((u16)~PAD_CROSS)) {
                /* Rewooled: the language chosen is the text language from the next level on (langpack.h); the
                 * game code that reads a config language is given a baked-in language's own (a pack has none: it is
                 * left as it is). DaffyWheel does not read its message's argument. */
                state = SCENEWHEEL_ST_CONFIRM;
                s32 number = LangPack_GameLanguageOf(selected);
                Scenaric_SendToClass(CLASSID_DAFFYWHEEL, MSG_DAFFYWHEEL_SELECT, (void *)(uptr)(number >= 0 ? number : 0));
                LangPack_ChooseText(selected);
                if (number >= 0)
                    g_pProgress->SetLanguage((u8)number);
            }
            Text_SetFont(FONT_GAME);
            SceneWheel_PrintName(LangPack_Name(selected));
            Hud_EndBox_stub();
            break;
        case SCENEWHEEL_ST_CONFIRM:
            SetUpdateMode(SCN_UPD_NEVER);
            if (g_pWolf && wolfFrozen) {
                g_pWolf->HandleMessage(this, MSG_UNFREEZE, 0);
                wolfFrozen = 0;
            }
            if (exitLevelOnConfirm) {
                g_levelExitFlags |= LEVEL_EXIT_DEMO;
                Fade_StartLevelExit(0x1000);
            } else
                Scenaric_SendToClass(CLASSID_MCARDMANAGER, MSG_USE, 0);
            state = SCENEWHEEL_ST_UNUSED_2;
            break;
    }
    if (state == SCENEWHEEL_ST_SELECT || state == SCENEWHEEL_ST_CONFIRM || state == SCENEWHEEL_ST_UNUSED_2) {
        pose = GetJointPose(1);
        if (pose && pose - 1) {
            g_sceneWheelAngle = pose->rot[0];
            pose->rot[0] += 4.712389f;
            AdvanceAnim();
        }
    } else
        AdvanceAnim();
}

sptr Scene_Wheel::HandleMessage(ScnObject *sender, u32 msgId, void *arg)
{
    if (msgId == MSG_SCENEWHEEL_SAVE_DONE && sender->GetClassId() == CLASSID_MCARDMANAGER) {
        SetUpdateMode(SCN_UPD_NORMAL);
        state = SCENEWHEEL_ST_SETUP;
        return 1;
    }
    return 0;
}

void Scene_Wheel::Reset()
{
    flags &= ~SCN_OF_HIDDEN;
    SetUpdateMode(SCN_UPD_NORMAL);
    index = 0;
    angVel = 0;
    snapping = 0;
    wolfFrozen = 0;
    unk6a = 0;
    idleMs = 0;
    state = SCENEWHEEL_ST_START;
}

ScnObject *Scene_Wheel_Create(void *record)
{
    ScnBody *obj = new Scene_Wheel;
    obj = obj->Init(record, 0);
    return obj;
}
