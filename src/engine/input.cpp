/*
 *  - g_padMenuCur / g_padMenuPrev / g_padMenuRepeat are fields of g_pad, not objects:
 *    written as macros for the fields, they compile to the same addresses. Other objects read g_padPrevButtons,
 *    g_padCurButtons (g_pad.prev/cur.buttons) and g_padMasks (&g_inputMap[4]) the same way; nothing
 *    can define those names as symbols, so their users need the same field / element spelling to link.
 *  - Pad is 0x64 bytes in the generated headers, but g_pad and g_pad2 are 0x70 apart and g_padRawSnapshot follows
 *    g_pad2 at +0x6c: sizeof(Pad) is 0x6c in the original (an object of 64 bytes or more is 8-aligned, so 0x6c gives
 *    exactly these gaps).
 *  - Clock globals as the Time object defines them: g_gameTimeMs s32 (only copied here); g_dtRawMs s32, read here
 *    through a macro as its low half-word, which is the s16 load the original makes.
 */
/*
 * The pad frames follow the PlayStation's libpad receive buffer: byte 0 status (0 = ok), byte 1 = type << 4 | length,
 * then the active-low button word and four stick bytes. The raw buffer is 34 bytes on the PS1, which is why the
 * previous frame starts at +0x22. The type nibble is read and written as a C bitfield (shr 4 / and 0xf):
 * PadFrame.typeLen is the bitfield struct PadTypeLen {len:4, type:4}.
 */
#define SDW_MEMBERS_Pad                            \
    inline s32 IsAnalog();
#define SDW_MEMBERS_Progress           \
    void SetPadIsAnalog(u8 analog)     \
    {                                  \
        controls.padIsAnalog = analog; \
    }
#include "sdw_enums.h"
#include "sdw_classes.h"
#define SDW_INLINE_PAD_SETACTUATOR_INT 1
#include "input_inlines.h"
#undef SDW_INLINE_PAD_SETACTUATOR_INT

inline s32 Pad::IsAnalog()
{
    return cur.typeLen.type == PADTYPE_ANALOG;
}

#include "../sdk/crt.h"
#include "../platform/platform.h"

/* ---- globals ---- */
#include "draw2d.h"
#include "progress.h"
#include "maths.h"
#include "file.h"
extern s32 g_gameTimeMs;
extern s32 g_dtRawMs;
#define g_dtRawMs (*(s16 *)&g_dtRawMs)

static u8 g_cineOpStride_57eb64[9] = {0, 8, 8, 4, 2, 2, 4, 2, 2};
/* four initialised globals no code refers to (types and names unknown). The zero half-word at is alignment padding:
 * g_inputMap, an array of 3 bytes or more, is 4-aligned. */
u16 g_inputUnref_57eb6e = 0x1000;
u16 g_inputUnref_57eb70 = 0x0100;
s16 g_inputUnref_57eb72 = -1;
u16 g_inputUnref_57eb74 = 0xffff;
/* button mapping. Entries 4.. are the active-low pad masks that other objects read as g_padMasks ( = &g_inputMap[4]; not
 * a separate object, so no symbol of that name is defined here). */
u16 g_inputMap[16] = {(u16)~PAD_SELECT,   (u16)~PAD_L3,     (u16)~PAD_R3,    (u16)~PAD_START,
                      (u16)~PAD_UP,       (u16)~PAD_RIGHT,  (u16)~PAD_DOWN,  (u16)~PAD_LEFT,
                      (u16)~PAD_L2,       (u16)~PAD_R2,     (u16)~PAD_L1,    (u16)~PAD_R1,
                      (u16)~PAD_TRIANGLE, (u16)~PAD_CIRCLE, (u16)~PAD_CROSS, (u16)~PAD_SQUARE};
u8 g_inputMapAux[16] = {0,   0,   0,   0,   0,  0,  0,  0,
                        198, 203, 197, 235, 93, 94, 95, 92}; /* swapped in lockstep with g_inputMap */
