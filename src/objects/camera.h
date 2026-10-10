#ifndef SDW_OBJECTS_CAMERA_H
#define SDW_OBJECTS_CAMERA_H

/* The functions and globals camera.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

struct CamRestrict;
class Camera;
class CollBox;
class Pad;
class ScnObject;
struct Trajectory;
class box;

extern s16 g_camMinPitch;
extern u8 g_camMode;            /* CameraMode (CAM_LOOK = look mode) */
extern s16 g_camReqDist;
extern s16 g_camReqFocal;
extern s16 g_camReqSpeedFactor;
extern CamRestrict *g_camRestriction;
extern u32 g_camScriptFlags;
extern ScnObject *g_camScriptOwner;
extern s16 g_camShakeAmplitude;
/* Slider positions: 0 = 0.5x, 16 = 1x, 32 = 2x speed / 12x acceleration. */
enum { CAMERA_CONTROL_CENTRE = 16, CAMERA_CONTROL_MAX = 32 };
extern u8 g_camSpeedSetting;
extern u8 g_camAccelerationSetting;
extern u8 g_camInvertJoystick;
void Camera_SaveControlSettings();
void Camera_ApplyRestriction(Vec3s *, s32 *, s16 *, s16 *, const CamRestrict *);
void Camera_BuildViewMatrix(Camera *, const Vec3s *);
void Camera_ClampToTrajectory(Vec3s *, Trajectory *, s32);
void Camera_EaseFocal(s16, s16, u16);
void Camera_EaseLerpAngles(Vec3s *, const Vec3s *, const Vec3s *, const u8 *, u16, u16, u16);
void Camera_EaseLerpVec3s(Vec3s *, const Vec3s *, const u8 *, u16, u16, u16);
void Camera_EyeToAngles(const Vec3s *, Vec3s *, s16 *, const Vec3s *);
s16 Camera_FindClearDistance(const Vec3s *, const Vec3s *, s16);
void Camera_FollowTrajectory(Trajectory *, Vec3s *, s16 *, s32 *, s32);
void Camera_FreeLevel();
void Camera_InitOnceStub();
void Camera_InitSettings();
s16 Camera_IsEyeClear(const Camera *, const Vec3s *);
s32 Camera_IsPointOnScreen(Camera *, Vec3s, s16, const Vec3s *, const Vec3s *, s32 *);
void Camera_OnManualRotate(s32);
void Camera_ReleaseAny();
void Camera_ReleaseScripted(ScnObject *owner);
void Camera_Reset();
void Camera_ResetMotion();
void Camera_ResetUnderwaterOverlay();
void Camera_SetMode(u8 mode, u8 smooth);
void Camera_SetSamChase(Vec3s *position, short y, CollBox *box);
void Camera_ShakeOffset(Vec3s *);
void Camera_SmoothToDesired(Camera *, Vec3s *, const s32 *, s16 *, s16, const Vec3s *, s32);
void Camera_SolveCollision(Camera *, Vec3s *, s16, s32, s16);
void Camera_StartShake(s16 amplitude, s32 durationTicks);
s16 Camera_StickAxisDeadzone(u8);
void Camera_StopShake();
u16 Camera_SweepEyeBox(const Vec3s *, const s16 *, Vec3s *, const Vec3s *, Vec3s *);
void Camera_Update(Camera *cam, u8 debugMode, Pad *pad, Pad *pad2);
void Camera_UpdateDebugFreeCam(Camera *, Pad *, Pad *);
void Camera_UpdateDirected(Camera *, Pad *);
void Camera_UpdateFollow(Camera *, Pad *);
void Camera_UpdateLedgeLook(Camera *);
void Camera_UpdateLook(Camera *, Pad *);
void Camera_UpdateRocket(Camera *, Pad *);
void Camera_UpdateScriptBlendIn(Camera *);
void Camera_UpdateScriptToScript(Camera *);
void Camera_UpdateScriptedHold(Camera *);
void Camera_UpdateUnderwater();
void Color_ScaleClamp(const u8 *src, u8 *dst, float scale, u8 maximum);
void Vec3s_OffsetAlongAngles(Vec3s *, const Vec3s *, s16, const Vec3s *);

/* The functions and globals camera.cpp defines, declared once for every file that uses them. */

struct CamFlagBits;
struct CamModeParams;
struct CamRequestBits;

extern Vec3s g_camChaseTarget2;
extern CamFlagBits g_camFlags;
extern CamModeParams g_camModeParams[]; /* [13]; [6] is the robot camera */
extern Vec3s g_camReqAimOffset;
extern CamRequestBits g_camReqFlags;
extern Vec3s g_camReqRot;               /* pitch, yaw, roll */
extern Vec3s g_camReqTarget;
extern Vec3s g_camReqVel;

#endif