u8 g_rumbleSeqKill[5] = {0xc8, 0x40, 0x80, 0x20, 0xff};
u8 g_rumbleSeqImpact[6] = {0x20, 0x80, 0x40, 0x60, 0x20, 0xff};
u8 g_rumbleSeqFallingRock[4] = {0x20, 0xc8, 0x20, 0xff};
/* ( : the literals of PadRec_Load / PadRec_Save) */

u8 g_inputMode = 0;                   /* 0 live, 1 record, 2 playback */
s16 g_padRepeatDelay = 0;             /* auto-repeat threshold, ms */
Pad g_pad = {0};
u32 g_padTail_71968c[2] = {0};        /* unreferenced: the last 8 bytes of a 0x6c-byte Pad (see the header) */
Pad g_pad2 = {0};
u32 g_padTail_7196fc[2] = {0};        /* likewise for g_pad2 */
PadFrame g_padRawSnapshot = {0};
u32 g_inputUnref_71970c[7] = {0};     /* 28 bytes no code refers to (unknown) */
/* The header and the data after it are one object, since a modern compiler need not place two globals
 * next to each other (PadRec_Load / PadRec_Save treat &g_padRec as the start of both). */
struct PadRecStore {
    PadRecHeader header;
    u8 data[0x1800];
};
PadRecStore g_padRecStore = {0};
#define g_padRec (g_padRecStore.header)

/* Fields of g_pad that other code reads under their own names ( .. are inside g_pad, not objects of their own) */
#define g_padMenuCur (g_pad.menuCur)
#define g_padMenuPrev (g_pad.menuPrev)
#define g_padMenuRepeat (g_pad.menuRepeat.output)

/* ---- functions ---- */
void Pad_DetectType(Pad *pad);
void Pad_ReadRaw(Pad *pad);
void Pad_InitPair(Pad *p1, Pad *p2);
void PadFrame_Clear(PadFrame *frame);
void Pad_ClearFrames(Pad *pad);
void Input_SetMode(u8 mode);
u16 Input_GetMapping(u8 slot);
void Input_SwapMapping(u8 slot, u16 value);
void Input_ApplyRemap(u16 slotC, u16 slotD, u16 slotE, u16 slotF, u16 slotB, u16 slotA);

template <class T> inline void Swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}

/* device type from the input manager: 4 for a digital device (keyboard), else 7 (analog) and the state
 * is cleared. */
void Pad_DetectType(Pad *pad)
{
    if (g_inputMgr.IsDeviceDigital(0) == 1) {
        pad->padType = PADTYPE_DIGITAL;
        pad->padTypeReq = PADTYPE_DIGITAL;
    } else {
        pad->state59 = 0;
        pad->state5a = 0;
        pad->state44 = 0;
        pad->state48 = 0;
        pad->state4c = 0;
        pad->state60 = 0;
        pad->actuatorEnable = 0;
        pad->state54 = 0;
        pad->padType = PADTYPE_ANALOG;
        pad->padTypeReq = PADTYPE_ANALOG;
    }
}

/* polls the input manager into pad->raw. On failure only the status is set (the buttons stay stale). The
 * type always comes from g_pad, whatever pad is being filled. */
void Pad_ReadRaw(Pad *pad)
{
    pad->raw.rightY = 0x80;
    pad->raw.rightX = 0x80;
    if (g_inputMgr.Poll() < 0) {
        pad->raw.status = 1;
    } else {
        Pad_DetectType(&g_pad);
        pad->raw.status = 0;
        pad->raw.typeLen.type = g_pad.padType;
        pad->raw.leftX = g_inputMgr.axisX;
        pad->raw.leftY = g_inputMgr.axisY;
        pad->raw.rightX = g_inputMgr.rightX; /* the original PC build left the right stick centred (no DirectInput axis
        pad->raw.rightY = g_inputMgr.rightY;  * fed it): the camera code reads it as on the PlayStation */
        pad->raw.buttons = g_inputMgr.padBits;
    }
}

void Pad_InitPair(Pad *p1, Pad *p2)
{
    p1->actuatorEnable = 0;
    p2->actuatorEnable = 0;
    g_padRepeatDelay = 500;
    p1->ResetState();
    p2->ResetState();
}

/* idle frame: no button down (active low), both sticks centred. */
void PadFrame_Clear(PadFrame *frame)
{
    frame->buttons = 0xffff;
    *(u16 *)&frame->leftX = 0x8080;
    *(u16 *)&frame->rightX = 0x8080;
}

void Pad_ClearFrames(Pad *pad)
{
    PadFrame_Clear(&pad->cur);
    PadFrame_Clear(&pad->prev);
    PadFrame_Clear(&pad->raw);
}

/* input bring-up (from Load_DAVnWAR): opens both pads, enables the actuators of an analog pad, clears the
 * frames, goes live and restores the saved button layout. */
void Input_Init()
{
    Pad_InitPair(&g_pad, &g_pad2);
    g_inputMgr.SetAcquiredAll(1);
    g_pad.Open(PAD_PORT_1);
    g_pad2.Open(PAD_PORT_2);
    if (g_pad.padTypeReq == PADTYPE_ANALOG)
        g_pad.SetActuator(1);
    if (g_pad2.padTypeReq == PADTYPE_ANALOG)
        g_pad2.SetActuator(1);
    Pad_ClearFrames(&g_pad);
    Pad_ClearFrames(&g_pad2);
    Input_SetMode(INPUT_MODE_LIVE);
    Input_ApplyRemap(g_pProgress->controls.remap[2], g_pProgress->controls.remap[3], g_pProgress->controls.remap[0],
                     g_pProgress->controls.remap[1], g_pProgress->controls.remap[4], g_pProgress->controls.remap[5]);
}

void Input_Reacquire()
{
    g_inputMgr.SetAcquiredAll(1);
    Input_SetMode(INPUT_MODE_LIVE);
}

void Input_Unacquire()
{
    g_inputMgr.SetAcquiredAll(0);
}

/* 0 live, 1 record (snapshots the button map into the recording header), 2 playback (latches both pads).
 * Entering record while playing back, or playback while recording, hangs on purpose (an assert of the PS1 code). */
void Input_SetMode(u8 mode)
{
    switch (mode) {
        case INPUT_MODE_RECORD:
            if (g_inputMode == INPUT_MODE_PLAYBACK)
                while (1)
                    ;
            g_padRec.remap[0] = Input_GetMapping(INPUT_SLOT_CROSS);
            g_padRec.remap[1] = Input_GetMapping(INPUT_SLOT_SQUARE);
            g_padRec.remap[2] = Input_GetMapping(INPUT_SLOT_TRIANGLE);
            g_padRec.remap[3] = Input_GetMapping(INPUT_SLOT_CIRCLE);
            g_padRec.remap[4] = Input_GetMapping(INPUT_SLOT_R1);
            g_padRec.remap[5] = Input_GetMapping(INPUT_SLOT_L1);
            break;
        case INPUT_MODE_PLAYBACK:
            g_pad.Latch();
            g_pad2.Latch();
            if (g_inputMode == INPUT_MODE_RECORD)
                while (1)
                    ;
            break;
    }
    g_inputMode = mode;
}

u8 Input_GetMode()
{
    return g_inputMode;
}

/* loads <name>.PAD into the recording buffer (copying as many bytes as the allocation holds, with no check against the
 * buffer), applies its button layout and starts playback. */
void PadRec_Load(const char *name)
{
    void *data;
    char path[0x40];
    unsigned int size;
    Str_Concat2(path, name, ".PAD");
    data = File_LoadWhole(path);
    if (data != 0) {
        size = _msize(data);
        if (size > sizeof(g_padRecStore)) /* a larger file overran the original's buffer */
            size = sizeof(g_padRecStore);
        memcpy(&g_padRec, data, size);
        if (data != 0) {
            free(data);
            data = 0;
        }
        Input_ApplyRemap(g_padRec.remap[2], g_padRec.remap[3], g_padRec.remap[0], g_padRec.remap[1], g_padRec.remap[4],
                         g_padRec.remap[5]);
        Input_SetMode(INPUT_MODE_PLAYBACK);
    }
}

/* writes the recording (0x28-byte header + dataLen) to ..\Data<name>.pad (no separator) and goes live.
 * No caller. */
void PadRec_Save(const char *name)
{
    char path[0x40];
    u32 size = g_padRec.dataLen + 0x28;
    g_padRec.gameTimeMs = g_gameTimeMs;
    g_padRec.version = 3;
    g_padRec.padType = PADTYPE_ANALOG;
    Str_Concat2(path, "..\\Data", name);
    Str_Concat2(path, path, ".pad");
    File_SaveWhole(path, &g_padRec, size);
    Input_SetMode(INPUT_MODE_LIVE);
}

/* once per frame (Main_Loop): reads g_pad; when live, latches both pads and follows a change of pad type
 * into the saved Progress+0x9c. Only g_pad is ever read: g_pad2 is latched from the frame Pad_ClearFrames left.
 * `buttons` is set and never read. */
void Input_Poll()
{
    Pad_ReadRaw(&g_pad);
    g_padRawSnapshot = g_pad.raw;
    u16 buttons = 0xffff;
    switch (g_inputMode) {
        case INPUT_MODE_LIVE:
            g_pad.Latch();
            g_pad2.Latch();
            if (g_pad.TypeChanged())
                g_pProgress->SetPadIsAnalog(g_pad.IsAnalog());
            if (g_pad.JustConnected())
                g_pad.OnConnected_stub();
            break;
    }
}

/* raw stick bytes (centre 0x80) to axes in -256..256: a SQUARE dead zone (0,0 only when BOTH |x| and |y|
 * are under 56; otherwise the small axis is kept as it is), then each axis clamped to +-120 and scaled by 256/120, so
 * a full diagonal gives (256, 256). */
void Pad_StickToDeadzonedAxes(u8 rawX, u8 rawY, int *outX, int *outY)
{
    s16 xVal = rawX - 0x80;
    s16 yVal = rawY - 0x80;
    s16 xAbs = xVal >= 0 ? xVal : -xVal;
    s16 yAbs = yVal >= 0 ? yVal : -yVal;
    if (xAbs < 0x38 && yAbs < 0x38) {
        *outX = 0;
        *outY = 0;
    } else {
        if (xVal > 0) {
            if (xVal > 0x78)
                xVal = 0x78;
        } else if (xVal < -0x78)
            xVal = -0x78;
        if (yVal > 0) {
            if (yVal > 0x78)
                yVal = 0x78;
        } else if (yVal < -0x78)
            yVal = -0x78;
        *outX = xVal * 256 / 0x78;
        *outY = yVal * 256 / 0x78;
    }
}

/* raw stick bytes to a direction vector: a ROUND dead zone of radius 56, then the length clamped to 120 and
 * mapped 56..120 -> 0..256 along the stick's direction. Returns that strength (0..256). */
int Pad_AnalogToStick(u8 rawX, u8 rawY, int *outX, int *outY)
{
    s16 x = rawX - 0x80;
    s16 y = rawY - 0x80;
    int dist = x * x + y * y; /* squared, then the length, then the 20.12 scale factor */
    if (dist <= 0xc40) {
        *outX = 0;
        *outY = 0;
        return 0;
    }
    dist = (int)sqrt((double)dist);
    int strength = dist;
    if (strength > 0x78)
        strength = 0x78;
    dist = ((strength - 0x38) << 20) / (dist << 6);
    *outX = x * dist / 4096;
    *outY = y * dist / 4096;
    return (strength - 0x38) * 256 / 64;
}

void Input_EmptyStub() {}

u16 Input_GetMapping(u8 slot)
{
    return g_inputMap[slot];
}

/* puts `value` at map slot `slot` by swapping it with wherever it is now, so the map stays a permutation
 * (value not found: the search stops at index 16, one past the table, and that entry is swapped). */
void Input_SwapMapping(u8 slot, u16 value)
{
    u8 i = 0;
    while (g_inputMap[i] != value && i < 0x10)
        i++;
    Swap(g_inputMap[i], g_inputMap[slot]);
    Swap(g_inputMapAux[i], g_inputMapAux[slot]);
}

/* a saved layout: the six remappable slots in the order c, d, e, f, b, a. */
void Input_ApplyRemap(u16 slotC, u16 slotD, u16 slotE, u16 slotF, u16 slotB, u16 slotA)
{
    Input_SwapMapping(INPUT_SLOT_TRIANGLE, slotC);
    Input_SwapMapping(INPUT_SLOT_CIRCLE, slotD);
    Input_SwapMapping(INPUT_SLOT_CROSS, slotE);
    Input_SwapMapping(INPUT_SLOT_SQUARE, slotF);
    Input_SwapMapping(INPUT_SLOT_R1, slotB);
    Input_SwapMapping(INPUT_SLOT_L1, slotA);
}

/* the options screen's control preset (Progress+0x9a): three fixed layouts or the custom one. */
void Input_ApplyControlConfig(ControlConfig *cfg)
{
    switch (cfg->preset) {
        case CTRL_PRESET_A:
            Input_ApplyRemap((u16)~PAD_TRIANGLE, (u16)~PAD_CIRCLE, (u16)~PAD_CROSS, (u16)~PAD_SQUARE, (u16)~PAD_R1,
                             (u16)~PAD_L1);
            break;
        case CTRL_PRESET_B:
            Input_ApplyRemap((u16)~PAD_SQUARE, (u16)~PAD_TRIANGLE, (u16)~PAD_CIRCLE, (u16)~PAD_CROSS, (u16)~PAD_R1,
                             (u16)~PAD_L1);
            break;
        case CTRL_PRESET_C:
            Input_ApplyRemap((u16)~PAD_CROSS, (u16)~PAD_SQUARE, (u16)~PAD_TRIANGLE, (u16)~PAD_CIRCLE, (u16)~PAD_R1,
                             (u16)~PAD_L1);
            break;
        case CTRL_PRESET_CUSTOM:
            Input_ApplyRemap(cfg->remap[2], cfg->remap[3], cfg->remap[0], cfg->remap[1], cfg->remap[4], cfg->remap[5]);
            break;
    }
}

/* the live map into the custom layout. */
void Input_StoreControlConfig(ControlConfig *cfg)
{
    cfg->remap[2] = Input_GetMapping(INPUT_SLOT_TRIANGLE);
    cfg->remap[3] = Input_GetMapping(INPUT_SLOT_CIRCLE);
    cfg->remap[0] = Input_GetMapping(INPUT_SLOT_CROSS);
    cfg->remap[1] = Input_GetMapping(INPUT_SLOT_SQUARE);
    cfg->remap[4] = Input_GetMapping(INPUT_SLOT_R1);
    cfg->remap[5] = Input_GetMapping(INPUT_SLOT_L1);
}

void Pad::ResetState()
{
    padType = PADTYPE_NONE;
    padTypeReq = PADTYPE_NONE;
    state59 = 0;
    state5a = 0;
    state5c = 0;
    state5d = 0;
    state44 = 0;
    state48 = 0;
    state4c = 0;
    state60 = 0;
    state50 = 0x1000;
    state54 = 0;
}

/* auto-repeat (`this` unused): the value passes through on the frame it changes and again every
 * g_padRepeatDelay ms while it is held (500 ms first, then 125 ms; the delay is ONE global shared by every channel of
 * both pads); in between the output is 0xffff. A repeat pulse ORs in 0xff06: in the active-low word that releases the
 * whole upper byte (L2 R2 L1 R1 Triangle Circle Cross Square) and L3/R3, so only the d-pad, Select and Start repeat. */
void Pad::AutoRepeat(PadRepeat *rep, u16 value)
{
    rep->output = value;
    if (rep->output == rep->lastValue) {
        rep->timerMs += g_dtRawMs;
        if (rep->timerMs < g_padRepeatDelay) {
            rep->output = 0xffff;
        } else {
            g_padRepeatDelay = 0x7d;
            rep->timerMs = 0;
            rep->output |= PAD_L3 | PAD_R3 | PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_TRIANGLE | PAD_CIRCLE | PAD_CROSS |
                           PAD_SQUARE;
        }
    } else {
        g_padRepeatDelay = 500;
        rep->lastValue = rep->output;
        rep->timerMs = 0;
    }
}

/* an exact copy of Pad::Latch (no caller found). */
void Pad::Latch_Dup()
{
    raw.buttons |= ~((u16)~PAD_L3 & (u16)~PAD_R3);
    if (!IsConnected())
        raw.buttons = PAD_ALL_RELEASED;
    AutoRepeat(&btnRepeat, cur.buttons);
    AnalogToDpadBits();
    prev = cur;
    cur = raw;
}

/* the frame step: L3/R3 forced released (bits 1 and 2; the constant is 0xffff0006, i.e. ~0xfff9), all
 * released when disconnected, the button auto-repeat and the stick-as-d-pad word updated, then prev = cur and
 * cur = raw. "Disconnected" is the status of the frame latched LAST time, so a failed poll is seen one frame late. */
void Pad::Latch()
{
    raw.buttons |= ~((u16)~PAD_L3 & (u16)~PAD_R3);
    if (!IsConnected())
        raw.buttons = PAD_ALL_RELEASED;
    AutoRepeat(&btnRepeat, cur.buttons);
    AnalogToDpadBits();
    prev = cur;
    cur = raw;
}

/* the button word on a frame it changed while something outside ignoreMask is down, else 0xffff. */
u16 Pad::GetChangedPress(u16 ignoreMask)
{
    if (cur.status == 0 && cur.buttons != prev.buttons && (cur.buttons | ignoreMask) != PAD_ALL_RELEASED)
        return cur.buttons;
    return PAD_ALL_RELEASED;
}

s32 Pad::IsConnected()
{
    return !cur.status;
}

/* empty (a PS1 libpad call stubbed out) */
void Pad::Stub_55f078(u32 arg) {}

/* empty: no rumble on PC */
void Pad::Rumble_stub(s32 durationMs, const u8 *pattern, s16 strength) {}

/* empty */
void Pad::OnConnected_stub() {}

/* empty */
void Pad::Stub_55f09d() {}

/* retries the first poll up to 60 times; then the same type detection as Pad_DetectType (on this pad).
 * `port` (0, 0x10: the PS1 port numbers) is unused; both pads poll the one input manager. */
void Pad::Open(int port)
{
    int tries = 0;
    while (g_inputMgr.Poll() < 0 && tries < 0x3c)
        tries++;
    if (tries == 0x3c)
        return;
    if (g_inputMgr.IsDeviceDigital(0) == 1) {
        padType = PADTYPE_DIGITAL;
        padTypeReq = PADTYPE_DIGITAL;
    } else {
        state59 = 0;
        state5a = 0;
        state44 = 0;
        state48 = 0;
        state4c = 0;
        state60 = 0;
        actuatorEnable = 0;
        state54 = 0;
        padType = PADTYPE_ANALOG;
        padTypeReq = PADTYPE_ANALOG;
    }
}

/* both frames have a type and it differs */
s32 Pad::TypeChanged()
{
    return cur.typeLen.type != PADTYPE_NONE && prev.typeLen.type != PADTYPE_NONE &&
           cur.typeLen.type != prev.typeLen.type;
}

/* a type now, none in the previous frame */
s32 Pad::JustConnected()
{
    return cur.typeLen.type != PADTYPE_NONE && prev.typeLen.type == PADTYPE_NONE;
}

/* menuCur = raw buttons with the left stick folded in as d-pad bits (past a dead zone of 56 per axis:
 * right 0x20, left 0x80, down 0x40, up 0x10), plus the controller's physical D-pad for menus only,
 * then its auto-repeat (g_padMenuRepeat). */
void Pad::AnalogToDpadBits()
{
    s16 x = raw.leftX - 0x80;
    s16 y = raw.leftY - 0x80;
    menuPrev = menuCur;
    menuCur = raw.buttons;
    if ((x >= 0 ? x : -x) < 0x38)
        x = 0;
    else if (x > 0)
        menuCur &= (u16)~PAD_RIGHT;
    else
        menuCur &= (u16)~PAD_LEFT;
    if ((y >= 0 ? y : -y) < 0x38)
        y = 0;
    else if (y > 0)
        menuCur &= (u16)~PAD_DOWN;
    else
        menuCur &= (u16)~PAD_UP;
    if (this == &g_pad && raw.status == 0 && g_inputMgr.joystick &&
        g_inputMgr.current == g_inputMgr.joystick) {
        unsigned directions = Platform_ReadJoystickDpad();
        if (directions & PLATFORM_DPAD_UP)
            menuCur &= (u16)~PAD_UP;
        if (directions & PLATFORM_DPAD_RIGHT)
            menuCur &= (u16)~PAD_RIGHT;
        if (directions & PLATFORM_DPAD_DOWN)
            menuCur &= (u16)~PAD_DOWN;
        if (directions & PLATFORM_DPAD_LEFT)
            menuCur &= (u16)~PAD_LEFT;
    }
    AutoRepeat(&menuRepeat, menuCur);
}

/* Pad_MenuHeld on the auto-repeat word: true on the press frame and on each repeat, with the same keyboard
 * fallbacks (Cross = Enter, Triangle = Esc, the d-pad masks = the arrow-key nav word). */
bool Pad_MenuRepeat(int activeLowMask)
{
    bool pulse = (g_padMenuRepeat & ~activeLowMask) == 0;
    switch (activeLowMask) {
        case (u16)~PAD_CROSS:
            pulse |= g_inputMgr.enterPressed == 1;
            break;
        case (u16)~PAD_TRIANGLE:
            pulse |= g_inputMgr.escPressed == 1;
            break;
        case (u16)~PAD_LEFT:
            pulse |= g_inputMgr.menuNav == (u16)~PAD_LEFT;
            break;
        case (u16)~PAD_RIGHT:
            pulse |= g_inputMgr.menuNav == (u16)~PAD_RIGHT;
            break;
        case (u16)~PAD_UP:
            pulse |= g_inputMgr.menuNav == (u16)~PAD_UP;
            break;
        case (u16)~PAD_DOWN:
            pulse |= g_inputMgr.menuNav == (u16)~PAD_DOWN;
            break;
    }
    return pulse;
}

/* is the pad button whose bit is CLEAR in activeLowMask held? The menu masks also accept the keyboard:
 * Cross 0xBFFF = Enter, Triangle 0xEFFF = Esc, and the four d-pad masks = the arrow-key nav word. Those keyboard inputs
 * are edges/auto-repeat pulses, so on the keyboard "held" is only true on the press and repeat frames. */
bool Pad_MenuHeld(int activeLowMask)
{
    bool held = (g_padMenuCur & ~activeLowMask) == 0;
    switch (activeLowMask) {
        case (u16)~PAD_CROSS:
            held |= g_inputMgr.enterPressed == 1;
            break;
        case (u16)~PAD_TRIANGLE:
            held |= g_inputMgr.escPressed == 1;
            break;
        case (u16)~PAD_LEFT:
            held |= g_inputMgr.menuNav == (u16)~PAD_LEFT;
            break;
        case (u16)~PAD_RIGHT:
            held |= g_inputMgr.menuNav == (u16)~PAD_RIGHT;
            break;
        case (u16)~PAD_UP:
            held |= g_inputMgr.menuNav == (u16)~PAD_UP;
            break;
        case (u16)~PAD_DOWN:
            held |= g_inputMgr.menuNav == (u16)~PAD_DOWN;
            break;
    }
    return held;
}

/* as Pad_MenuHeld, but only on the frame the button goes down (down now, up in the previous frame). */
bool Pad_MenuPressed(int activeLowMask)
{
    bool pressed = (g_padMenuCur & ~activeLowMask) == 0 && (g_padMenuPrev & ~activeLowMask) != 0;
    switch (activeLowMask) {
        case (u16)~PAD_CROSS:
            pressed |= g_inputMgr.enterPressed == 1;
            break;
        case (u16)~PAD_TRIANGLE:
            pressed |= g_inputMgr.escPressed == 1;
            break;
        case (u16)~PAD_LEFT:
            pressed |= g_inputMgr.menuNav == (u16)~PAD_LEFT;
            break;
        case (u16)~PAD_RIGHT:
            pressed |= g_inputMgr.menuNav == (u16)~PAD_RIGHT;
            break;
        case (u16)~PAD_UP:
            pressed |= g_inputMgr.menuNav == (u16)~PAD_UP;
            break;
        case (u16)~PAD_DOWN:
            pressed |= g_inputMgr.menuNav == (u16)~PAD_DOWN;
            break;
    }
    return pressed;
}
