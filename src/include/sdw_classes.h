// Declare a class's constructors and non-virtual methods by defining
// SDW_MEMBERS_<Class> (or, on top of a shared member header, SDW_EXTRA_<Class>) before including this file.
#ifndef SDW_CLASSES_H
#define SDW_CLASSES_H
#include "sdw_types.h"
#include "sdw_fileptr.h"
#include "sdw_render.h"
#include "../sdk/windef.h" /* GUID, RECT */

#define SDW_PURE

struct Aabb;
struct ActionHit;
class AltModel;
class AmbientSoundManager;
struct AnimHeader;
struct AnimJointPose;
struct AnimKey;
class AnimMesh;
class AnimSprite;
struct AnimatedWorldObj;
struct Animator;
class Anvil;
struct AttachLink;
struct AttachLinkBits;
class AutomaticDoor;
struct BallBits;
class Bat;
class Battery;
class Bees;
class Bell;
class BipbipLevel14;
class Bird;
class BlackHole;
struct BlackHoleFlagBits;
struct BonusEntry;
class BonusManager;
struct Box6i;
struct BoxCrate;
class BsFile;
class BsPolyBlendFlat;
class BsPolyBlendGouraud;
class BsPolyFlat;
class BsPolyGouraud;
class BsPolyTexFlat;
class BsPolyTexGouraud;
class BsStream;
class Bullet;
class Bush;
struct BushFlagBits;
class Butterfly;
class Cactus;
struct CamFlagBits;
struct CamMgrEntry;
struct CamModeParams;
struct CamPitchAdjBits;
struct CamProbeFrame;
struct CamRequestBits;
struct CamRestrict;
struct CamRestrictBits;
struct CamSamPush;
struct CamSetup;
class CamShot;
class Camera;
class CameraManager;
class CameraManager2;
class CameraRestriction;
class CannonBall;
class CannonBall2;
class CanonDummy;
class CanonSheep;
class CanonSimple;
class Case;
class Catapult;
struct CatapultBits;
struct CheckpointEntry;
class CheckpointManager;
class Chronometer;
class Cine;
struct CineActorMsg;
struct CineActorSave;
struct CineActorSlot;
struct CineAttach;
struct CineDialogue;
struct CineRecord;
struct CineTrack;
struct CineTrigger;
class CinematicsManager;
class CollBox;
struct CollCell;
struct CollContact;
struct CollMapHeader;
struct CollRay;
struct CollRayHit;
struct CollTri;
class CompositeRod;
struct ContactInfo;
struct ControlConfig;
class Crane;
class CreditsManager;
class CrocodileLevel09;
class CrocodileLevel11;
class Crowd;
class CrumblyGround;
class CrumblyPlat;
class D3DApp;
struct D3DDeviceInfo;
struct DIJoystickInfo;
class DaffyElf;
class DaffyLevel01;
struct DaffyLevel01FlagBits;
class DaffyLevel02;
class DaffyLevel09;
class DaffyMilitary;
class DaffyScene;
class DaffyTrainingLevel;
class DaffyWheel;
struct DanceStep;
class DancingGhost;
struct DancingGhostFlagBits;
class DancingGhostManager;
struct Dav;
struct DavBitmapRec;
struct DavDirectory;
struct DavHeader;
class DefusableMine;
struct DefusableMineBits;
struct DialogBox;
struct DialogueShownBits;
struct DialogueShownFlags;
class Diamond;
class DoorLevel;
class DoorMechanism;
class DoorWorld;
class Dragon;
struct DrawMsgArgs;
struct DropMsgArg;
class Dynamite;
struct DynamiteFlagBits;
class ElasticTree;
class Elmer;
struct EmitterColumnParams;
struct EmitterDriftParams;
struct EmitterFadeParams;
struct EmitterFlagBits;
struct EmitterPerfumeParams;
struct EmitterRiseParams;
struct EmitterTrailParams;
struct FILE;
class FacingCamera;
class FallingGate;
class FallingGate2;
class FallingRock;
class Fan;
struct FanReport;
struct FileHandle;
class FireBall;
class Firefly;
class Fish;
class FishingRod;
class FloatingBox;
struct FlockScentSource;
class Flute;
struct FmvList;
class FogManager;
struct Font;
class FrozenRiver;
class Frustrum;
struct GUID;
class GameState;
class GeyserIn;
class GeyserManger;
class GeyserOut;
class Ghost;
class GhostCostume;
class GhostHalo;
struct GhostNode;
struct GhostTravel;
class Goal;
struct GoalFlagBits;
class GoldenCoins;
class GossamerOnde;
class Gossamer_Boss;
class Gossamer_Lev08;
class GroundMine;
struct GroundMineFlagBits;
struct GroundQuery;
struct HINSTANCE__;
struct HMMIO__;
struct HWND__;
class HairDryer;
class Heap;
struct HeapBlock;
class HeapOfLeaf;
class HiddenRocks;
class HitSwitch;
class Hive;
class HoleFX;
class HoneyPot;
class Hoover;
struct IAMMultiMediaStream;
struct IBaseFilter;
struct IDirect3D7;
struct IDirect3DDevice7;

struct IDirectDraw7;
struct IDirectDrawStreamSample;
struct IDirectDrawSurface;
struct IDirectDrawSurface7;
struct IDirectInput8A;
struct IDirectInputDevice8A;
struct IDirectSound;
struct IDirectSoundBuffer;
struct IDirectSoundNotify;
struct IGraphBuilder;
struct IUnknown;
class IceCube;
class IceGround;
struct IdleAnimEntry;
struct InflatableFlagBits;
class InflatableSheep;
class InlineEmitter1;
class InlineEmitter10;
class InlineEmitter16;
class InlineEmitter3;
class InlineEmitter32;
class InlineEmitter4;
class InlineEmitter6;
class InlineEmitter8;
struct InputBinding;
struct InputBindingIdx;
class InputDevice;
class InputMgr;
class Instance;
struct InstanceBase;
class InstantHoover;
class InstantMartian;
class InstantSocket;
struct InteractScan;
class Jail;
class Joystick;
class Key;
class Keyboard;
class Laser;
class LaunchArc;
struct LaunchArcFlagBits;
class Lava;
class LazerRobot;
class Leaf;
class LightSpot;
struct ListNode;
class MCardManager;
struct MMCKINFO;
class Magnet;
class MagnetRod;
class Mailbox;
struct MailboxFlagBits;
class Map;
struct MapLocSlot;
class MapLocation;
struct MapMarker;
class Marvin;
struct MarvinFlagBits;
struct Mat34s;
class Mat44;
struct Menu;
struct MenuBox;
struct MenuPage;
struct MenuState;
class Mesh;
class MeshAnimFrame;
class MeshAnimSeq;
class MeshPart;
class MeshPartPose;
class Mine;
class MineDetector;
struct MineFlagBits;
class MirrorManager;
struct MltHeader;
struct Model;
struct ModelBoxList;
struct ModelJoint;
class Monolithe;
struct MonolitheRider;
class Mouse;
struct MoveModifyArg;
struct MoveRecord;
struct NavNode;
struct NavSearch;
struct ObjGridHeader;
struct ObjMgrEntry;
class ObjectManager;
class PackJpeg;
struct PackJpegEntry;
struct PackJpegImage;
class Pad;
struct PadFrame;
struct PadRecHeader;
struct PadRepeat;
struct PadTypeLen;
struct Particle;
class ParticleEmitter;
class PathFollower;
class Perfume;
struct PerfumeFlagBits;
struct PerfumeScentReport;
class Pipe;
class Pipe2;
class Piranhas;
class PolyBatcher;
class PolyTri;
class PorkyLevel01;
class PrayingGhost;
class Progress;
struct ProgressOptionBits;
struct ProgressRuntimeFlagBits;
struct RCarpetFlagBits;
class RCarpetMobile;
struct RECT;
class RabbitCostume;
class Raft;
struct RaftFlagBits;
class RemoteControl;
class RenderPoly;
class Resizer;
struct ResolveScratch;
struct RiverCargo;
class Robot;
struct RobotStateDesc;
class Rock;
struct RockFlagBits;
class Rocket;
class Rocks;
struct RodFlagBits;
class RodParts;
class RollingCarpet;
struct RollingCarpetFlagBits;
class Rook;
class Sail;
class Salad;
class SaladRod;
class Sam;
struct SamBeachBits;
struct SamCarryGoal;
struct SamChaseBits;
struct SamChaseSoundBits;
struct SamContactInfo;
struct SamEdgeNormal;
struct SamFetchBits;
struct SamFetchFlags;
struct SamFetchGoal;
struct SamFollowBits;
struct SamPathHist;
struct SamSheepBits;
struct SamTracked;
class Sam_Pirate;
class SceneSheepPanel;
class Scene_Wheel;
class ScnBody;
struct ScnClassRegEntry;
class ScnControllable;
class ScnLogic;
class ScnLogicShadowed;
class ScnMobile;
class ScnObject;
struct ScnRecordSynth;
class Screen;
class ScrollText;
struct ScrollTextFlagBits;
class Seaweed;
class SecretDoor;
class Seed;
struct SeesawBodyEntry;
struct SeesawPendingBits;
class SensibleButton;
struct SensibleButtonFlagBits;
class SfxCineManager;
class Shadow;
struct ShadowScratch;
class Shark;
struct SharkFxScratch;
class Sheep;
class SheepCostume;
class SignPost;
class SignPostAnimated;
class SignPostSimple;
class SignTips;
class SlidingIceCube;
struct SlidingIceCubeFlagBits;
class SmallRock;
struct SmallRockFlagBits;
struct SmallRockMoveBits;
struct SndBankEntry;
class Snowball;
class SnowyGround;
class Sound;
struct SoundChannel;
class SoundDevice;
struct SoundDeviceEntry;
class Sprite;
struct SpriteFrame;
class StaticSound;
class StreamPlayer;
class StreamSound;
struct StringBank;
class SuperButton;
struct SuperButtonInput;
class SwirlSign;
class Telescope;
struct TexScroll;
struct TextBox;
class TextResBank;
struct TextResEntry;
class Texture;
class TimeKeeper;
class TimeMachineChrono;
struct TimeMachinePair;
class TimeMachineSphere;
struct TimeTravelArg;
class Timer;
class Torch;
class TrafficJams;
class TrailEmitter;
class Train;
struct TrainCar;
class TrainCarBody;
class TrainStation;
struct TrainWaypoint;
struct TrainingHelpSlot;
struct TrainingLineBits;
class TrajFollower;
class TrajPatrol;
struct Trajectory;
struct Trajectory3;
class Tree;
class TreeSection;
class TriggedStone;
class Twig;
class UiCursor;
struct UiCursorFrame;
struct UiFrame;
class UiIcon;
class UiQuad;
class Umbrella;
struct UmbrellaFlagBits;
class Vdx7;
struct Vdx7Record;
struct Vec2s;
class Vec3f;
struct Vec3i;
struct Vec4i;
struct Vec4s;
class Video;
class VideoPlayer;
class VisibilityManager;
class Volcano;
struct WAVEFORMATEX;
class WallAvoid;
struct WallAvoidFlagBits;
struct WarFile;
struct WarHeader;
struct WarLevelHeader;
class Watch;
class WaterGeyser;
class WaterMine;
struct WaterMineFlagBits;
class WaveFile;
class Weather;
struct WeatherParticle;
struct WeatherTex;
class Wheel;
class WheelDummy;
class Wolf;
struct WolfArcScratch;
struct WolfClimbScratch;
struct WolfDanceMove;
struct WolfFloatScratch;
struct WolfHoldScratch;
struct WolfLaunchAxis;
class WolfLaunchPath;
struct WolfLineArg;
struct WolfMoveBank;
struct WolfMoveScratch;
struct WolfRespawnArg;
struct WolfSpotArg;
struct WolfStateDesc;
struct WolfSwimScratch;
class WolfTrap;
struct WolfWalkToArg;
class WoodenLift;
class WoodenPlatForm;
class WorldObj;
class ZoneList;
class balance;
class bipbip;
class box;
class bridge;
class bull;
class elastic;
class seesaw;

struct IDirectDrawSurface;
struct RECT;
typedef s32 (*VideoFrameProc)(struct IDirectDrawSurface *surface, struct RECT *rect);   // Video's per-frame callback: set by Video_SetFrameCallback, called cdecl with the frame surface and source rect
typedef struct ScnObject * (*ScnFactoryFn)(void *record);   // a scenaric class factory (<Class>_Create): stored by Scenaric_RegisterClass_2 in ScnClassRegEntry.factory and called by Scenaric_CreateObject with the object's WAR record

struct Aabb {
public:
    Vec3s min;                                     // minimum corner
    u8 _pad006[0x2];
    Vec3s max;                                     // maximum corner (Cull_IsAabbVisible reads it as words 4..6)
};

struct ActionHit {
public:
    s32 action;                                    // nonzero reply of the candidate to msg 2 (primary) or 0x26 (secondary); ScnControllable_ScanInteracta
    ScnObject *target;                             // the object that gave the reply
};

class AltModel {
public:
#ifdef SDW_MEMBERS_AltModel
    SDW_MEMBERS_AltModel
#endif
    s32 IsValid();
    u16 modelResIdx;                               // DAV resource index of the model, 0xffff = none; written into WAR record word 0 on swap
    u16 res2Idx;                                   // secondary resource index (record word 1)
    void *animBuf;                                 // joint-buffer block (nbJoints*0x78) handed to Animator_Init on swap; NULL for rigid models
    Vec3s boundCenter;                             // copied to obj+0x28 on swap
    u16 boundRadius;                               // copied to obj+0x2e on swap
};

struct InstanceBase {
public:
    u16 inst_flags;                                // Instance flags: 1 drawn/visible last render, 2 (mode bit 5), 4 animated, 8 attached to parent, 0x10
    u8 inst_mode;                                  // Instance_InitBase stores its mode argument here: 0xFF ScnBody, 0xFE ScnLogic, or for world objects t
    u8 inst_kind;                                  // 0 = rigid scenaric instance, 4 = animated (set by Animator_Init). World objects use 1/2/3 for resour
    Model *inst_model;                             // model resource; model+0xc box list {s32 n; Box16[n]}, +0x10 anim table, +0x14 joints (10 B each), +0
};

class Instance : public InstanceBase {
public:
    Instance *Inst();
    Vec3s pos;                                     // world position (y down)
    u8 attachedChildCount;                         // Also the length of this parent's run of consecutive AttachLink slots in g_attachLinkPool. AttachLink
    u8 partHeight;                                 // height in sixteenths (inferred: the classes store value >> 4, e.g. Train 0x80, Wolf 0x100, Bees 0x18
    Vec3s rot;                                     // rotation, +0x16 = facing (4096/turn)
    s16 tintAmount;                                // colour tint strength 0..0x1000 (0x1000 burnt, 0xC00 shadow zone, glow by distance)
    union {
        u32 tintColor;                                 // tint RGB (0 = black, for glow)
        u8 tintBytes[4];                           // the tint colour's bytes: [0] blue, [1] green, [2] red
    };
    u16 *record;                                   // WAR scenaric record; designer properties at record+0x14
    AttachLink *attachLink;                        // parent attachment link (pool of 10; link+0x18 parent instance, +0x1c parent obj, +0x20 f
    Vec3s boundCenter;                             // model-space bounding centre
    u16 boundRadius;                               // bounding half-extent used for render culling
    void *secondaryRes;                            // record u16[1] resource (type 3), set with inst flag 0x40
};

class ScnObject : public Instance {
public:
#ifdef SDW_MEMBERS_ScnObject
    SDW_MEMBERS_ScnObject
#endif
#ifdef SDW_EXTRA_ScnObject
    SDW_EXTRA_ScnObject
#endif
    virtual void PostLoadInit() SDW_PURE;          // _purecall
    virtual void Update() SDW_PURE;                // _purecall
    virtual void Render(Camera *view) SDW_PURE;    // _purecall
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode) SDW_PURE; // _purecall
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // ScnObject_HandleMessage_default
    virtual void Reset();                          // ScnObject_Nop
    virtual void RenderScaled(Camera *view, Vec3s *scale) SDW_PURE; // _purecall
    virtual void SetPosition(Vec3s *pos);          // ScnObject_SetPosition
    CollBox *FirstBox();
    CollBox *GetFirstModelBoxInline();
    CollBox *GetModelBox();
    ScnObject *GetParent();
    s16 Facing();
    s16 GetFacing();
    s16 GetHeading();
    s32 FlagsClear(u32 mask);
    s32 InWorld();
    s32 InstFlags(u16 mask);
    s32 InstanceFlags(u16 mask);
    s32 IsActive();
    s32 IsCollidable();
    s32 IsInWorld();
    s32 IsKept();
    s32 IsSoundPlaying(u16 handle);
    s32 IsVisible();
    s32 SoundIsPlaying(u16 handle);
    u16 GetClassId();
    u16 GetFlags();
    u16 PlaySound(u16 id, u16 volume, u8 flags, s32 pitch);
    void *Record();
    void BroadcastAround(s32 below, s32 above, u16 radius, u32 msg, void *arg);
    void Drop(Vec3s *where);
    void EnableBoxCollide(s32 on);
    void EnableTint(s32 on);
    void SetAttachment(u8 joint, const Vec3s *localOffset, const Vec3s *rotation, s32 rootRotation, const Vec3s *worldOffset);
    void SetBoxCollide(s32 on);
    void SetCollidable(s32 solid);
    void SetCollision(s32 on);
    void SetContactEnabled(s32 on);
    void SetDrawMode(u32 mode);
    void SetFlag40(s32 on);
    void SetHeading(s16 value);
    void SetInstFlag(u16 mask, s32 on);
    void SetKept(s32 on);
    void SetMovementEnabled(s32 on);
    void SetNoCull(s32 on);
    void SetNoDistCull(s32 on);
    void SetPartHeight(u32 value);
    void SetSoundRate(u16 handle, s32 rate);
    void SetTintOverride(s32 on);
    void SetTinted(s32 on);
    void SetVisible(s32 on);
    void StartCameraBlended(u16 rotX, u16 rotY, u16 rotZ, Vec3s *eye, u16 focal);
    void StopSound(u16 sound);
    void StopSoundHandle(u16 handle);
    s32 IsRocket(ScnObject *obj);
    s32 GetSideQuadrant(ScnObject *other);/* inline: partHeight = v >> 4; the constant is loaded into a register and shifted unsigned ( ; the same *//* with 0x100 and 0x180) */
    s32 IsOnSameSideAs(ScnObject *a, ScnObject *b);
    s16 GetSideHeading(ScnObject *);
    void CalcFacingCameraRot(Vec3s *outRot, Camera *view, s32 yawOnly, const Vec3s *baseRot);
    ScnObject *InitWithAltModels(void *record, AltModel *outMain, s32 count, const u16 *ids, AltModel *outAlts);
    void Translate(Vec3s *delta);
    s32 Scenaric_FindActiveIndex();
    s32 Scenaric_FindObjectIndex();
    void AddToWorld(Vec3s *posOrNull);
    void RemoveFromWorld();
    u16 Collide_ResolveMove(Vec3s *ioDelta, ContactInfo *outInfo, u16 floorNormalCutoff, u16 flags, Vec3s *startPosOrNull, CollBox *boxOrNull, s16 stepHeight, ScnObject **excludeList, s32 excludeCount);
    void DisableBoxes(u32 mask);
    void EnableBoxes(u32 mask);
    CollBox *GetFirstSolidBox();
    s16 World_GroundYRay(Vec3s *pos, s32 includeObjects);
    s16 QueryGroundY(Vec3s *pos, s32 includeObjects);
    s16 QueryGroundY_Shrunk(Vec3s *pos);
    s16 QueryGroundYAndObject(Vec3s *pos, ScnObject **outHitObj);
    s32 TestBodyAt(Vec3s *pos, u8 mode);
    ScnObject *Scenaric_FindNearestOfClass(Vec3s *center, s16 classId, s16 minY, s16 maxY, u16 radius, s16 *outDist, s32 includeHidden);
    void Scenaric_BroadcastInRadius(u16 classFilter, s16 minY, s16 maxY, u16 radius, u32 msgId, void *arg, s32 includeHidden);
    char *Text_GetClassString(u8 index);
    void Detach();
    void SnapToGround(s32 includeObjects);
    void AltModel_InitRigid(AltModel *out, u16 idListId);
    void AltModel_SaveRigid(AltModel *out);
    s32 IsRespawnWobbleDone(s32 elapsed);
    void RenderRespawnWobble(Camera *view, s32 elapsed);
    void TrajFollower_Init(TrajFollower *f, Trajectory *trajectory, s16 speed, s16 headingBias, u32 continuousHeading, u32 use3dDistance, s16 arriveRadius);
    s32 TrajFollower_Step(TrajFollower *f, Vec3s *outVel, s16 *outHeading);
    s16 HeadingTo(Vec3s *target);
    s32 Scenaric_SendToClass(u16 classId, s32 msg, void *arg);
    void RenderPivoted(Camera *view, Vec3s *pivot, Vec3s *scaleOrNull);
    void RenderFacingCamera(Camera *view, s32 yawOnly, Vec3s *scaleOrNull, Vec3s *baseRotOrNull);
    u32 camDist2;                                  // squared distance to camera, written by Scenaric_UpdateAll
    u16 classId;                                   // ScenaricClassId (0xffff = classless)
    u16 flags;                                     // ScnObjFlags
    u16 weight;                                    // Base field. Mass used by seesaw_Update: torque = Î£((axisCoord - centreOnAxis) * weight * contactSpee
    u16 pad3e;                                     // tail padding: derived-class fields start at +0x40 (ScnLogic/ScnBody); makes Ghidra print +0x44 as a
};

class ScnLogic : public ScnObject {
public:
    virtual void PostLoadInit() {}                 // ScnObject_Nop (override)
    virtual void Update() {}                       // ScnObject_Nop (override)
    virtual void Render(Camera *view);             // ScnLogic_Render (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode) { return 0; } // ScnObject_CustomCollide_default (override)
    virtual void RenderScaled(Camera *view, Vec3s *scale); // ScnLogic_RenderScaled (override)
    virtual ScnObject* Init(void *record);         // ScnLogic_Init
    void SwapModel(const AltModel *m);
};

class AmbientSoundManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // AmbientSoundManager_Init (override)
    virtual void Update();                         // AmbientSoundManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // AmbientSoundManager_HandleMessage (override)
    void PickRepeatDelay();
    u16 soundId;                                   // SOUNDID property, passed to Sound_Play
    u16 volume;                                    // VOLUME property (0..255): the maximum volume, scaled by distance or box falloff
    u16 soundHandle;                               // current Sound channel handle, 0 = none; cleared when the channel is found stopped or on leaving rang
    u16 loopPollCounter;                           // looping mode only: incremented every in-range update; the IsPlaying / replay check runs only when it
    s32 repeatTimerMs : 24;                        // (bits) signed 24-bit bitfield (low 24 bits, top byte preserved): ms until the next one-shot play. -= g_dtMs
    s32 repeatTimerTop : 8;                        // (bits) top byte of the repeat-timer unit, preserved by every store
    u8 playFlags : 7;                              // (bits) 7-bit bitfield of SoundPlayFlags passed as Sound_Play's 4th argument: 8 always; +2 POSITIONAL (FLAGS
    u8 playFlagsTop : 1;                           // (bits) top bit of the play-flags byte, preserved
    s32 linearFalloff : 1;                         // (bits) bit0 = FLAGS bit4: volume falls linearly with camera Manhattan-XZ distance over MAXDIST | signed bit
    s32 linearFalloffRest : 31;                    // (bits) remaining bits
    Box *soundBox;                                 // SOUNDBOX (Scn_GetPropBox prop 0x10): when set, the sound plays only while g_camPos is inside it and
    Box *maxVolBox;                                // SOUNDMAXVOLBOX (prop 0x18): full volume inside; linear per-axis falloff out to the soundBox edges. I
};

struct AnimHeader {
public:
    u8 _pad000[0x8];
    u16 keyCount;                                  // number of key records (the frame-index modulus in Anim_Start/Advance/StartDirect); the records are k
    u8 keys[0x4];                                  // (AnimKey[]) variable-length key records, next = key + 8 + 2*payloadWords
};

struct AnimJointPose {
public:
    float rot[3];                                  // joint Euler angles in radians [0, 2PI), used by Mat44_SetRotYXZ in Instance_DrawAnimParts
    float pos[3];                                  // joint translation in model units, added to the hierarchy offset
    float scale[3];                                // joint scale, 1.0 default; scales this joint (second pass) and its children's base offsets
    u16 channels;                                  // copy of the entry control word; & 0x7000 gates the per-part scale pass
    u16 pad26;                                     // never written by the decoders or the interpolator (only carried by memcpy); size 0x28
};

struct AnimKey {
public:
    u16 durationMs;                                // key duration in ms; the animator accumulator counts 1/1024 ms, so a key lasts durationMs << 10 accum
    u16 entryCount;                                // number of packed joint entries that follow
    u16 payloadWords;                              // payload size in u16 words, used only to find the next record (never reconciled with entryCount)
    u16 eventId;                                   // event/sound id raised into animator +0x14
    u8 entries[0x4];                               // (u16[]) packed entries: u16 control word (AnimKeyChannel) followed by its present channel values (s16/u16, o
};

class Mesh {
public:
#ifdef SDW_MEMBERS_Mesh
    SDW_MEMBERS_Mesh
#endif
    virtual ~Mesh();                                  // Mesh_ScalarDeletingDtor
    virtual u8 BuildFromBsFile(D3DApp *app, BsFile *file); // Mesh_BuildFromBsFile
    virtual void TransformRange(u32 firstVertex, u32 count); // Mesh_TransformRange
    virtual void TransformAll();                   // Mesh_TransformAll
    virtual void TransformRangeWithMatrices(Mat44 *world, Mat44 *view, Mat44 *proj, u32 firstVertex, u32 count); // Mesh_TransformRangeWithMatrices
    virtual void DrawAll(Mat44 *world, Mat44 *view, Mat44 *proj); // Mesh_DrawAll
    void DrawImmediate(PolyBatcher *batcher, Frustrum *cam);
    s32 CollectVisiblePolys(RenderPoly *outPolys, Frustrum *cam);
    s32 CollectVisibleVerts(void *outVerts, Frustrum *cam);
    void DrawImmediateTinted(u32 tintColor, float tintBlend, PolyBatcher *batcher, Frustrum *cam);
    u32 Color_Lerp(u32 source, u32 target, float f);
    void Mesh_DrawOutlined(PolyBatcher *, Frustrum *, float);
    void Mesh_DrawOutlinedTinted(u32 tintColor, float tintBlend, PolyBatcher *batcher, Frustrum *cam, float outlineWidth);
    u8 flags;                                      // flags byte; elastic_Init sets it to 3
    D3DApp *app;                                   // Mesh_BuildFromBsFile's FIRST argument; Mesh_TransformAll reads [[this+8]+0x28],
    SdwVertexBuffer *vbPositions;           // D3D7 vertex buffer for positions. Mesh_BuildFromBsFile allocates a scratch of vertexCount*0xc (three
    SdwVertexBuffer *vbTransformed;         // destination of SdwVertexBuffer::ProcessVertices (vtbl +0x14) in Mesh_TransformAll, t
    u32 vertexCount;                               // vertex count, taken from BsFile_PeekEntryCount - i.e. the u16 at geometry record +8. It siz
    RenderPoly *faces;                             // the face array (Mesh_Construct nulls it; Mesh_Destruct frees it when faces and faceCount ar
    u32 faceCount;                                 // number of faces in the array; elastic_Init stores 8 (dword store). Completes the 0x20-by
};

class AnimMesh : public Mesh {
public:
#ifdef SDW_MEMBERS_AnimMesh
    SDW_MEMBERS_AnimMesh
#endif
    virtual ~AnimMesh();                              // AnimMesh_ScalarDeletingDtor
    virtual u8 BuildFromBsFile(D3DApp *app, BsFile *file); // AnimMesh_BuildFromBsFile (override)
    virtual void TransformAll();                   // AnimMesh_TransformAll (override)
    virtual void DrawAll(Mat44 *world, Mat44 *view, Mat44 *proj); // AnimMesh_DrawAll (override)
    void SetSequence(s32 seq, u8 loop, u8 blend);
    void Stub_40bac3();
    void Stub_40bace();
    u8 GetFlag(char which);
    void UpdateAnim();
    void ApplyFrame(MeshAnimFrame *frame, float weight);
    void CapturePose(MeshAnimFrame *frame);
    void SetBlendTarget(MeshAnimFrame *frame);
    MeshPart *parts;                               // array of partCount hierarchy parts, 0x70 bytes each (new MeshPart[n] in AnimMesh_Load -0x40b
    u32 partCount;                                 // number of hierarchy parts, read from the resource by
    MeshAnimSeq *sequences;                        // array of seqCount 0x14-byte sequence records; +0x0c = frame array (0xc bytes per frame, the u16 at +
    u32 seqCount;                                  // number of animation sequences, read from the resource by
    u8 playing;                                    // 0 means the next update does the first-frame initialisation; cleared from the loop flag when a non-l
    Timer *animTimer;                              // a privately owned Timer (operator new 0x28) that drives the animation in wall-clock milliseconds, in
    u8 loop;                                       // 1 keeps the sequence repeating when its last frame is reached
    u8 blend;                                      // 1 cross-fades into a newly requested sequence using elapsedMs/frameDurationMs as the weight instead
    s32 currentSeq;                                // sequence being played; -1 at construction, compared against requestedSeq to detect a change
    s32 frameIndex;                                // index of the NEXT frame to apply (it is incremented immediately after each frame is applied)
    s32 requestedSeq;                              // sequence requested by AnimMesh_SetSequence, latched into currentSeq by the update
    float elapsedMs;                               // milliseconds accumulated in the current frame from the private Timer; frameDurationMs is subtracted
    float frameDurationMs;                         // duration of the current frame, computed as 2 * (u16 at frameRecord+4), so frame times are authored i
};

struct SpriteFrame {
public:
    u16 texPage;                                   // texture page (TexAtlas_GetPage) of the frame bitmap; +4 is the RenderPoly type
    u8 u;                                          // U origin
    u8 v;                                          // V origin
};

class AnimSprite {
public:
#ifdef SDW_MEMBERS_AnimSprite
    SDW_MEMBERS_AnimSprite
#endif
    s32 InitFromRes(u16 resId);                                  /* AnimSprite_InitFromRes */
    void Draw(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u8 frame, u32 flipMode);
    void DrawThunk9(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u8 frame, u32 unused, u32 flipMode);
    void DrawThunk8(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u8 frame, u32 flipMode);
    void *texture;                                 // the first frame's id-list entry; non-NULL once AnimSprite_InitFromRes found the bitmap group (0x53b5
    SpriteFrame frames[10];
    u16 frameCount;                                // already known; AnimSprite_InitFromRes hard-clamps it to 10 frames
    u8 _pad02e[0x2];
    s16 width;
    s16 height;
};

struct Animator {
public:
    AnimHeader *cur;                               // current animation (AnimHeader: u16 keyCount at +8) (ScnBody +0x40)
    void *bufA;                                    // AnimJointPose[jointCount]: pose of the key being left ('from'); interpolation input A; swapped with
    void *bufB;                                    // AnimJointPose[jointCount]: pose of the key being approached ('to'); always the destination of Anim_D
    void *bufC;                                    // AnimJointPose[jointCount]: displayed pose written by Anim_InterpolatePose, read by Instance_DrawAnim
    u32 timeAcc;                                   // time accumulator (g_animDt*speed>>12; frame period = frameDuration<<10) (ScnBody +0x50)
    u32 pendingSoundId;                            // key event id: set from key 0 unconditionally (can clear it), from later keys only when non-zero; con
    u16 nbJoints;                                  // model joint count (ScnBody +0x58)
    u16 animId;                                    // current animation id (ScnBody +0x5a)
    u16 frame;                                     // current key-frame index (ScnBody +0x5c)
    u16 frameDuration;                             // duration of current key (x1024 time units) (ScnBody +0x5e)
    u16 speed;                                     // playback speed 4.12 (0x1000 = 1.0) (ScnBody +0x60)
    u16 flags;                                     // AnimFlags: 1 playing, 2 looping, 4 ping-pong/alt wrap, 8 finished (ScnBody +0x62)
};

struct AnimatedWorldObj {
public:
    Instance inst;                                 // the world object's instance: WorldObj's InstanceBase header, then pos/rot/record/attach fields; Worl
    Animator anim;                                 // the animation state (Animator_Init, WorldObj_DrawAnimated); its three pose buffers are the nbJoints
};

class ScnBody : public ScnObject {
public:
#ifdef SDW_MEMBERS_ScnBody
    SDW_MEMBERS_ScnBody
#endif
#ifdef SDW_EXTRA_ScnBody
    SDW_EXTRA_ScnBody
#endif
    virtual void PostLoadInit() {}                 // ScnObject_Nop (override)
    virtual void Update();                         // ScnBody_Update_default (override)
    virtual void Render(Camera *view);             // ScnBody_Render (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // ScnObject_CustomCollide_default (override)
    virtual void RenderScaled(Camera *view, Vec3s *scale); // ScnBody_RenderScaled (override)
    virtual ScnBody* Init(void *warRecord, u32 nbJoints); // ScnBody_Init
    s32 AnimFlags(u16 mask);
    u16 AnimId();
    u16 CurrentAnim();
    u16 GetAnimId();
    void PlayAnim(u16 id, s32 loop, s32 blend);
    void AdvanceAnim();/* inline: start animation id with option bits 1 loop / 2 blend (as in src/game/wolf.h); the */
    void AltModel_InitBody(AltModel *out, u16 idListId);
    void AltModel_SaveBody(AltModel *out);
    void SwapModel(AltModel *m);
    void ShareAnimTable(const u16 *resIdx);
    void RenderEx(Camera *view, void *buffer, s32 unused, s32 projDist, s16 *screenXY);
    void RenderTinted(Camera *view, u32 color, u16 amount, u16 mirrorPlane);
    void SetJointOverride(s32 joint, Vec3s *rotOrNull, Vec3s *transOrNull, Vec3s *scaleOrNull);
    Animator anim;                                 // the body's animation state, an Animator (+0x40..+0x64: cur, the three pose buffers, timeAcc, pending
};

class Anvil : public ScnBody {
public:
    virtual void PostLoadInit();                   // Anvil_PostLoadInit (override)
    virtual void Update();                         // Anvil_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Anvil_HandleMessage (override)
    u8 state;                                      // 0 dropping (anim 0 until it ends), 2 holding (anim 2, 2000 ms then the scripted camera is released),
    u32 holdTimeMs;                                // ms spent in state 2, += g_dtMs; compared unsigned with 2000 ( cmp/jbe);
    u32 dropping;                                  // 1 from msg 0x1280 until the hold ends; state 0 waits for it and the anim end be
};

struct AttachLinkBits {
public:
    u8 matrixValid : 1;                            // (bits) bit 0: the parent published this link's matrix during its Render (AttachLinkFlags 1) | AttachLink.fl
    u8 parentDone : 1;                             // (bits) bit 1: the parent has been through Scn_RenderIfVisible this frame (set on every child link at 0x5601
    u8 rootRotation : 1;                           // (bits) bit 2: AttachLink_Alloc's rootRotation argument; Instance_CalcWorldMatrix then keeps the child's own
    u8 hasLocal : 1;                               // (bits) bit 3: localOffset is applied (in the parent part's frame, *8)
    u8 hasRot : 1;                                 // (bits) bit 4: rot is applied (Mat44_SetRotXYZ)
    u8 hasWorld : 1;                               // (bits) bit 5: worldOffset is added to the child's translation (*8)
    u8 unused : 2;                                 // (bits) bits 6..7
};

class Mat44 {
public:
#ifdef SDW_MEMBERS_Mat44
    SDW_MEMBERS_Mat44
#endif
    Mat44 &Copy(const Mat44 &src);
    Mat44 &ScaleInPlace(float s);
    const float &AtConst(int row, int col) const;
    float &At(int row, int col);
    void SetLookAt(const Vec3f &vFrom, const Vec3f &vAt, const Vec3f &vWorldUp);
    void SetRotAxisAngle(const Vec3f &axis, float angle);
    void SetRotAxisAngleDiv(float x, float y, float z, float angle);
    void SetRotXYZ(float x, float y, float z);
    void SetRotYZX(float x, float y, float z);
    void SetTranslationV(const Vec3f &v);
    void TransposeInPlace();
    void Zero();
    void SetIdentity();
    void SetRotZYX(float x, float y, float z);                   /* Mat44_SetRotZYX */
    void SetRotZXY(float x, float y, float z);
    void SetRotXZY(float x, float y, float z);                   /* Mat44_SetRotXZY */
    void SetRotYXZ(float x, float y, float z);                   /* Mat44_SetRotYXZ */
    void SetTranslation(float x, float y, float z);
    void SetScale(float sx, float sy, float sz);
    void SetPerspective(float fov, float aspect, float zn, float zf);
    Mat44 &MulInPlace(const Mat44 &b);
    void Transpose(Mat44 *out);
    void Transpose3x3InPlace();
    void Transpose3x3(Mat44 *out);
    void NormalizeColumnsInPlace();
    void NormalizeColumns(Mat44 *out);
    void TransformPoint(const Vec3f *src, Vec3f *dst);
    float m[4][4];                                 // a D3DMATRIX: m[row][col], row-major (_11.._44); D3D row-vector convention v' = v*M, translation in m
};

struct AttachLink {
public:
    Vec3s localOffset;                             // offset in the parent part's frame, *8, applied when flags & 0x08 (Instance_CalcWorldMatrix)
    u8 partIndex;                                  // parent model part whose matrix M[partIndex] is published into the link
    union {
        AttachLinkBits flags;                          // AttachLinkFlags (1 matrix valid, 2 parent already run through Scn_RenderIfVisible this frame - set o
        u8 flagsByte;                              // flags as the plain byte: Scenaric_RenderAll clears matrixValid and parentDone together with one and-
    };
    Vec3s rot;                                     // extra rotation (4096/turn) applied with Mat44_SetRotXYZ when flags & 0x10
    u8 _pad00e[0x2];
    Vec3s worldOffset;                             // added to the child's translation *8 when flags & 0x20
    Instance *parentInst;                          // The parent's embedded render instance (parentObj + 4). Written by AttachLink_Alloc and zeroed by Att
    ScnObject *parentObj;                          // The parent scenaric object. Written by AttachLink_Alloc and zeroed by AttachLink_Free and AttachLink
    Mat44 matrix;                                  // parent-published D3D matrix for the attached child (translation row at +0x50/+0x54/+0x58). A real Ma
};

class ZoneList {
public:
#ifdef SDW_MEMBERS_ZoneList
    SDW_MEMBERS_ZoneList
#endif
#ifdef SDW_EXTRA_ZoneList
    SDW_EXTRA_ZoneList
#endif
    Box *Contains(Vec3s *point);
    Box *ContainsXZ(Vec3s *point);
    Box *FindContaining(Vec3s *p);
    Box *FindContainingXZ(Vec3s *p);
    void Clear();
    void Resolve(u32 id);
    Box **boxes;                                   // zone boxes of one type; seven lists stride 8, filled by Zones_ResolveGlobalLists 0x50d79
    u16 count;                                     // number of boxes
};

class AutomaticDoor : public ScnBody {
public:
    virtual void PostLoadInit();                   // AutomaticDoor_PostLoadInit (override)
    virtual void Update();                         // AutomaticDoor_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // AutomaticDoor_HandleMessage (override)
    virtual void Reset();                          // AutomaticDoor_Reset (override)
    void SetCollision(s32);
    void SetState(s8);
    ZoneList sensibleBoxes;                        // zone list {boxes, count}: Trigger boxes resolved from the SENSIBLEBOXES property (designer property
    s8 state;                                      // 0 closed (anim 2, boxes solid), 1 open (anim 4), 2 opening (anim 3, collision switched OFF at the fi
};

struct BallBits {
public:
    u8 movable : 1;
    u8 magnet : 1;                                 // (bits) bit 1 (1-byte unsigned unit)
    u8 sound : 1;                                  // (bits) bit 2 (1-byte unsigned unit)
};

class Bat : public ScnBody {
public:
    virtual void PostLoadInit();                   // Bat_PostLoadInit (override)
    virtual void Update();                         // Bat_Update (override)
    virtual void Render(Camera *view);             // Bat_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Bat_HandleMessage (override)
    void SetState(u8);
    u8 state;                                      // 0 roosting (not drawn), 1 flying at Ralph, 2 circling him, 3 flying home. Written only by Bat_SetSta
    u8 takeoffFrames;                              // Consecutive Update frames on which Ralph was within 800 units AND the was-drawn latch at +0x78 was c
    s16 spinSpeed;                                 // Signed angular speed (4096 per turn, per second) while circling; drives both the orbit angle +0x90 a
    u16 unk68;                                     // Written 0 by Bat_SetState case 2 and never read. An exhaustive scan of every [reg + 0xNN]
    s16 hoverHeight;                               // How far above Ralph the bat aims (the vertical axis points down, so it is subtracted). Rand_Range(0x
    u8 _pad06c[0x4];
    s32 approachRadius;                            // Horizontal distance at which the approach turns into circling; Rand_Range(0x5a,0x96) = 90..150 when
    s32 circleTimeMs;                              // Milliseconds left to circle Ralph, counted down by g_dtMs; at < 0 the bat flies home. Rand_Range(0x1
    s32 wasDrawnLastRender;                        // Bat_Render latches inst.flags bit 0 here and Update clears it at the end of every frame. inst.flags
    Vec3s targetPos;                               // The point the bat aims at: Ralph's position copied each frame (6-byte copy from g_pWolf+0xc) with th
    Vec3s toTarget;                                // targetPos - pos, computed while approaching; no
    Vec3s homePos;                                 // Where the bat hangs: its designer-placed position captured at load, restored by Bat_SetState(0) thro
    Vec3s orbitRot;                                // Euler angles fed to Mat34s_FromEulerScaled to spin orbitOffset around Ralph; only the middl
    Vec3s orbitOffset;                             // Vector from Ralph to the bat at the moment circling started (pos copied in, then the target words su
    u16 soundHandle;                               // Handle of the looping wing-flap voice (sample 0xfe). SetState stops it before every transition and s
};

class UiQuad {
public:
#ifdef SDW_MEMBERS_UiQuad
    SDW_MEMBERS_UiQuad
#endif
    void SetColor(u32 rgb);
    void SetFadeLevel(u8 level);
    void UiQuad_Mirror(s32 flipX, s32 flipY);
    void UiQuad_SetFromBitmap(const u16 *bitmap, s32 x, s32 y, s32 boxW, s32 boxH, s32 scaleX, s32 scaleY);
    void UiQuad_Draw(u16 layer);/* inline: the fade level in drawFlags bits 6-8. */
    u32 color;                                     // 24-bit RGB tint; passed through Color_RgbToBgr (which also zeroes the alpha byte) into all four vert
    u8 u;                                          // U origin of the sub-rect within the 256-pixel texture page
    u8 v;                                          // V origin of the sub-rect
    u8 wMinus1;                                    // sub-rect width minus 1 in texels; used as both the offset and the size argument of Tex_CornerUV on t
    u8 hMinus1;                                    // sub-rect height minus 1 in texels
    u16 drawFlags;                                 // flag word whose bits 6..8 (mask 0x01C0) hold a 0..7 fade/alpha level set by UiIcon_SetFadeLevel; not
    u16 texIndex;                                  // texture/poly index passed as Draw2D_TexRect's texIndex argument (the same space as RenderPoly.type m
    s16 x;                                         // left edge in the 512x240 virtual HUD space, fed to Screen_ScaleX
    s16 y;                                         // top edge in virtual HUD space, fed to Screen_ScaleY
    s16 w;                                         // width in virtual HUD units; x1 = x + w. Made negative by UiQuad_Mirror to flip horizontally
    s16 h;                                         // height in virtual HUD units; y1 = y + h. Made negative by UiQuad_Mirror to flip vertically
};

class Battery : public ScnBody {
public:
    virtual void PostLoadInit();                   // Battery_PostLoadInit (override)
    virtual void Update();                         // Battery_Update (override)
    virtual void Render(Camera *view);             // Battery_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Battery_HandleMessage (override)
    virtual void Reset();                          // Battery_Reset (override)
    s32 StepCollectFlight();
    void SetState(u8);
    Vec3s flyStart;                                // collect-flight start: SetState(3) copies pos componentwise ( /37/45)
    u8 _pad06a[0x2];
    Vec3s hudTarget;                               // collect-flight target on the HUD, stored x/y/z by SetState(3) ( /87/8e)
    u8 _pad072[0x2];
    Vec3s flyPosition;                             // lerp output of the collect flight, passed to SetPosition (address taken)
    u8 _pad07a[0x2];
    u16 flyT;
    u8 state;                                      // SetState stores its argument
    u8 hudState;                                   // Update stores 5 on area entry and 6 on departure
    s16 scale;                                     // SetState(3) stores 0x1000; Update compares signed and shrinks it; Render copies it three
    s16 hudOffsetY;                                // PostLoadInit stores 30; Update compares signed and adds/subtracts 4
    uptr *hudFullBitmap;                            // u32*: IdList_FindWithCount(0x9b); Update double-dereferences it before UiQuad_SetFromBitmap, the sam
    uptr *hudEmptyBitmap;                           // u32*: IdList_FindWithCount(0x9c), the same double dereference
    u32 chargeDuration;                            // from the Ghost message 0x4005 at init; Update divides it by five
    UiQuad hudSlots[5];                            // UiQuad[5]: UiQuad_SetFromBitmap((UiQuad*)(this+0x90+i*0x14)) for i<5; 0x90+5*0x14 = 0xf4 = master, s
    Battery *master;                               // MASTER property, or this when null; messages 0x4401/0x4403/0x4405 go to it
    ScnObject *hoover;                             // found by class 0x83 at init; HandleMessage sends it 0x4581/0x4582
    ScnObject *ghost;                              // found by class 0x78 at init; sent 0x4005
    ScnObject *collected[5];                       // message 0x4401 stores the sender at +0x100 + count*4
    Box *triggerBox;                               // Box property at init; Update tests its XZ bounds
    Vec3s homePos;                                 // saved at init; Reset and SetState(2) pass its address to SetPosition
    Vec3s renderRot;                               // copied to rot for rendering; StepCollectFlight updates the first component signed
    s32 isMaster;                                  // 1 when the MASTER property is null, else 0
    s32 reported;                                  // SetState(1) sends the collection notification once; cleared by reset/init
    s32 wolfInBox;                                 // latched XZ containment of the trigger box
    s32 charged;                                   // set by HandleMessage on the fifth battery
    s32 draining;                                  // argument of message 0x4404; gates the drain timer and collection
    s32 unknown138;                                // Update writes 1 when not draining; role not established
    u16 collectedCount;
    s16 flySpin;                                   // SetState stores a derived angle; read signed by StepCollectFlight
    s32 drainTimer;                                // Update subtracts g_dtMs and compares signed with 0
    u16 soundHandle;                               // SetState stores the Sound_Play handle; cleared by init/reset
};

class Bees : public ScnBody {
public:
    virtual void PostLoadInit();                   // Bees_PostLoadInit (override)
    virtual void Update();                         // Bees_Update (override)
    virtual void Render(Camera *view);             // Bees_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Bees_HandleMessage (override)
    virtual void Reset();                          // Bees_Reset (override)
    void SetState(u8 newState);
    s16 Orbit(Vec3s offset, Vec3s centre, s16 angle, s16 step);
    s32 MoveToward(Vec3s target, s32 speed, s32 hover);
    void FaceTowards(Vec3s target);
    CollBox *flyBox;                               // PROPERTY BOXFLY (Scn_GetPropBox(record, 4)): the area the honey pot must stay in; once the
    CollBox *detectWolfBox;                        // PROPERTY BOXDETECTWOLF (Scn_GetPropBox(record, 0)): Ralph inside it (3-D test) st
    ScnObject *honeyPot;                           // the level's HoneyPot (Scenaric_FindByClass(108)); its SCN_OF_IN_WORLD bit (spilled, not car
    ScnObject *victim;                             // msg 0x67's argument (from HoneyPot_Update while Sam stands in the spill); refused in state
    ScnObject *sam;                                // the level's Sam (Scenaric_FindByClass(1)): the default chase target of state 0xF
    ScnObject *hitHive;                            // the hive that reported the cannonball (msg 0x3981's sender); gets msg 0x3e02 when the swar
    ScnObject *firstHive;                          // PROPERTY FIRSTHIVE (Scn_GetPropObject(record, 8)): where the swarm starts (SetPosition at 0
    ScnObject *secondHive;                         // PROPERTY SECONDHIVE (Scn_GetPropObject(record, 0x10)): where a cannonball on the first hive
    ScnObject *motherHive;                         // PROPERTY MOTHERHIVE (Scn_GetPropObject(record, 0xc)): the home the broadcast 0x3982 sends e
    ScnObject *hive;                               // the hive currently circled; firstHive at load, secondHive at the end of state 3 (
    u8 state;                                      // BeesState, 0..0x13; written at the top of SetState and dispatched by Update
    u8 alert;                                      // msg 0x3980's argument: indexes chaseTime and picks the taunt animation in state 0x13; 3 a
    s32 timer;                                     // ms countdown, decremented by g_dtMs in states 2, 5, 0xb, 0xc, 0x12 and 0x13; loaded from chaseTime,
    s32 chaseTime[4];                              // how long the swarm chases, in ms, by alert level: 450, 300, 150, 0; read as chas
    s32 settleTimer;                               // 150 ms at load, counted down only in state 0xE; at 0 it sets g_beesLeftFirstHive (0x49789
    u32 animDuration;                              // Anim_GetDurationMs(inst, 1, 1) at load: the length of the taunt animation, used as the st
    s32 potLeftFlyBox;                             // latched to 1 the first frame the HoneyPot is outside flyBox on x/z; never cleared, so it
    s32 stungHome;                                 // set to 1 on entering state 7; blocks any further Ralph detection
    s32 wolfDist;                                  // distance to Ralph from the last MoveToward of state 0x10; under 0x14 it switches to the s
    s32 targetDist;                                // distance to the chase target from the last MoveToward of state 0xF; under 0x28 it starts
    u8 _pad0bc[0x4];
    s32 camAngle;                                  // atan2 angle (4096/turn) from the camera to the circled hive, taken on entering state 0; t
    s32 hiveAngle;                                 // the same angle measured from the swarm each frame of state 0
    s16 orbitAngle;                                // the circling angle, advanced by Orbit's step every frame (-0x19, -0x32 or -0x64 depending on the sta
    Vec3s renderRot;                               // extra rotation handed to ScnObject_RenderFacingCamera; x = 0x400 at load, the other two 0
    Vec3s targetPos;                               // the point states 0xD, 0xE and 0x10 fly to: the circled hive's position on entering them,
    u8 _pad0d6[0x12];
    Vec3s orbitOffset;                             // the radius vector of the circle: pos - orbitAnchor, so 0x50 (or 0x64 in state 0xB) on x and 0 on the
    Vec3s orbitAnchor;                             // the swarm's position at the moment the circling starts, moved 0x50 (0x64) back on x; only used to de
    u16 soundHandle;                               // handle of the buzz 0x141; volume 0x3f while circling (state 9) and 0xff while chasing (0xF, 0x12)
};

class Bell : public ScnBody {
public:
    virtual void PostLoadInit();                   // Bell_PostLoadInit (override)
    virtual void Update();                         // Bell_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Bell_HandleMessage (override)
    virtual void Reset();                          // Bell_Reset (override)
    void SetState(u8);
    s32 ringTimer;                                 // Milliseconds left of the ring, loaded with 0x1000 (4096 ms) when the bell starts and drained by g_dt
    u16 voice;                                     // Handle of the ringing sound (id 0x14f); re-played whenever it is 0 or Sound_IsPlaying reports stoppe
    u8 state;                                      // 1 silent/idle (anim 1, one shot), 2 ringing (anim 0 looping + sound 0x14f + ringTimer), 3 wind-down
};

class TrajFollower {
public:
#ifdef SDW_MEMBERS_TrajFollower
    SDW_MEMBERS_TrajFollower
#endif
    Trajectory *traj;                              // the trajectory it follows ({u16 count; Vec3s pts[count]}): TrajFollower_Init stores it, Tra
    s16 pointIndex;                                // index of the waypoint currently being steered toward; wraps to 0 at the end
    s16 speed;                                     // movement speed in units/s; multiplies the normalised direction to give the output velocity
    s16 headingBias;                               // constant added to the atan2 heading; every call site passes 0x800 (half a turn)
    u16 heading;                                   // cached 12-bit facing toward the current waypoint; reused unchanged when neither advanced nor continu
    s32 arriveRadiusSq;                            // squared arrival radius; the waypoint is consumed while the squared distance is below it
    u32 continuousHeading;                         // when non-zero the heading is recomputed every step; when zero it is only refreshed on waypoint chang
    u32 use3dDistance;                             // when non-zero the vertical delta is included in the arrival distance and in the output velocity
    u32 moving;                                    // set to 1 by every step, cleared by init; a 'follower is running' latch
    u32 advanced;                                  // 1 when at least one waypoint was consumed during this step (used to trigger the heading recompute)
};

class BipbipLevel14 : public ScnBody {
public:
    virtual void PostLoadInit();                   // BipbipLevel14_PostLoadInit (override)
    virtual void Update();                         // BipbipLevel14_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // BipbipLevel14_HandleMessage (override)
    virtual void Reset();                          // BipbipLevel14_Reset (override)
    void SetState(u8 newState);
    void TrajInit(TrajFollower *f, Trajectory *traj, s16 speed, s16 bias, s32 continuousHeading, s32 use3d, s16 arriveRadius); /* this file's copy of */
    s32 TrajStep(TrajFollower *f, Vec3s *outVel, s16 *outHeading, s32 forward);
    u8 state;                                      // BipbipState 0-7; written at the end of SetState, read by Update's
    u8 stateAfterWait;                             // state SetState(4) waits for: written 3 / 7 as a byte, replayed by state 4 (mov
    u8 unk066[6];                                  // never read or written by the class
    s32 waitLeftMs;                                // countdown of state 4: -= g_dtMs each frame and SetState(stateAfterWait) at <= 0
    s32 waitTimeMs;                                // TIMEWAIT property (dword at record+0x14+12); 10000 in Lvl-16. Loaded whole (0x42e
    ScnObject *trainStation;                       // TRAINSTATION property (Scn_GetPropObject prop 16); asked msg 0x4803 in states 2 a
    ScnObject *button;                             // nearest SensibleButton (class 24) within 200 units, found when the outward path ends ( -0x42e
    ScnObject *gate;                               // nearest FallingGate (class 26) within 1200 units, refreshed twice in state 2 an
    s32 chased;                                    // set 1 by state 5, cleared by PostLoadInit / Reset / the gate branch / the path end; gates
    s32 gateShut;                                  // 1 once a FallingGate answered msg 0x72; cleared and. State
    s32 unk088;                                    // zeroed by PostLoadInit and Reset; never read
    Vec3s startPos;                                // first point of GOTOTRAJECTORY snapped to the ground; Reset and the end of the re
    u8 pad092[2];                                  // alignment before the box pointer
    Box *stopBox;                                  // BOXSTOPBIPBIP property (Scn_GetPropBox prop 0); state 2 tests pos against it in x
    Trajectory *gotoTraj;                          // GOTOTRAJECTORY property (Scn_GetPropTrajectory prop 4); walked forward in state 2
    TrajFollower gotoFollower;                     // follower over gotoTraj: speed 0x514, bias 0x800, continuous heading, 2-D, arrive radius 0x32 (0x42db
    Trajectory *returnTraj;                        // RETURNTRAJECTORY property (Scn_GetPropTrajectory prop 8); walked by state 7
    TrajFollower returnFollower;                   // follower over returnTraj, same settings, stepped by the shared TrajFollower_Step
    u16 soundHandle;                               // Sound_Play(0x136) channel of the running loop; stopped through Sound_IsPlaying/
    u8 pade2[2];                                   // tail padding to sizeof 0xe4
};

class Bird : public ScnBody {
public:
    virtual void PostLoadInit();                   // Bird_Init (override)
    virtual void Update();                         // Bird_Update (override)
    virtual void Render(Camera *view);             // Bird_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Bird_HandleMessage (override)
    virtual void Reset();                          // Bird_Reset (override)
    void SetState(u8);
    u8 state;                                      // BirdState (0 appear-wait, 1 circle, 2 takeoff, 3 descend, 4 leave)
    u8 offscreenTicks;                             // updates counted in state 0 while wasOnScreen == 0; at >= 2 the bird starts circling; zeroed by SetSt
    u8 boxIndex;                                   // index into appearBoxes of the box the bird appeared in (loop variable in state 4); every state entry
    u8 _pad067[0x5];
    s16 orbitDir;                                  // +1/-1 circling direction; negated when the orbit is mirrored (reverseNext)
    s16 orbitCentreVertCopy;                       // written = orbitCentre.y in SetState(0); never read
    s16 orbitAngSpeed;                             // orbitDir*0x226 (550/4096 turn per second); applied to orbitRot.y and to facing
    u32 reverseNext;                               // Rand_Bounded(2) result; when set, the next SetState(1) mirrors the orbit centre through the bird and
    u32 wasOnScreen;                               // Render copies inst.flags & INST_F_DRAWN here; Update zeroes it every frame
    Vec3s orbitRadius;                             // pos - orbitCentre at the start of a quarter circle; rotated by orbitRot each frame
    Vec3s takeoffPos;                              // pos when leaving the circle for state 2; the orbit centre is shifted by pos - takeoffPos on landing
    Vec3s orbitRot;                                // Euler angle of the orbit (only y is advanced); zeroed per quarter circle
    Vec3s appearPos;                               // centre of the APPEARBOX box Ralph was found in; the bird is teleported here
    Vec3s orbitCentre;                             // circle centre; appearPos + (400,0,0) on appearing
    Vec3s flightStep;                              // per-frame translation for states 2/3 = Vec3s_ScaleByDt(vel), computed once at state entry
    ZoneList appearBoxes;                          // zone list {boxes, count}: APPEARBOX id-list (Scn_FindIdList); export boxes {u32 flags; Vec3s min; Ve
    s16 flightTimerMs;                             // 1000 (takeoff) / 1500 (descent), minus g_dtMs per frame
};

struct BlackHoleFlagBits {
public:
    u8 registered : 1;                             // (bits) bit view of BlackHole.flags (+0x65): the hole is in the Wolf's move-modifier list. Read as a one-bit
};

class BlackHole : public ScnBody {
public:
    virtual void PostLoadInit();                   // BlackHole_PostLoadInit (override)
    virtual void Update();                         // BlackHole_Update (override)
    virtual void Render(Camera *view);             // BlackHole_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // BlackHole_HandleMessage (override)
    virtual void Reset();                          // BlackHole_Reset (override)
    void ApplyPull(MoveModifyArg *, ScnObject *);
    u8 state;                                      // 0 = waiting/pulling, 1 = Ralph has been swallowed. Nothing in state 1 returns to 0, so it is termina
    BlackHoleFlagBits flags;                       // Bit 0 = this hole is registered in the Wolf's rider/move-modifier list (MSG_RIDER_ADD 0x18 returned
};

struct BonusEntry {
public:
    s8 kind;
    u8 price;                                      // price in points, read unsigned
    char payload[10];                              // the gallery name (PERSO, DECOR, STORY, COLOR, TEAM, INTRO) of a kind 0 entry, or in payload[0] the t
};

class ScrollText {
public:
    void Init(char *text, const s16 *rect);                      /* ScrollText_Init */
    void ScrollList_Update(s8 dir);
    void Draw(u8 layerIndex, u8 align);                          /* ScrollText_Draw */
    s16 rect[4];                                   // x, y, w, h of the scroll window, copied wholesale from the caller and replayed through Text_SetWindo
    char *text;                                    // the string being scrolled (overlay char * - the generator emits const char * as void *; ScrollText_D
    u16 topLine;                                   // index of the first visible wrapped line
    u16 maxTopLine;                                // max(0, totalWrappedLines - font->rows); the scroll clamp
    u16 charOffset;
    s8 scrollDir;                                  // pixels per frame of the smooth scroll in flight (negative = up, positive = down); 0 when idle
    s8 scrollPixels;                               // accumulated sub-line pixel offset; the line commits when it reaches font->lineHeight or wraps to 0.
};

class BonusManager : public ScnBody {
public:
    virtual void PostLoadInit();                   // BonusManager_PostLoadInit (override)
    virtual void Update();                         // BonusManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // BonusManager_HandleMessage (override)
    virtual void Reset();                          // BonusManager_Reset (override)
    char *GetUiString(u8);
    void DrawBonusList();
    void DrawEntryInfo();
    void DrawViewerFooter();
    CamSetup *camera;                              // CAMERA property from Scn_GetPropCamera in PostLoadInit; Update reads it for Camera_StartS
    u8 _pad068[0x8];
    UiQuad iconQuads[2];
    ScrollText scrollText;                         // its address goes to ScrollText_Init / Draw / Update
    u8 _pad0ac[0x8];
    s32 viewerNeedsDraw;                           // set to 1 after the picture loads, tested and cleared
    s32 unknownB8;                                 // Update stores 1 on picture load; no purpose established
    s16 points;
    u8 promptState;
    u8 screen;                                     // 0 closed, 1 opening, 2 browse, 3 confirm, 4 purchase completed, 5 not enough points ( -0x49c4
    u8 backdropFade;                               // Reset stores 7; Update counts it down and passes it to Ui_DrawMemCardBackdrop
    u8 viewerDelay;
    u8 firstVisible;
    s8 cursor;
    s8 boughtSomething;
};

struct Box6i {
public:
    s32 min[3];                                    // collision-space box min x,y,z (coords x16)
    s32 max[3];                                    // collision-space box max x,y,z
};

struct BoxCrate {
public:
    u8 _pad000[0x7c];
    ScnObject *contents;                           // The object this crate delivers. The crate never resolves it â€” Mailbox_Deliver writes it at
    u16 unk80;                                     // Zeroed at the moment the crate opens and referenced nowhere else in. Na
    u8 state;                                      // 0 idle/parked, 1 falling (driven down 800/s with a landing test), 2 opening (waiting for the open an
    s32 canRelease;                                // Gate on the release step: set by Init/Reset and by msg 0x30, cleared by msg 0x31. With it clear the
};

class BsFile {
public:
#ifdef SDW_MEMBERS_BsFile
    SDW_MEMBERS_BsFile
#endif
    virtual ~BsFile();                                // BsFile_ScalarDeletingDtor
    bool Bs_IsGeometryResource();
    int NextFrame();
    int SeekFrameOfType(u8 type);
    int FirstFrame();
    int FindFrameOfType(u8 type);
    u32 PeekEntryCount();
    int TotalVerticesAllFrames();
    u32 FindEntryValue(u16 id);
    int CountKindAcrossFrames(u16 kind);
    u32 Type4_GetField18();
    int TotalField18AllFrames();
    u32 Type4_GetTableCount();
    int TotalTableCountAllFrames();
    u32 ReadVertices(float *dest, u16 firstIndex);
    int BsDecode_Kind0_1(BsPolyFlat *dest, int firstIndex, int bias);
    int BsDecode_Kind2_3(BsPolyGouraud *dest, int firstIndex, int bias);
    int BsDecode_Kind10_11(BsPolyBlendFlat *dest, int firstIndex, int bias);
    int BsDecode_Kind12(BsPolyBlendGouraud *dest, int firstIndex, int bias);
    int BsDecode_Stride24_Pass1(BsPolyTexFlat *dest, int firstIndex, int bias);
    int BsDecode_Stride48_Pass1(BsPolyTexGouraud *dest, int firstIndex, int bias);
    int BsDecode_Stride24_Pass2(BsPolyTexFlat *dest, int count, int bias);
    int BsDecode_Stride48_Pass2(BsPolyTexGouraud *dest, int count, int bias);
    u32 Type4_ReadTable14(MeshPart *dest);
    int ReadAnimNames(MeshAnimSeq *dest, u32 count);
    u8 ReadU8(u8 advance);
    u16 ReadU16(u8 advance);
    u32 ReadU32(u8 advance);
    u32 ReadU24BE(u8 advance);
    u32 ReadRgbExpanded(u8 advance);
    void WriteU8(u8 value, u8 advance);
    void WriteU16(u16 value, u8 advance);
    void WriteU32(u32 value, u8 advance);
    float Bs_S16ToFloat(s16 v);
    float Bs_Angle12ToRadians(s16 a);
    float Bs_Angle8ToRadians(s8 a);
    float Bs_Fixed12ToFloat(u16 v);
    float Bs_Normal8ToFloat(u8 v);
    float Bs_TexelToUV(u8 texel, s16 size, s16 origin);
    void SkipEntry();
    void ReadTrack12(MeshAnimFrame *frames, u32 count, u32 nParts);
    void ReadJointTable(MeshPartPose *poses, u32 count);
    u8 ok;                                         // 1 once the file loaded and its Vdx7 companion opened (BsFile_Open copies vdx->ok); NextFram
    u32 frameCount;                                // number of frame-directory entries, the u32 at file offset 0xc (BsFile_Open); NextFrame wrap
    u8 frameType;                                  // type byte of the current frame: directory entry >> 24, with bit 0x40 masked off by Next/FirstFrame (
    u8 *data;                                      // base of the loaded "V2.6" Black Sheep model/anim file image
    s32 size;
    s32 cursor;                                    // read offset advanced by BsFile_ReadU16/U32 when their `advance` argument is 1
    u32 frameIndex;                                // index of the current frame-directory entry; NextFrame increments it, FirstFrame zeroes it
    Vdx7 *vdx;                                     // the companion .VDX7 reader opened from the same path with the extension replaced by DAV (BsFile_Open
};

class PolyTri {
public:
#ifdef SDW_MEMBERS_PolyTri
    SDW_MEMBERS_PolyTri
#endif
    virtual ~PolyTri();                               // PolyTri_ScalarDeletingDtor
    PolyTri & operator=(const PolyTri &src);
    u32 idx[3];                                    // the triangle's three vertex indices: HoleFX_BuildMesh writes them at +4/+8/+0xc through &poly (0x538
};

class BsPolyBlendFlat : public PolyTri {
public:
#ifdef SDW_MEMBERS_BsPolyBlendFlat
    SDW_MEMBERS_BsPolyBlendFlat
#endif
    virtual ~BsPolyBlendFlat();                       // BsPolyBlendFlat_VectorDeletingDtor
    u32 colour;                                    // flat colour from BsFile_ReadRgbExpanded (BsDecode_Kind10_11)
    u32 blendMode;                                 // a u32 read straight from the file (BsDecode_Kind10_11); RenderPoly_InitFromBsBlendFlat swit
};

class BsPolyBlendGouraud : public PolyTri {
public:
#ifdef SDW_MEMBERS_BsPolyBlendGouraud
    SDW_MEMBERS_BsPolyBlendGouraud
#endif
    virtual ~BsPolyBlendGouraud();                    // BsPolyBlendGouraud_VectorDeletingDtor
    u32 colour[3];                                 // per-vertex colours from BsFile_ReadRgbExpanded (BsDecode_Kind12)
    u32 blendMode;                                 // a u32 read straight from the file (BsDecode_Kind12); RenderPoly_InitFromBsBlendGouraud swit
};

class BsPolyFlat : public PolyTri {
public:
#ifdef SDW_MEMBERS_BsPolyFlat
    SDW_MEMBERS_BsPolyFlat
#endif
    virtual ~BsPolyFlat();                            // BsPolyFlat_VectorDeletingDtor
    u32 colour;                                    // flat colour from BsFile_ReadRgbExpanded, stored
};

class BsPolyGouraud : public PolyTri {
public:
#ifdef SDW_MEMBERS_BsPolyGouraud
    SDW_MEMBERS_BsPolyGouraud
#endif
    virtual ~BsPolyGouraud();                         // BsPolyGouraud_VectorDeletingDtor
    u32 colour[3];                                 // per-vertex colours from BsFile_ReadRgbExpanded: BsDecode_Kind2_3 stores them at +0x10/+0x14/+0x18 of a
};

class BsPolyTexFlat : public PolyTri {
public:
#ifdef SDW_MEMBERS_BsPolyTexFlat
    SDW_MEMBERS_BsPolyTexFlat
#endif
    virtual ~BsPolyTexFlat();                         // BsPolyTexFlat_VectorDeletingDtor
    u32 colour;                                    // flat colour from BsFile_ReadU24BE
    u32 texIndex;                                  // the Vdx7Rect page zero-extended
    float uv[6];                                   // u0 v0 u1 v1 u2 v2 from Bs_TexelToUV (fstp +0x18..+0x2c)
};

class BsPolyTexGouraud : public PolyTri {
public:
#ifdef SDW_MEMBERS_BsPolyTexGouraud
    SDW_MEMBERS_BsPolyTexGouraud
#endif
    virtual ~BsPolyTexGouraud();                      // BsPolyTexGouraud_VectorDeletingDtor
    u32 colour[3];                                 // per-vertex colours from BsFile_ReadU24BE
    u32 texIndex;                                  // the Vdx7Rect page zero-extended
    float uv[6];                                   // u0 v0 u1 v1 u2 v2 from Bs_TexelToUV
};

class BsStream {
public:
#ifdef SDW_MEMBERS_BsStream
    SDW_MEMBERS_BsStream
#endif
    virtual ~BsStream();                              // BsStream_ScalarDeletingDtor
    bool Seek(u32 pos);
    bool Skip(int delta);
    u8 ReadU8(u8 advance);
    u16 ReadU16(u8 advance);
    u32 ReadU32(u8 advance);
    u8 ok;                                         // 1 when Bs_LoadFile returned a length > 0 (BsStream_Open)
    u8 *data;                                      // file image from Bs_LoadFile (BsStream_Open); operator delete'd by BsStream_Dtor
    u32 size;                                      // image length; Seek/Skip compare unsigned against it (jae)
    u32 cursor;                                    // read offset (BsStream_ReadU8); zeroed by BsStream_Open
};

class Shadow {
public:
#ifdef SDW_MEMBERS_Shadow
    SDW_MEMBERS_Shadow
#endif
#ifdef SDW_EXTRA_Shadow
    SDW_EXTRA_Shadow
#endif
    void Invalidate();
    void Reproject();
    void SetEnabled(s32 on);
    void SetFlag4(s32 on);
    void SetVisible(s32 on);
    Vec3s groundPos;                               // ground position the blob is projected to
    u8 alpha;                                      // blob alpha
    u8 radius;                                     // blob radius (ScnMobile +0x6b = shadow +7, verified there)
    Vec3s corners[2];                              // the two half-diagonals of the projected ground quad (tangent+bitangent, tangent-bitangent); Shadow_U
    u8 flags;                                      // 1 = not drawn (Shadow_Render), 2 = re-project on the next Shadow_Update, 4 = set by SmallRo
    u8 pad15[3];                                   // tail padding to the 0x18-byte size used by ScnLogicShadowed +0x40 and ScnMobile +0x64
};

class ScnMobile : public ScnBody {
public:
#ifdef SDW_EXTRA_ScnMobile
    SDW_EXTRA_ScnMobile
#endif
    virtual void Render(Camera *view);             // ScnMobile_Render (override)
    virtual void RenderScaled(Camera *view, Vec3s *scale); // ScnMobile_RenderScaled (override)
    virtual void SetPosition(Vec3s *pos);          // ScnMobile_SetPosition (override)
    virtual ScnBody* Init(void *warRecord, u32 nbJoints); // ScnMobile_Init (override)
    s16 GroundY();
    void SetShadowRadius(u8 radius);
    void UpdateShadow();
    Shadow shadow;                                 // the object's ground shadow, a 0x18-byte Shadow (+0x64..+0x7c; Shadow_Init/Update/Render take &shadow
};

class Bullet : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Bullet_PostLoadInit (override)
    virtual void Update();                         // Bullet_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Bullet_HandleMessage (override)
    virtual void Reset();                          // Bullet_Reset (override)
    void AimAt(Vec3s *target);                                   /* Bullet_AimAt */
    void SetState(u8 newState);                                  /* Bullet_SetState */
    u8 state;                                      // BulletState: 1 homing flight, 2 hit (attached to Ralph, playing anim 4), 3 final anim 1 then gone, 5
    ScnObject *elmer;                              // Elmer (class 90), found by Scenaric_FindByClass(0x5a, &elmer, 1) in PostLoadInit; told msg
    ScnObject *swirlSign;                          // SwirlSign (class 89), found by Scenaric_FindByClass(0x59, &swirlSign, 1) in PostLoadInit; n
    s16 yawOffset;                                 // bearing of the bullet from Ralph relative to Ralph's own facing, computed when state 2 is entered (0
    s32 dist;                                      // XZ distance to Ralph measured every frame of state 1; divides the 1000 u/s flight velocit
};

struct BushFlagBits {
public:
    u8 fanBlown : 1;                               // (bits) bit 0 (1-byte unsigned unit)
    u8 liftLock : 1;                               // (bits) bit 1 (1-byte unsigned unit)
    u8 wolfNear : 1;                               // (bits) bit 2 (1-byte unsigned unit)
};

class Bush : public ScnBody {
public:
    virtual void PostLoadInit();                   // Bush_Init (override)
    virtual void Update();                         // Bush_Update (override)
    virtual void Render(Camera *view);             // Bush_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Bush_HandleMessage (override)
    virtual void Reset();                          // Bush_Reset (override)
    void StartRespawn();
    u32 stateTime;                                 // g_gameTime at the last state change (respawn timers, the 3 s sheep-attraction delay)
    s32 eatenTime;                                 // sum of g_dt over msg 1 from sheep in state 0; >= 0x14000 (20 s) = eaten -> respawn. Zeroed only by I
    Vec3s homePos;                                 // ground-snapped placed position (ScnObject_QueryGroundY at Init); respawn point
    u8 state;                                      // BushState (0 idle, 1 respawn hidden, 2 respawn wobble, 3 worn by Ralph)
    BushFlagBits bushFlags;                        // BushFlags: 1 fan-blown this frame, 2 lift lock (1 s), 4 Ralph within 50 last frame
    u32 liftLockMs;                                // ms since MSG_LIFT_CRUSH; bushFlags bit 1 is cleared above 1000
};

class Butterfly : public ScnBody {
public:
    virtual void PostLoadInit();                   // Butterfly_Init (override)
    virtual void Update();                         // Butterfly_Update (override)
    virtual void Render(Camera *view);             // Butterfly_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Butterfly_HandleMessage (override)
    void SetState(u8);
    u8 state;                                      // ButterflyState (0 flutter, 2 leave/hidden)
    u8 boxIndex;                                   // index of the appear box (loop variable in state 2)
    u8 appearDelay;                                // counts not-drawn updates with Ralph in a box while hidden; shown at >= 3; zeroed by SetState(0) and
    s16 orbitAngSpeed;                             // orbitDir*0x226
    s16 orbitDir;                                  // +1/-1
    s16 vertStep;                                  // this frame's vertical move = (sign(vertDelta)*80*g_dt)>>12
    s16 vertDelta;                                 // targetVert - pos.y
    s16 targetVert;                                // random flutter height, Rand_Range(appearPos.y-200, appearPos.y); re-picked within 20 units
    u32 reverseNext;                               // as Bird: mirror the orbit on the next SetState(0)
    u32 wasOnScreen;                               // inst.flags & INST_F_DRAWN copied in Render, zeroed in Update
    s16 variant;                                   // _4BUTTERFLY property (0/1/2): 0 -> model APAPIL02, anims 0 (flutter) / 1 (leave); 1 -> APAPIL03, ani
    Vec3s orbitRadius;                             // pos - orbitCentre at the start of a quarter circle
    Vec3s orbitRot;                                // orbit Euler angle (y advanced)
    Vec3s orbitCentre;                             // appearPos + (200,0,0) on appearing
    Vec3s appearPos;                               // centre of the appear box
    ZoneList appearBoxes;                          // zone list {boxes, count}: APPEARBOX id-list | count (+4): count from Scn_FindIdList
    AltModel mainModel;                            // main model snapshot from ScnObject_InitWithAltModels in Butterfly_Create
    AltModel altModels[2];                         // id lists {0xb8 WAR_IDO_APAPIL02, 0xb9 WAR_IDO_APAPIL03}; [0] used when variant == 0, [1] otherwise
};

class Cactus : public ScnBody {
public:
    virtual void PostLoadInit();                   // Cactus_Init (override)
    virtual void Update();                         // Cactus_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Cactus_HandleMessage (override)
};

struct CamFlagBits {
public:
    u8 manual : 1;                                 // (bits) CAMF_MANUAL
    u8 smooth : 1;                                 // (bits) CAMF_SMOOTH
    u8 snap : 1;                                   // (bits) CAMF_SNAP
    u8 tooFar : 1;                                 // (bits) CAMF_TOO_FAR
    u8 tooFarCut : 1;                              // (bits) CAMF_TOOFAR_CUT. g_camFlags is this bitfield byte: the Wolf stores it with byte read-modify
};

struct CamMgrEntry {
public:
    CamSetup *camera;                              // camera setup record {u16 focal; s16 rot[3]; Vec3s eye}
    Box *box;                                      // CAMERAnnBOX (Wolf pos tested on 3 axes, inclusive)
    u32 blendIn;                                   // CAMERAnnINTB; nonzero gives CAMSCR_BLEND_IN (1)
    u32 blendOut;                                  // CAMERAnnINTE; nonzero gives CAMSCR_BLEND_OUT (2)
};

struct CamModeParams {
public:
    s32 pitchSpeed;                                // Max pitch rate in angle units/s (mode 0: 300). Also scaled for manual pitch input and doubled (x2) f
    s32 yawSpeed;                                  // Max yaw rate in angle units/s (mode 0: 1500; mode 0xC: 3000). Manual rotation = input*g_dt*yawSpeed*
    s32 rollSpeed;                                 // max roll rate (mode 0: 800)
    s32 pitchRollAccel;                            // Accel and decel passed to Math_ApproachAngle for pitch and roll in Camera_SmoothToDesired (mode 0: 6
    s32 yawAccel;                                  // Yaw acceleration for Math_ApproachAngle (mode 0: 2048; mode 2 run: 32768)
    s32 yawDecel;                                  // Yaw deceleration, used when the required velocity is below the current one (mode 0: 6144; mode 2: 32
    s16 dist;                                      // Default orbit distance: 610, 0, 370, 510, 0, 400, 230, 0, 610, 610, 610, 400, 700 for modes 0..12
    s16 focal;                                     // Default projection focal for Screen_SetProjection: 384, 384, 230, 321, 321, 384, 284, 0, 384, 384, 3
};

struct CamPitchAdjBits {
public:
    s8 state : 3;                                  // (bits) g_camPitchAdjState low 3 bits, signed: 0 none, 1 raising the pitch over an obstacle, 2 lo
    s8 unused : 5;                                 // (bits) remaining bits
};

struct CamProbeFrame {
public:
    u8 value : 3;                                  // (bits) g_camProbeFrame low 3 bits, unsigned: frame counter during turn states 1/2, incremented &
    u8 unused : 5;                                 // (bits) remaining bits
};

struct CamRequestBits {
public:
    u8 ledgeProbe : 1;                             // (bits) CAMREQ_LEDGE_PROBE
    u8 snapYaw : 1;                                // (bits) CAMREQ_SNAP_YAW
    u8 usePitch : 1;                               // (bits) CAMREQ_USE_PITCH
    u8 useYaw : 1;                                 // (bits) CAMREQ_USE_YAW
    u8 padRotate : 1;                              // (bits) CAMREQ_PAD_ROTATE
    u8 restriction : 1;                            // (bits) CAMREQ_RESTRICT. g_camReqFlags is this bitfield byte (mov al,; or al,1 at 0x48f9e
};

struct CamRestrictBits {
public:
    u8 snapYaw : 1;                                // (bits) bit 0 CAMR_SNAPBETA: yaw snaps to multiples of 0x400 (same path as CAMREQ_SNAP_YAW) | CamRestrict.fl
    u8 forbidPad : 1;                              // (bits) bit 1 CAMR_FORBIDPADCONTROL: pad yaw input ignored (also implied by a trajectory)
    u8 mirrorYaw : 1;                              // (bits) bit 2 CAMR_MIRRORBETA (MIRRORBETAINTERVAL): yaw is also allowed in the arc shifted by 0x800
    u8 modeRun : 1;                                // (bits) bit 3 CAMR_MODE_RUN (MISCFLAGS bit 2): applies in camera mode 2
    u8 modeFollow : 1;                             // (bits) bit 4 CAMR_MODE_FOLLOW (MISCFLAGS bit 0): applies in modes 0 and 0xC; activates when the Wolf is ins
    u8 modeRobot : 1;                              // (bits) bit 5 CAMR_MODE_ROBOT (MISCFLAGS bit 1): applies in mode 6; activates when the Robot is inside an in
    u8 notifyWolf : 1;                             // (bits) bit 6 CAMR_NOTIFY_WOLF (MISCFLAGS bit 3): on Wolf activation sends Wolf msg 0x425
    u8 reserved : 1;                               // (bits) bit 7
};

struct CamRestrict {
public:
    s16 pitchMin;                                  // ALPHAMIN converted from degrees by deg*2048/180 ( at CameraRestriction init). If mi
    s16 yawMin;                                    // BETAMIN in 4096 units
    s16 rollMin;                                   // GAMMAMIN in 4096 units
    s16 distMin;                                   // DISTMIN. The lower clamp in Camera_ApplyRestriction. Camera_SolveCollision raises the TOO-FAR base d
    s16 pitchMax;                                  // ALPHAMAX in 4096 units. Camera_SolveCollision sets bit0 (overhead restriction, no blocked c
    s16 yawMax;                                    // BETAMAX in 4096 units
    s16 rollMax;                                   // GAMMAMAX in 4096 units
    s16 distMax;                                   // DISTMAX
    Vec3s aimOffset;                               // (AIMCORRECTIONX, -AIMCORRECTIONZ, AIMCORRECTIONY): the designer's Z-up axes mapped to x/vertical(dow
    s16 eyeVertMax;                                // -ALTITUDEMIN. Passed as maxVert to Camera_SolveCollision, which clamps eye.y to at most this value,
    Trajectory *trajectory;                        // First entry of the TRAJECTORY id-list, a Trajectory {u16 count; Vec3s pts[]}. NULL if absent or coun
    s32 pitchSpeed;                                // ALPHASPEED deg/s * 2048/180. Replaces speeds[0] when the pitch was clamped.
    s32 yawSpeed;                                  // BETASPEED in angle units per second. Replaces speeds[1] when the yaw is clamped, and in the yaw-snap
    s32 rollSpeed;                                 // GAMMASPEED in angle units per second
    s16 focal;                                     // FOCAL: when nonzero it overrides the desired projection focal (Screen_SetProjection distance)
    CamRestrictBits flags;                         // CamRestrictFlags as one-bit fields (CamRestrictBits): SNAPBETA, FORBIDPADCONTROL, MIRRORBETAINTERVAL
    ZoneList influenceBoxes;                       // zone list {boxes, count}: INFLUENCEBOXES id-list, from Scn_FindIdList. The restriction activates whi
};

struct CamSamPush {
public:
    s16 value : 10;                                // (bits) g_camSamPush low 10 bits, signed: Sam-visibility push counter (mode 0xC, +-5 per frame, h
};

struct CamSetup {
public:
    u16 focal;                                     // a CAMERA designer property as Scn_GetPropCamera returns it (the record other classes keep as u16 *):
    s16 rot[3];                                    // pitch, yaw, roll (4096 = full turn)
    Vec3s eye;                                     // eye position of the scripted camera (CrocodileLevel11_UpdateCamera reads it at +8)
    u16 pad;                                       // padding to the 16-byte record
};

class CamShot {
public:
    void Init(u32 flags);
    void Start(CamSetup *, ScnObject *, u32);
    void Stop(ScnObject *);
    void Update(ScnObject *);
    CamSetup *cam;                                 // camera setup {u16 focal; u16 rot[3]; Vec3s pos}; NULL = inactive
    s32 elapsedMs;                                 // g_dtMs accumulated; skip input accepted from 1000
    u32 param;                                     // passed to Camera_StartScripted (8th argument)
    s8 wolfFrozen;                                 // 1 if the Wolf accepted msg 0xE at start; msg 0xF sent on stop
    u32 noFreeze;                                  // nonzero: do not freeze the Wolf; survives CamShot_Init re-inits
};

struct Mat34s {
public:
    union {
        s16 rot[9];                                    // 3x3 rotation, 4.12
        Vec3s rows[3];                             // rot as its three rows (rows[2] is the view direction the weather code reads)
    };
    s32 trans[3];                                  // translation
};

class Camera {
public:
    Vec3s ViewDir();
    Mat34s viewMatS;                               // fixed-point 4.12 view rotation + translation, rebuilt from rot every frame
    u8 _pad020[0x20];
    Mat44 viewMat;                                 // D3D view matrix; the 4x4 rotation is built from rot and the translation row at +0x70 is set to -(R *
    Mat44 viewMatCopy;                             // The matrix every world-mesh and rigid-instance draw passes to SetTransform(VIEW). Cull_IsAabbVisible
    s32 viewPos[3];                                // camera world position transformed through viewMatS
    s32 planeD;                                    // dot(viewMatS.rot row at +0x0C..+0x10, worldPos) â€” the view plane constant
    Vec3s pos;                                     // camera world position (x, y, z)
    s16 dist;                                      // Orbit distance from eye to aim point. Written by Camera_SolveCollision (+0xD6 at its end), the fixed
    Vec3s rot;                                     // camera orientation, 12-bit angles (4096 per turn)
};

class CameraManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // CameraManager_Init (override)
    virtual void Update();                         // CameraManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CameraManager_HandleMessage (override)
    virtual void Reset();                          // CameraManager_Reset (override)
    CamMgrEntry entries[16];                       // compacted CAMERAnn/CAMERAnnBOX pairs (both props nonzero)
    s32 entryCount;                                // number of entries
    u8 enableMask[2];                              // 16 enable bits indexed by designer slot (0..14); init 0xFFFF; msg 0xD00 rewrites it
    s8 entrySlot[16];                              // designer slot index (CAMERAnn - 1) of each entry; 0xFF unused
    s32 scriptActive;                              // 1 after this manager started a scripted camera; on exit it calls Camera_ReleaseAny
};

class CameraManager2 : public ScnLogic {
public:
    virtual void PostLoadInit();                   // CameraManager2_Init (override)
    virtual void Update();                         // CameraManager2_Update (override)
    virtual void Reset();                          // CameraManager2_Reset (override)
    CamSetup *camera;                              // CAMERA (Scn_GetPropCamera): {u16 focal; s16 rot[3]; Vec3s eye}
    ZoneList zones;                                // zone list {boxes, count}: BOXES id-list | count (+4): BOXES element count
    u32 flags;                                     // FLAGS prop (CameraManager2Flags): 1/2/4 script flags, 8 track Wolf x, 0x10 track z, 0x20 track verti
    u32 scriptFlags;                               // FLAGS & 7 passed to Camera_StartScripted
    s32 scriptActive;                              // 1 while this manager has started its script
};

class CameraRestriction : public ScnLogic {
public:
    virtual void PostLoadInit();                   // CameraRestriction_Init (override)
    virtual void Update();                         // CameraRestriction_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CameraRestriction_HandleMessage (override)
    void ReadAngleRange(s16 *, s16 *, s32, s32);
    CamRestrict limits;                            // The runtime restriction block (0x34 bytes, +0x40..+0x73) built by init. Update sto
    ScnObject *robot;                              // Scenaric_FindByClass(0x65 = Robot, &this->robot, 1) at init. If present and it answers msg 0x3681 no
};

struct ContactInfo {
public:
    ScnObject *floorObj;                           // object of a floor-class contact
    ScnObject *wallObj;                            // object of a wall-class contact
    ScnObject *movableObj;                         // contact object whose classFlags & 4
    Vec3s floorNormal;                             // init (0,-0x1000,0)
    s16 minContactY;                               // lowest contact Y seen (init box bottom)
    Vec3s wallNormalMean;                          // mean of wall normals
    s16 minAuxY;                                   // min of sweep out-param local_40
};

struct EmitterDriftParams {
public:
    s32 hSpeed;                                    // horizontal drift per second along -sin/-cos(angle)
    s32 vSpeed;                                    // vertical drift per second; vertical points down
    s32 life;                                      // particle life in ticks
    s32 spawnInterval;                             // ticks between spawns; Wolf_UpdateTrailFx rewrites it from the speed (0x
    s16 sizeStart;                                 // particle size at birth; Wolf_UpdateTrailFx lifts the source by half
    s16 sizeEnd;                                   // particle size at death
    u8 sheetIndex;                                 // sprite sheet copied to emitter+0x20
};

struct EmitterRiseParams {
public:
    s32 riseSpeed;                                 // vertical speed per second; negative = up
    s32 life;                                      // maximum particle life in ticks
    s32 spawnInterval;                             // passed to the spawner
    s16 size;                                      // particle size; Wolf_UpdateTrailFx randomises it 10..20
    s16 cap;                                       // height above which (smaller vertical) particles die; Wolf_UpdateTrailFx sets it to the water top (0x
    u8 sheetIndex;                                 // sprite sheet copied to emitter+0x20
};

struct Particle {
public:
    u32 age : 24;                                  // (bits) age in ticks (g_dt units), bits 0..23
    u32 grey : 8;                                  // (bits) grey level splatted to all three channels for the vertex diffuse, bits 24..31 (Emitter_AddParticle 0
    u32 angle : 12;                                // (bits) 12-bit angle, bits 0..11
    u32 size : 12;                                 // (bits) quad size, bits 12..23 (Emitter_AddParticle)
    u32 frame : 7;                                 // (bits) frame index into the sprite sheet, bits 24..30 (Emitter_AddParticle)
    u32 alive : 1;                                 // (bits) bit 31
};

struct EmitterFlagBits {
public:
    u8 active : 1;                                 // (bits) an emitter's flags byte (+0x22) as the code reads it: Wolf_ClearEffects tests it with a byte load, a
};

class ParticleEmitter {
public:
#ifdef SDW_MEMBERS_ParticleEmitter
    SDW_MEMBERS_ParticleEmitter
#endif
#ifdef SDW_EXTRA_ParticleEmitter
    SDW_EXTRA_ParticleEmitter
#endif
    s32 Emitter_UpdateDriftAnimated(const EmitterDriftParams *p, Vec3s *src, s16 angle, s32 spawn);
    s32 Emitter_UpdateDriftLift(const EmitterDriftParams *p, Vec3s *src, s16 angle, s32 spawn);
    s32 Emitter_UpdatePerfume(const EmitterPerfumeParams *params, Vec3s *source, s16 direction, s32 drift, s16 referenceHeight, s32 spawn);
    void Emitter_AddParticle(const Vec3f *pos, s16 angle, s16 size, u8 grey, u8 frame);
    void Sfx_SpawnBillboard(const Vec3s *src, s16 angle, s16 size, u8 grey, u32 interval, u32 now, s32 spawn, u8 frameSel);
    void Emitter_Render(Camera *view, s32 altSheet);
    void Emitter_RenderFlat_Fwd(Camera *view);
    void Emitter_RenderFlat(Camera *view);
    s32 Emitter_UpdateDrift(const EmitterDriftParams *p, Vec3s *src, s16 angle, s32 spawn);
    s32 Emitter_UpdateRiseToCap(const EmitterRiseParams *p, Vec3s *src, s32 spawn);
    s32 Emitter_UpdateColumn(const EmitterColumnParams *params, const Vec3s *anchor, s32 age, s32 spawn);
    s32 Emitter_UpdateFade(const EmitterFadeParams *p, Vec3s *src, s16 angle, s32 spawn);
    s32 Emitter_UpdateTrail(const EmitterTrailParams *p, Vec3s *src, s32 spawnTime, s16 angle, s32 keep);
    void Emitter_Reset();
    void Emitter_Init(s32 count);
    Vec3f *slotPool;
    Particle *particles;                           // pool of count 8-byte particle records {u32 colour; u32 flags}
    u32 lastSpawnTime;
    u32 spawnCursor;                               // time of the last particle spawned
    s32 ring;                                      // next ring slot, (ring + 1) % count (Emitter_AddParticle idiv)
    float lastSrc[3];                              // source position at the last spawn call, as floats
    u8 sheetIndex;                                 // index into the 0x34-stride sprite-sheet table; 0xFF = unset
    u8 count;                                      // number of particle slots; set from the allocator's argument
    EmitterFlagBits flags;                         // bit 0 = emitter active/visible (EmitterFlagBits.active); cleared by Emitter_Reset, tested by Emitter
};

class Vec3f {
public:
#ifdef SDW_MEMBERS_Vec3f
    SDW_MEMBERS_Vec3f
#endif
    float x;
    float y;                                       // vertical points down
    float z;
};

class InlineEmitter16 {
public:
#ifdef SDW_MEMBERS_InlineEmitter16
    SDW_MEMBERS_InlineEmitter16
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[16];                             // 16 x 0xC position records
    Particle particleBuf[16];                      // 16 x 8-byte particle records
};

struct ScnRecordSynth {
public:
    u16 modelResIndex;                             // DAV resource index of the exported model, or 0xFFFF when the export list was empty. Instances live i
    u16 secondaryRes;                              // always set to 0xFFFF = no secondary resource
    Vec3s pos;                                     // spawn position, copied from the caller's Vec3s when non-NULL (4 bytes + 2 bytes)
    u16 classId;                                   // not rotOrFlags: the class id.
    Vec3s rot;                                     // not pad: the record rotation (x, y, z), s16[3] at +0x0C on disc as well as in the synthetic copies.
};

class CannonBall : public ScnMobile {
public:
#ifdef SDW_MEMBERS_CannonBall
    SDW_MEMBERS_CannonBall
#endif
    virtual void PostLoadInit();                   // CannonBall_Init (override)
    virtual void Update();                         // CannonBall_Update (override)
    virtual void Render(Camera *view);             // CannonBall_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CannonBall_HandleMessage (override)
    virtual void Reset();                          // CannonBall_Reset (override)
    s32 SweepMove(Vec3s *, Vec3s, u8);
    void PlaceChaseCamera();
    void SetState(u8);
    void UpdateChaseCamera(Vec3s);
    void UpdateDebris(s32, ParticleEmitter *, ParticleEmitter *);
    Vec3s homePos;                                 // home position: CannonBall_Reset passes &this->homePos to SetPosition (vtable +0x1c) -0x4a
    Vec3s curPos;                                  // pos copied at the start of each flight step (the kill test uses this previous-frame position); set b
    Vec3s targetPos;                               // pipe entry point (msg 0x3383, approached in state 4); msg 0x3384 overwrites it with the current pos
    Vec3s camGoal;                                 // chase-camera end position for pipe transit (msg 0x3385: target - dir/8, vertical target-200, clamped
    Vec3s launchDir;                               // launch vector; velocity = launchDir*1000/0xfff u/s (a 4.12 unit vector gives ~1000 u/s); set by 0x33
    Vec3s velocity;                                // units/s for this frame (per-state formula)
    Vec3s frameDelta;                              // Vec3s_ScaleByDt(velocity), the move applied this frame; stored as hitDir when the lifetime expires
    Vec3s homingTarget;                            // state-2 target point (msg 0x338a from Rook: Wolf pos, 120 up)
    s32 distSq;                                    // state 4: squared distance to targetPos this frame
    s32 prevDistSq;                                // state 4: previous distSq; entry completes once distSq >= prevDistSq
    ScnObject *pipe;                               // sender of 0x3383; gets when the entry point is reached
    ScnObject *launcher;                           // cannon that owns the ball (class 97/146/178 sender of 0x3381/0x3382/0x338a); gets msg 0x4f (arg hitC
    u16 hitClassId;                                // class id of what the ball hit (0xffff = static geometry); argument of msg 0x4f
    ContactInfo contact;                           // Collide_ResolveMove output (0xc0 floorObj, 0xc4 wallObj, 0xc8 movableObj, 0xd4 wallNormalMean); 0x1c
    u8 state;                                      // CannonBallState
    u32 cameraReleased;                            // 1 after msg 0x3386; suppresses CannonBall_UpdateChaseCamera; cleared by SetState(0) and Reset
    u32 lethal;                                    // msg 0x3388 arg; enables the state-1 proximity kill (CanonSheep sends 1, CanonSimple 0)
    u32 firedByCannon;                             // 1 when 0x3381 came from class 97/146/178; the sweep then passes through cannon objects (97/105/146)
    u32 hitPipe;                                   // set by the sweep on touching a Pipe/Pipe2 wall; cleared by 0x3383/SetState(1)/Reset; still set on th
    u32 fxActive;                                  // explosion/splash body is animating; cleared when its anim ends
    u32 hitWolf;                                   // 1 when the ball killed the Wolf: it follows the Wolf during the explosion, stays drawn, and plays no
    s32 explodeTimerMs;                            // state 5 countdown, 2500 ms, g_dtMs
    ScnObject *pipes[8];                           // Pipe (class 96) objects found at Init; each gets msg 0x4f at the end of an explosion
    ScnObject *pipes2[8];                          // Pipe2 (class 163) objects found at Init; also get msg 0x4f
    u32 pipeCount;                                 // count for pipes[]
    u32 pipe2Count;                                // count for pipes2[]
    u32 flightTimeMs;                              // PARABOLIC flight time, g_dtMs accumulated in state 1
    s32 lifetimeMs;                                // PROPERTY LIFETIME (level data 5000/7000)
    s32 lifeLeftMs;                                // state-1 countdown from lifetimeMs; at <= 0 the ball explodes in mid-air
    u8 _pad150[0x8];
    Vec3s camPos;                                  // chase-camera eye; clamped to +-200 of the ball in flight
    u8 _pad15e[0x2];
    Vec3s transitStart;                            // pipe entry point (set when state 4 completes); start of the state-6 interpolation
    Vec3s camTransitStart;                         // camPos at msg 0x3384
    Vec3s transitDelta;                            // exit - entry for state 6
    Vec3s camTransitDelta;                         // camGoal - camTransitStart
    u16 camPitch;                                  // chase-camera pitch passed to Camera_StartScripted
    u16 camYaw;                                    // chase-camera yaw
    u8 _pad17c[0x4];
    s32 transitStartTime;                          // g_gameTime at msg 0x3384
    s32 transitDuration;                           // msg 0x3384 arg, in g_gameTime ticks (1/4096 s)
    u32 ownsCamera;                                // msg 0x3387 arg (CanonSimple 1, Rook 0); enables the chase camera; releases it after the explosion; c
    InlineEmitter16 trailSmokeFx;                  // emitter 0 of 10 (trail smoke), inline storage. Factory constructs each of the ten separatel
    InlineEmitter16 debrisSmokeFx0;                // emitter 1 of 10 (debris 0 smoke), inline storage. Factory constructs each of the ten separa
    InlineEmitter16 debrisSmokeFx1;                // emitter 2 of 10 (debris 1 smoke), inline storage. Factory constructs each of the ten separa
    InlineEmitter16 debrisSmokeFx2;                // emitter 3 of 10 (debris 2 smoke), inline storage. Factory constructs each of the ten separa
    InlineEmitter16 debrisSmokeFx3;                // emitter 4 of 10 (debris 3 smoke), inline storage. Factory constructs each of the ten separa
    InlineEmitter16 trailBubbleFx;                 // emitter 5 of 10 (trail bubbles), inline storage. Factory constructs each of the ten separat
    InlineEmitter16 debrisBubbleFx0;               // emitter 6 of 10 (debris 0 bubbles), inline storage. Factory constructs each of the ten sepa
    InlineEmitter16 debrisBubbleFx1;               // emitter 7 of 10 (debris 1 bubbles), inline storage. Factory constructs each of the ten sepa
    InlineEmitter16 debrisBubbleFx2;               // emitter 8 of 10 (debris 2 bubbles), inline storage. Factory constructs each of the ten sepa
    InlineEmitter16 debrisBubbleFx3;               // emitter 9 of 10 (debris 3 bubbles), inline storage. Factory constructs each of the ten sepa
    EmitterDriftParams trailSmokeParams;           // Emitter_UpdateDrift params {s32 50, -10, 0xa30, 0x146; u16 size 40->100; u8 0}
    EmitterDriftParams debrisSmokeParams;          // drift params {50,-10,0xa30,0x146; size 20->50 (reset by SetState(5), shrunk 1 per frame in state 5);
    EmitterRiseParams trailBubbleParams;           // Emitter_UpdateRiseToCap params {s32 -120, 0xa30, 0x146; u16 15; u16 cap 0; u8 frame 2}
    EmitterRiseParams debrisBubbleParams;          // rise params {-120, 0xa30, 0x32f; 10; cap 0; 2}
    Vec3s debrisPos[5];                            // explosion debris positions (start at the ball pos)
    Vec3s debrisVel[5];                            // debris velocities, Rand_Range(-800,800) per axis; wall bounce = normal/5
    s32 gravityTicks;                              // state 1 PARABOLIC: flightTimeMs/40 (x WEIGHT = downward u/s); state 5: +1 per frame (debris gravity
    u16 gravityWeight;                             // PROPERTY WEIGHT (gravity multiplier)
    u32 parabolic;                                 // PROPERTY PARABOLIC (gravity on in state 1)
    u8 _pad1014[0x6];
    u8 rumbleMotors[2];                            // {0xfe,0xff} passed to Pad_Rumble_stub
    ScnBody explosionFx;                           // embedded explosion/splash model (WAR_IDO_AEXPLOS1 = 3); anim 1 undirected, anim 2 oriented by hitDir
    ScnRecordSynth explosionRecord;                // record synthesised by Scn_BuildRecordFromExport(3)
    u32 hasExplosionFx;                            // 1 if the export existed and explosionFx was initialised
    u32 inWater;                                   // set once the ball enters a g_waterZones box in flight (bubbles on; no second splash)
    u32 noHitDir;                                  // static hit with a zero normal: the explosion plays undirected anim 1
    Vec3s hitDir;                                  // orientation for the anim-2 explosion (hit normal or last frameDelta)
    u16 explodeSound;                              // Sound_Play(0x44) handle
    s32 debrisRemainder = 0;
};

class CannonBall2 : public ScnMobile {
public:
    virtual void PostLoadInit();                   // CannonBall2_PostLoadInit (override)
    virtual void Update();                         // CannonBall2_Update (override)
    virtual void Render(Camera *view);             // CannonBall2_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CannonBall2_HandleMessage (override)
    virtual void Reset();                          // CannonBall2_Reset (override)
    u32 Move(Vec3s *, ContactInfo *, u16, Vec3s *);
    u32 StepFall(u16);
    s32 timer;                                     // 12.12 fixed accumulator of g_dt. In state 1 it is the fall timer, clamped to 0x1e000, and the fall s
    Vec3s spawnPos;                                // Home position captured by PostLoadInit after ScnObject_SnapToGround (dword+word copy); R
    u8 state;                                      // 0 resting/held (waiting for a move modifier), 1 falling under gravity (magnet sound 0x110 while a ma
    BallBits flags;                                // Bit 0 = the last move ended touching a MOVABLE-class object - it is written from ContactInfo+8, whic
    ScnObject *moveModifier;                       // The registered move modifier (platform, lift, carpet, train or magnet). Set by msg 0x18 only when fr
};

class CanonDummy : public ScnBody {
public:
    virtual void PostLoadInit();                   // CanonDummy_Init (override)
    virtual void Update();                         // CanonDummy_Update (override)
    virtual void Render(Camera *view);             // CanonDummy_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CanonDummy_HandleMessage (override)
    virtual void Reset();                          // CanonDummy_Reset (override)
    void SetState(u8);
    u8 state;                                      // CanonDummyState
    u32 ownedBySheepCannon;                        // 1 once a CanonSheep (146) messaged it: drawn at 2x
    Vec3s movePos;                                 // msg 0x3882 arg; SetPosition target on entering state 2
    ScnObject *owner;                              // CanonSheep sender; gets msg 0x4d01 when the state-2 anim ends
};

class CanonSheep : public ScnBody {
public:
    virtual void PostLoadInit();                   // CanonSheep_PostLoadInit (override)
    virtual void Update();                         // CanonSheep_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CanonSheep_HandleMessage (override)
    virtual void Reset();                          // CanonSheep_Reset (override)
    Vec3s ComputeLaunchVelocity(Vec3s);
    Vec3s NearestPoint(Vec3s *, u16, Vec3s);
    void AimDummyAlong(Vec3s);
    void SetState(u8);
    u8 rumbleMotors[2];                            // Pad rumble motor pattern {0xFE, 0xFF} (0xFF terminates), handed to Pad_Rumble_stub with 0xf
    u8 state;                                      // 0 idle, 1 first-person aim (Wolf frozen via msg 0xe and hidden via msg 0x411 arg 0), 2 fire, 3/4/5 t
    Vec3s launchPos;                               // Muzzle position the ball launches from and the point the CanonDummy is told to move to. Seeded at Po
    Vec3s lastLaunchPos;                           // Previous frame's launchPos (compared word by word, copied back); wh
    Vec3s homeRot;                                 // Copy of the placed rotation taken at PostLoadInit. An offset census over -0x4a714
    Vec3s homeRot2;                                // Second copy of the placed rotation, component by component from homeRot. The cen
    Vec3s launchVel;                               // Velocity sent to the CannonBall as MSG_CB_LAUNCH 0x3381's argument: a length-0x1FFE vector from laun
    Vec3s launchPoints[2];                         // Centres of the LAUNCHPOINT1 and LAUNCHPOINT2 boxes, (min+max)/2 per axis, built at PostLoadInit from
    Vec3s aimStartTarget;                          // Ralph's position lowered 0x3C, captured when the 400 ms auto-aim (state 10) begins. Used as the aim
    Vec3s fireTarget;                              // Ralph's position lowered 0x3C, captured at the moment of firing (SetState case 2)
    s32 aimTimeMs;                                 // Milliseconds left in the auto-aim: set to 400 (0x190) on entering state 10, counted down
    s32 fireArmed;                                 // Set to 1 on any aim-mode frame with no button edge and cleared on entering state 1; the fire RELEASE
    s32 wolfFrozen;                                // Non-zero while this cannon holds the Wolf's UI freeze. SetState(1) stores the reply of msg 0xe; SetS
    s32 ballReady;                                 // 1 when the CannonBall is back at rest and available. Cleared when the ball is launched, s
    s32 dummyReady;                                // 1 when the CanonDummy has finished moving to launchPos. Cleared right after MSG_CD_MOVE_TO 0x3882 is
    s32 loaded;                                    // 0 until a Sheep has been loaded, 1 afterwards. While 0 the cannon scans LOADBOX each frame; set at t
    u32 cine;                                      // PROPERTY_CANONSHEEP_CINE (property offset 4); the cinematic Cine_Start plays in state 8.
    u32 cineFlag;                                  // PROPERTY_CANONSHEEP_CINEFLAG (offset 8); Cine_Start's flag argument. Bit 0x80 means the cinematic ca
    u8 _pad0c0[0x4];
    Box *activationBox;                            // PROPERTY_CANONSHEEP_ACTIVATIONBOX (offset 0); standing in it in XZ with the cannon loaded makes msg
    Box *loadBox;                                  // PROPERTY_CANONSHEEP_LOADBOX (offset 0x24); while not loaded, Update state 0 queries it with ObjGrid_
    Box *detectBox;                                // PROPERTY_CANONSHEEP_DETECTBOX (offset 0x10); Ralph standing in it in XZ makes the cannon track him a
    Box *launchPoint1;                             // PROPERTY_CANONSHEEP_LAUNCHPOINT1 (offset 0x18); its centre is launchPoints[0]. Also the base of the
    Box *launchPoint2;                             // PROPERTY_CANONSHEEP_LAUNCHPOINT2 (offset 0x1C); its centre is launchPoints[1].
    Box *launchPoint3;                             // Resolved at PostLoadInit and never read - the offset
    u8 _pad0dc[0x8];
    ScnObject *ball;                               // The level's CannonBall (class 95) from Scenaric_FindByClass(0x5f). Receives MSG_CB_PLACE 0x3382 at i
    ScnObject *dummy;                              // The level's CanonDummy (class 105) from Scenaric_FindByClass(0x69). Hidden at init (or 0x800), moved
    char *cineText;                                // Text_GetClassString(CINETEXT & 0xFF) when cineFlag has bit 0x80, else NULL (else path); pas
};

class CanonSimple : public ScnBody {
public:
    virtual void PostLoadInit();                   // CanonSimple_Init (override)
    virtual void Update();                         // CanonSimple_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CanonSimple_HandleMessage (override)
    virtual void Reset();                          // CanonSimple_Reset (override)
    void AddAimOffset(s16 *, s16, u8);
    void ApplyAim(Vec3s *);
    void MoveTo(Vec3s *);
    void SetState(u8);
    void Translate(Vec3s *);
    void UpdateAim(Pad *);
    u32 csFlags;                                   // CanonSimpleFlags
    u32 stickRepeatMs;                             // ms since the analog step was recomputed (150 ms period)
    u32 dpadRepeatMs;                              // ms since dpadStep last grew
    s16 dpadStep;                                  // d-pad aim step per frame, 5 rising by 2 every 150 ms up to 25
    s32 stickPitchStep;                            // cached analog pitch step (stick Y)
    s32 stickYawStep;                              // cached analog yaw step (stick X)
    u8 state;                                      // CanonSimpleState
    u8 rumbleMotors[2];                            // {0xfe,0xff} for Pad_Rumble_stub
    Vec3s basePos;                                 // ground-snapped position at Init
    Vec3s lastPos;                                 // position used to shift the activation box in MoveTo/Translate
    Vec3s muzzlePos;                               // basePos raised by 80: aim-camera eye, barrel position, ball home/respawn
    Vec3s defaultAim;                              // (ANGLEX, ANGLEY, 0) converted deg*0xfff/360
    Vec3s aim;                                     // current (pitch, yaw, roll) = defaultAim + aimOffset; replaced by the camera forward vector on fire a
    Vec3s aimOffset;                               // player offsets (pitch clamp -vertLimit..40, yaw +-horzLimit); kept across Reset
    u16 horzLimit;                                 // PROPERTY HORZLIMIT, (deg<<12)/360
    u16 vertLimit;                                 // PROPERTY VERTLIMIT, (deg<<12)/360 (upward limit)
    s32 fireDelayMs;                               // state 3 countdown, 400 ms
    ScnObject *pipes[5];                           // Pipe (96) objects; each gets msg 0x66 on fire
    ScnObject *pipes2[5];                          // Pipe2 (163) objects; msg 0x66 on fire
    u16 pipeCount;                                 // count for pipes[]
    u16 pipe2Count;                                // count for pipes2[]
    u8 _pad0d8[0x8];
    Vec3s muzzlePosCopy;                           // copy of muzzlePos written by MoveTo/Translate; no reader found
    Box *activationBox;                            // PROPERTY _ACTIVATIONBOX (tested in XZ only)
    ScnObject *barrel;                             // PROPERTY CANONDUMMY: the CanonDummy barrel model, rotated by ApplyAim
    ScnObject *ball;                               // PROPERTY CANNONBALL: the CannonBall driven by messages
    s32 pitchRemainder = 0;
    s32 yawRemainder = 0;
};

class Case : public ScnBody {
public:
    virtual void PostLoadInit();                   // Case_PostLoadInit (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Case_HandleMessage (override)
};

struct CatapultBits {
public:
    s32 reserved0 : 1;                             // (bits) bit 0 (4-byte signed unit) | Signed one-bit fields match arithmetic shifts in the original catFlags
    s32 sideways : 1;                              // (bits) bit 1 (4-byte signed unit)
    s32 reversed : 1;                              // (bits) bit 2 (4-byte signed unit)
    s32 bucketRequested : 1;                       // (bits) bit 3 (4-byte signed unit)
    s32 bucketArmed : 1;                           // (bits) bit 4 (4-byte signed unit)
    s32 reserved5 : 27;                            // (bits) bits 5..31 (4-byte signed unit)
};

class CollBox {
public:
#ifdef SDW_MEMBERS_CollBox
    SDW_MEMBERS_CollBox
#endif
#ifdef SDW_EXTRA_CollBox
    SDW_EXTRA_CollBox
#endif
    void Box_Translate(const CollBox *src, const Vec3s *offset); /* this = src + offset, flags copied */
    u32 flags;                                     // COLLBOX_* bits: 1 non-solid, 2 cone, 4 ellipsoid, 0x10 cylinder; on the mover any of 0x16 enables sh
    Vec3s min;                                     // min corner (x, y, z); object-local in the object's box list, world after Box_Translate
    Vec3s max;                                     // max corner; +0xC = maxY = bottom (Y points down)
};

struct LaunchArcFlagBits {
public:
    s32 noFreeze : 1;                              // (bits) signed bit view of LaunchArc.flags (+0x34), bit order from the LaunchArcFlags enum: LAF_NO_FREEZE
    s32 planeYZ : 1;                               // (bits) LAF_2D_ZY; read signed shl 30 / sar 31
    s32 full3D : 1;                                // (bits) LAF_3D
    s32 seesawBit3 : 1;                            // (bits) LAF_SEESAW_BIT3
    s32 reserved : 28;                             // (bits) remaining bits
};

class LaunchArc {
public:
#ifdef SDW_MEMBERS_LaunchArc
    SDW_MEMBERS_LaunchArc
#endif
    s32 Step(s32 dt, s16 *outX, s16 *outY, s16 *outZ);
    void InitCoefficients(s32 x0, s32 x1, s32 x2, s32 y0, s32 y1, s32 y2, s32 z0, s32 z1, s32 z2);
    s32 coeffs[9];                                 // per-axis {p0, v, a} for the 3 outputs (x,y,z in 3-D mode); value = p0 + (t*v>>8) + (t*t*a>>15). Same
    s32 t;                                         // arc parameter 0..0x100, advanced by g_dtMs>>2 (0x100 = 1024 ms)
    CamSetup *camera;                              // camera setup {u16 focal; u16 rot[3]; Vec3s pos} for a CamShot, or NULL; receiver zeroes it after sta
    ScnObject *obj;                                // the launched object (seesaw fills it with the object itself); in 2-D modes its current x or z is kep
    u32 camParam;                                  // passed as CamShot param (8th arg of Camera_StartScripted)
    LaunchArcFlagBits flags;                       // LaunchArcFlags as signed one-bit fields (LaunchArcFlagBits): bit 0 no-freeze, bit 1 2-D in the ZY pl
};

class Catapult : public ScnBody {
public:
#ifdef SDW_MEMBERS_Catapult
    SDW_MEMBERS_Catapult
#endif
    virtual void PostLoadInit();                   // Catapult_Init (override)
    virtual void Update();                         // Catapult_Update (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // Catapult_CustomCollide (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Catapult_HandleMessage (override)
    virtual void Reset();                          // Catapult_Reset (override)
    s32 FindFreeLandingSpot(CollBox *box, Vec3s *target, s16 maxShift);
    s32 GroundQuery(struct GroundQuery *query);
    s32 Launch();
    s32 SweepThrown(ScnObject *object, Vec3s *delta);
    void DrawPowerGauge();
    void ScanBucket(s32 seat);
    void SetArmPose(s16 angle);
    void SetState(u8 value);
    void TrackCamera(Vec3s *target, CamSetup *setup, u32 mode);
    void UpdateArmAngle();
    void UpdateBucketArmed();
    void UpdateBucketBox();
    void UpdatePowerInput();
    u8 state;                                      // CatapultState
    s16 armAngle;                                  // pulled-back arm angle = 0xe60 + power*0x140/1024; SetArmPose input
    s16 power;                                     // throw power 0..0x400 (set in state 1); 0x200 = trajectory P2 exactly
    UiQuad powerGauge;                             // HUD gauge quad from GameRes 71 DAV_IDI_ICTJAUG1 at (8,16)
    Box *activationBox;                            // PROPERTY ACTIVATIONBOX (XZ test)
    CamSetup *mobileCamera;                        // PROPERTY MOBILECAMERA camera setup {focal; rot[3]; Vec3s pos}, used to follow the thrown object
    s16 camHoldMs;                                 // 2000 after a landing/hit; the camera is released when it runs out
    u8 _pad08a[0x2];
    s16 launchDelayMs;                             // state 4 delay, set to 1 by Launch (one update)
    u32 wolfFrozen;                                // 1 while this catapult holds the Wolf frozen (msg 0xe sent; 0xf on release)
    u16 powerSound;                                // handle of looping sound 0x76 while power is adjusted
    s16 offset0;                                   // PROPERTY OFFSET0: horizontal offset of the arm pivot
    s16 h0;                                        // PROPERTY H0: pivot height
    s16 armLength1;                                // PROPERTY ARMLENGTH1
    s16 armLength2;                                // PROPERTY ARMLENGTH2
    s16 armLength3;                                // PROPERTY ARMLENGTH3
    Vec3s bucketExtent;                            // bucket box half-extents from the model's last flag-1 box (halved on the cross axes)
    CollBox bucketBox;                             // world-space bucket box (flags 0xa8, min 0xac, max 0xb2): collision (bit4), ground query, occupancy
    u32 stateJustEntered;                          // 1 from SetState until the next Update; blocks the entering press from exiting state 1
    ScnObject *bucketObjs[16];                     // objects in the bucket (ScanBucket); [0] is the one thrown
    u16 bucketCount;                               // count for bucketObjs; > 1 refuses to launch
    Vec3s maxThrowOffset;                          // MAXDIST along the TRAJECTORY P0->P2 XZ direction; scaled by (power-0x200)/512
    s16 maxDist;                                   // PROPERTY MAXDIST
    LaunchArc arc;                                 // embedded LaunchArc: coeffs 0x108, t 0x12c (0..0x100), camera 0x130, obj 0x134 (thrown object), camPa
    Trajectory *trajectory;                        // PROPERTY TRAJECTORY export {u16 3; Vec3s P0,P1,P2} or NULL
    Vec3s thrownLastPos;                           // thrown object's position last frame (arc delta base)
    CatapultBits catFlags;                         // CatapultFlags
    Vec3s landingTarget;                           // P2 + power offset, shifted by FindFreeLandingSpot (x/z used as the arc end)
    s16 armSwingT;                                 // 0..0x100 arm swing (state 2, +g_dtMs>>3) / return (state 3, +g_dtMs>>2) parameter
};

struct CheckpointEntry {
public:
    Box **triggers;                                // trigger box list (Scn_FindIdList of the entry's first id property); FindEntryAt skips an en
    CollBox *wolfDestination;                      // Wolf's respawn box (PostLoadInit): its centre is sent with MSG_WOLF_SAVE_RESPAWN;
    CollBox *sheepDestination;                     // the sheep's respawn box (PostLoadInit): its centre is sent with MSG_SHEEP_SAVE_RES
    u16 count;                                     // number of trigger boxes (FindEntryAt)
    u16 yaw;                                       // orientation (4096/turn), converted from the entry's degrees property by PostLoadInit ( -0x4ab
};

class CheckpointManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // CheckpointManager_Init (override)
    virtual void Update();                         // CheckpointManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CheckpointManager_HandleMessage (override)
    virtual void Reset();                          // CheckpointManager_Reset (override)
    CheckpointEntry *FindEntryAt(Vec3s *position);
    u32 lastChaseTime;                             // g_gameTime when Sam last answered msg 0x480 (alert level 2); checkpoints are frozen for 0x2000 ticks
    CheckpointEntry entries[8];                    // the eight checkpoint entries; the first entryCount are populated
    u32 jointMask;                                 // bit i: entry i only applies when Wolf and sheep are in the SAME entry
    u32 disableEarlierMask;                        // bit i: reaching entry i clears all earlier entries and sends the sheep msg 0x988
    u8 entryCount;                                 // number of populated entries
    Sam *sam;                                      // Sam object found at init (may be null)
};

class Chronometer : public ScnBody {
public:
    virtual void PostLoadInit();                   // Chronometer_PostLoadInit (override)
    virtual void Update();                         // Chronometer_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Chronometer_HandleMessage (override)
    void SetMode(u8 value);
    u8 mode;                                       // 0 = parked (fully in or fully out), 1 = sliding in, 2 = sliding out. Written only by Chronometer_Set
    u8 bufToggle;                                  // Flipped 0/1 every drawn frame; selects unusedBufA or unusedBufB as ScnBody_RenderEx's second argumen
    s32 onScreen;                                  // 1 while the HUD clock is drawn. Set by SetMode(1) once Ralph answers 'not dead' and by msg 0x4703 wi
    u32 panelColor;                                // RGB currently used for the backing sprite (g_spriteCrayon2). Latched from panelColorTarget when the
    u32 panelColorTarget;                          // RGB requested by the last ChronometerMsg: for MSG_CHRONO_SHOW_DEPARTURE 0x4701 and 0xdd5050
    s32 slideOffset;                               // Vertical HUD offset in pixels: 0 = fully in view, 0x20 = parked off screen (the Init value). Mode 1
    u8 unusedBufA[400];                            // First of two 0x190-byte blocks whose address is handed to ScnBody_RenderEx as argument 2 an
    u8 unusedBufB[400];                            // Second block, alternated with unusedBufA by bufToggle and equally dead. Its size is fixed by timeFix
    s32 timeFixed;                                 // The displayed time, in seconds with a 12-bit fraction (1.0 s = 0x1000). Tens digit = (t / 0xa000) %
    Camera hudCam;                                 // A private camera used only to draw the clock model as a HUD element; 0xe0 bytes, exactly filling 0x3
    AnimSprite digitSprite;                        // Digit sprite sheet, AnimSprite_InitFromRes(0x82). Its width/height at sprite +0x30/+0x32 (this+0x4ac
    u16 alarmSoundHandle;                          // Handle of sound 0xe4, started once during the slide-out when timeFixed has reached 0 and no copy is
    u16 tickSoundHandle;                           // Handle of the looping tick, sound 0x3f, started by SetMode(1) and volume-ramped by the ChronometerMs
};

struct CineActorSave {
public:
    Vec3s pos;                                     // ScnObject.pos at cinematic start
    Vec3s rot;                                     // ScnObject.rot
    s32 visible;                                   // (flags & 0x804) == 0
    AnimHeader *anim40;                            // obj+0x40 (anim state block start)
    u32 anim50;                                    // obj+0x50
    u32 anim54;                                    // obj+0x54
    u16 anim5a[5];                                 // obj+0x5a..+0x62 (curAnimId and four following words)
};

struct CineActorSlot {
public:
    ScnObject *obj;                                // the selector-1/2 track's object (Cine_ResolveTracks)
    CineActorSave save;                            // its state at the start, saved when Cine.flags & 0x40 and restored by Cine_Finish (0x56210
};

struct CineAttach {
public:
    ScnObject *child;                              // the object to attach: Cine_AttachEntry loads it and tests its inst_flags & 8
    ScnObject *parent;                             // the object to attach to (pushed); compared with the script target by Cine_OpToggleActorE
    Vec3s offset;                                  // &entry+8 passed as the offset
    Vec3s rot;                                     // &entry+0xe passed as the rotation
    u8 joint;                                      // parent joint index, zero-extended byte
    u32 arg;                                       // ScnObject_AttachTo's fifth argument
};

struct CineDialogue {
public:
    ScnObject *speaker;                            // object whose talk/idle animations follow the dialogue; set from Cine_SetDialogueParams or found via
    u16 idleAnim;                                  // anim played once 2 s have passed since the page began
    u16 talkAnim;                                  // anim played during the first 2 s of a dialogue page and at dialogue start
    CamSetup *camSetup;                            // {u16 focal; u16 rot[3]; Vec3s pos} passed to Camera_StartScripted when dialogue mode starts
};

class Cine {
public:
#ifdef SDW_MEMBERS_Cine
    SDW_MEMBERS_Cine
#endif
    s32 IsActive();
    s32 IsFinished();
    s32 IsWolfReady();
    CineRecord *RecordSize();
    void RenderTaggedObjects();
    s32 ScriptTargetsObject(ScnObject *obj);
    Vec3s *FindFirstPosKeyFor(ScnObject *obj);
    Vec3s *FindFirstPosKey();
    Vec3s *FindFirstRotKey();
    Box *FindCinBBox();
    Box *FindCinSheepBBox();
    void SetCamPos(Vec3s *p);
    void SetCamRot(Vec3s *r);
    void SetCamScalar(u16 v);
    void CaptureCamera();
    void ResetIterators();
    s32 AdvanceScriptCursor();
    s32 SkipRecords();
    void SaveIterator();
    void RestoreIterator();
    void SetDialogueParams(const CineDialogue *p);
    void Load();
    void ResolveTracks();
    void NotifyActors();
    void TagObjectsInBox();
    void Reset();
    void Finish();
    void RunOpcodes();
    void SaveActorState(ScnObject *obj, CineActorSave *s);
    void RestoreActorState(ScnObject *obj, CineActorSave *s);
    void KeyframePos(Vec3s *dest, Vec3s *from, Vec3s *to, s32 t0, s32 t1, u16 kind);
    void LerpVec3s(Vec3s *out, Vec3s *from, Vec3s *to, s32 t0, s32 t1);
    void KeyframeRot(Vec3s *dest, u16 *from, u16 *to, s32 t0, s32 t1, u16 kind);
    u16 LerpAngle(s16 from, s16 to, s32 t0, s32 t1);
    void KeyframeScalar(u16 *dest, u16 from, u16 to, s32 t0, s32 t1, u16 kind);
    void AttachEntry(s32 i);
    void DetachEntry(s32 i);
    void Update();
    void OpKeyPosition();
    void OpKeyRotation();
    void OpStartAnim();
    void OpAdvanceAnim();
    void OpFlickerVisibility();
    void OpKeyCamScalar();
    void OpToggleActorEntry();
    s32 KeyAtLast();
    s32 KeyPastLast();
    void LoadKeyPair();
    void LoadStepKeyPair();
    void Start(u32 id, u32 startFlags, Box *box, Box *sheep, void *text, const CineDialogue *dlg);
    void Stop();
    void Rewind();
    void InitRecordCursors();
    CineTrack *tracks;                             // first track header = cinematic resource + 4 (Cine_Load, Cine_Finish)
    u32 *resource;                                 // cinematic blob from WarReloc_Find; *resource = voice stream id played once by Cine_Update
    u16 trackCount;                                // number of tracks (15-bit count from the type-0x85 entry); Cine_ResetIterators loads +0x2c from it
    s32 time;                                      // the cinematic's current time; every keyframe evaluator (Cine_KeyframeScalar, Cine_LerpAngle
    s32 endTriggerMs;                              // last key time*16 - 1000; flag 0x1000 starts the level-exit fade when time reaches it
    s32 startRawTime;                              // g_rawTime at Cine_Rewind; time = (g_rawTime - this)*1000 >> 12
    s32 active;                                    // 1 while playing; 0 on Cine_Reset/Cine_Stop
    s32 finished;                                  // 1 after stop/reset
    s32 wolfReady;                                 // Game_Frame sends Wolf msg 0x407 until non-zero; Cine_NotifyActors sets 1 directly when fla
    u32 runFlags;                                  // bit 0 = voice stream already started (cleared by Cine_Load, set by Cine_Update)
    CineTrack *targetDesc;                         // pointer to the current target descriptor. Its FIRST BYTE is a selector: 0 means the cinematic's own
    s32 blocksRemaining;                           // outer block counter, decremented when scriptRemaining hits 0
    CineRecord *scriptCursor;                      // cursor into the opcode stream, advanced by Cine_AdvanceScriptCursor. A record is an 8-byte
    s32 scriptRemaining;                           // records left in the current block; when it reaches 0, +0x2c is decremented and +0x28/+0x30 are reloa
    CineTrack *savedTargetDesc;                    // shadow copy of the live iterator word at +0x28, saved by Cine_SaveIterator and restored by
    s32 savedBlocksRemaining;                      // shadow copy of the live iterator word at +0x2c, saved by Cine_SaveIterator and restored by
    CineRecord *savedScriptCursor;                 // shadow copy of the live iterator word at +0x30, saved by Cine_SaveIterator and restored by
    s32 savedScriptRemaining;                      // shadow copy of the live iterator word at +0x34, saved by Cine_SaveIterator and restored by
    u16 *keyCur;                                   // current keyframe pointer, loaded from the script record's +4 by Cine_RunOpcodes
    u16 *keyNext;                                  // next keyframe pointer; the pair bracket the interpolation
    CineActorSlot actorSlots[46];                  // CineActorSlot[] {ScnObject *obj; CineActorSave save} stride 0x2c, one per selector-1/2 track, filled
    u8 _pad838[0x1c];
    ScnObject *taggedObjs[16];                     // objects frozen by Cine_TagObjectsInBox (cap 0x10)
    u8 taggedVisible[16];                          // per tagged object: 1 visible / 0 hidden at tag time; restored by Cine_Finish
    s32 taggedCount;                               // number of taggedObjs
    u32 cineId;                                    // Cine_Start arg 1; low word is the key into the WAR type-0x85 cinematic directory
    Box *sheepBox;                                 // Cine_Start arg 4, or the first WAR_IDO_CINSHEEPBBOX (9) export when 0 and flags & 0x4000000
    Box *cineBox;                                  // Cine_Start arg 3, or the first WAR_IDO_CINBBOX (8) export when 0 and flags & 0x8000000; the box Cine
    u32 flags;                                     // flags (dword: written 0xc000000, read with 32-bit loads; byte tests use the low byte). b
    void *dialogueText;                            // Cine_Start arg 5; shown via Dialogue_Show each frame, enables the skip/dialogue paths
    s32 talkStartTime;                             // g_gameTime when the current dialogue page began; the talk anim plays for 0x2000 (2 s) after it
    Vec3s worldOffset;                             // added to interpolated positions when flags bit 0x10 is set (three s16 at +0x8c0/+0x8c2/+0x8c4)
    s32 hasCameraTrack;                            // 1 if any track has selector 0; gates Camera_StartScripted / Camera_ReleaseScripted_2
    Vec3s camPos;                                  // the cinematic camera's position, written by when the target selector is 0 - i.e. opcode 1 a
    Vec3s camRot;                                  // the cinematic camera's rotation, written by on the selector-0 path of opcode 2
    u16 camFocalScale;                             // quantised FOCAL SCALE for the cinematic camera, animated by opcode 6. Camera_StartScripted turns it
    s32 unk8dc;                                    // zeroed by Cine_Start; no reader found
    s32 cameraCaptured;                            // set 1 by Cine_CaptureCamera, 0 by Cine_Load; Cine_Finish re-runs Cine_RunOpcodes when 1
    s32 claimFlag;                                 // WarReloc_Find's claim in/out: 0 at load (receives the entry's bit 31), 1 at finish (sets bit 31 of t
    CineAttach attachTable[4];                     // CineAttach[4] stride 0x1c {+0 child, +4 parent (compared with the script target by), +8 Vec
    s32 attachCount;                               // number of entries in attachTable (4 slots, stride 0x1c); cleared by Cine_Start, never made positive
    s32 wolfFrozen;                                // result of Wolf msg 0xe at Cine_Start (0 if flags & 0x200 or no Wolf); Cine_Stop sends msg 0xf when n
    s32 cameraHeld;                                // set 1 by Cine_Start; tested by Cine_CaptureCamera, Cine_RunOpcodes, Cine_Update, Cine_Stop
    u8 savedSfxVolume;                             // flags & 0x4000: copy of g_sfxVolume taken at start (then set 0;), restore
    CineDialogue dialogue;                         // the dialogue block {speaker, idle anim, talk anim, camera setup}: Cine_SetDialogueParams co
    s8 dialogueMode;                               // 1 once the cinematic has jumped to its end state and waits on the dialogue box (Cine_Update). s8: th
};

struct CineActorMsg {
public:
    Vec3s *pos;                                    // first position key of the Wolf's track (Cine_FindFirstPosKey), sent with msg 0x12 by Cine_NotifyActo
    Vec3s *rot;                                    // first rotation key of the Wolf's track (Cine_FindFirstRotKey)
    Box *box;                                      // Cine.sheepBox
};

struct CineRecord {
public:
    u16 opcode;                                    // CineOpcode 1..8; indexes g_cineOpStride (Cine_RecordSize)
    u16 count;                                     // number of keys that follow the 8-byte header (Cine_RecordSize); odd counts of the stride-2
    SDW_WARPTR(u16) keyCursor;  // current key; copied to Cine.keyCur/keyNext by Cine_RunOpcodes
};

struct CineTrack {
public:
    u8 selector;                                   // CineTrackSelector: 0 camera, 1/2 scenaric object resolved by Scenaric_FindByRecord, 3/4 cinematic-on
    u8 recordCount;                                // number of script records that follow this header (Cine_SkipRecords, Cine_ResetIterators 0x5
    u16 index;
    SDW_OBJREF(ScnObject) obj;  // the resolved target, written by Cine_ResolveTracks
};

struct CineTrigger {
public:
    u16 cineId;                                    // CINnn: cinematic id passed to Cine_Start (+0x2 padding)
    Box *cineBox;                                  // CINnnBOX list[0], or NULL (Cine_Start then may use WAR_IDO_CINBBOX)
    Box *actBox;                                   // CINnnACTBOX list[0]: the trigger box tested against the Wolf's pos; also the key for msg 0x1400
    u32 flags;                                     // CINnnFLAGS, passed to Cine_Start as CineFlags. Also read here: 1 confirm-press, 0x80 has text, 0x200
    u32 played;                                    // set when started (unless flags & 0x2000); blocks retrigger; cleared by msg 0x1400
    void *text;                                    // Text_GetClassString(manager, CINnnTEXT & 0xff) when flags & 0x80, else NULL; Cine_Start's text argum
    Box *sheepBox;                                 // CINnnSHEEPBOX list[0], or NULL
    u32 started;                                   // set to 1 on every Cine_Start from this trigger; no reader found (write-only)
};

class CinematicsManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // CinematicsManager_Init (override)
    virtual void Update();                         // CinematicsManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CinematicsManager_HandleMessage (override)
    CineTrigger triggers[15];                      // compacted CINnn triggers (0x20 bytes each), only those with ACTBOX != 0 and a resolvable ACTBOX list
    u8 _pad220[0x20];
    s32 triggerCount;                              // number of valid triggers
    u32 saveRequested;                             // set once when a flag-0x8000 trigger fires (autosave or save UI requested). While set, the scan waits
    ScnObject *mcardManager;                       // Scenaric_FindByClass(0x62 MCardManager) at init
};

struct CollCell {
public:
    u32 count;                                     // number of triangle pointers following
    u8 tris[0x4];                                  // (CollTri *[]) pointer-sorted triangle list (indices rewritten to pointers by Coll_BuildCellGrid)
};

struct CollContact {
public:
    ScnObject *obj;                                // owner of the hit box, NULL for static triangles / static boxes
    Vec3s normal;                                  // 4.12 contact normal in game space
    Vec3s point;                                   // reference point on the surface (first triangle vertex in game space, or a corner/centre of the hit b
    s16 *triVerts;                                 // pointer to the raw CollTri.verts of the hit triangle; written only by Collide_SweepBox's triangle pa
};

struct CollMapHeader {
public:
    u16 nx;                                        // grid columns
    u16 nz;                                        // grid rows
    s16 originX;                                   // grid origin x
    s16 originZ;                                   // grid origin z
    s16 cellSizeX;                                 // cell width
    s16 cellSizeZ;                                 // cell depth; cells follow at +0xC in z-outer, x-inner order
};

struct CollRay {
public:
    s32 maxDist;                                   // ray length / search limit in world units
    Vec3s dir;                                     // 4.12 unit direction
    Vec3s origin;                                  // start point
    Vec3s end;                                     // end point (used for cell walking and Y-span culling)
};

struct CollRayHit {
public:
    Vec3s verts[3];                                // game-space vertices of the nearest hit triangle
    Vec3s normal;                                  // normal of the hit triangle
};

struct CollTri {
public:
    Vec3s bbMin;                                   // game-space AABB min (x, vertical-down, z)
    Vec3s bbMax;                                   // game-space AABB max
    Vec3s normal;                                  // 4.12 plane normal in game space (y<0 = floor facing up)
    s16 pad12;                                     // padding after normal (never read in this module)
    Vec3s verts[3];                                // three vertices stored as (x, -y, -z) = detector space /16; queries negate y and z to get game space
    u16 flags;                                     // surface flags (not read by any function of this module)
};

struct RodFlagBits {
public:
    u8 lineOut : 1;                                // (bits) bit 0 (1-byte unsigned unit)
};

class FishingRod : public ScnMobile {
public:
    virtual void PostLoadInit();                   // FishingRod_Init (override)
    virtual void Update();                         // FishingRod_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FishingRod_HandleMessage (override)
    virtual void Reset();                          // FishingRod_Reset (override)
    virtual void BeginFishing();                   // FishingRod_BeginFishing (signature not modelled)
    virtual void EndFishing();                     // FishingRod_EndFishing (signature not modelled)
    s32 GetLineFloorY(Vec3s *position);
    s32 IsTipBlocked(const Vec3s *position, const Vec3s *rotation);
    void CalcTipPos(const Vec3s *position, const Vec3s *rotation, Vec3s *out);
    Vec3s homePos;                                 // pos at FishingRod_Init and at msg 9 arg 1; target of Reset's SetPosition when in the world. Never se
    s16 lineLen;                                   // Fishing line length in world units: 0 at Init, Reset and BeginFishing. Msg 0x61 shortens it (floor 0
    u8 state;                                      // FishingRodState: 0 idle, 1 casting, 2 fishing, 3 retracting
    RodFlagBits rodFlags;                          // FishingRodFlags. Bit 0 = line out (set by BeginFishing, cleared by EndFishing, Init and InitParts);
    u8 pad86[2];                                   // tail padding to sizeof 0x88
};

class RodParts {
public:
#ifdef SDW_MEMBERS_RodParts
    SDW_MEMBERS_RodParts
#endif
    u16 rodClass;                                  // class id of the rod part (0x43 = FishingRod)
    u16 baitClass;                                 // class id of the bait part (Magnet or Salad)
    ScnObject *rod;                                // the rod part object
    ScnObject *bait;                               // the bait part object
};

class CompositeRod : public FishingRod {
public:
    virtual void Update();                         // CompositeRod_Update (override)
    virtual void Render(Camera *view);             // CompositeRod_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CompositeRod_HandleMessage (override)
    virtual void Reset();                          // CompositeRod_Reset (override)
    virtual void BeginFishing();                   // CompositeRod_BeginFishing (override) (signature not modelled)
    virtual void EndFishing();                     // CompositeRod_EndFishing (override) (signature not modelled)
    void InitParts(u16 baitClass);
    RodParts rodParts;                             // RodParts: rodClass = Always 0x43 (CLASSID_FISHINGROD), set by CompositeRod_InitParts; matched by msg
};

struct ControlConfig {
public:
    u8 preset;                                     // 0..2 fixed layouts, 3 = custom (switch); = Progress+0x9a
    u8 actuatorEnable;                             // = Progress+0x9b
    u8 padIsAnalog;                                // = Progress+0x9c
    u16 remap[6];                                  // custom map, slots 0xe 0xf 0xc 0xd 0xb 0xa (Input_ApplyControlConfig, Input_StoreControlConf
};

struct DropMsgArg {
public:
    Vec3s pos;                                     // message 5 (MSG_DROP) argument: put the item down at pos
    u16 placed : 1;                                // (bits) put down on purpose: set by the Wolf's put-down; Seed plants, Perfume starts. A bitfield:
    u16 flag1 : 1;                                 // (bits) cleared by the Wolf, set with bit 0 by the Robot's drop; not traced
};

class Crane : public ScnBody {
public:
    virtual void PostLoadInit();                   // Crane_PostLoadInit (override)
    virtual void Update();                         // Crane_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Crane_HandleMessage (override)
    virtual void Reset();                          // Crane_Reset (override)
    s32 SetCargoAttached(s32 attach);
    s32 SetWolfLock(s32 lock);
    void SetState(u8 newState);
    ScnObject *cargo;                              // the object the crane is lifting: msg 0x37's argument, accepted only in state 0; cleared o
    ScnObject *frozenRiver;                        // the level's FrozenRiver (Scenaric_FindByClass(0x53)); sent msg 0x3e with 1 when the hook ta
    s32 cargoAttached;                             // the cargo's reply to msg 4: non-zero while it hangs on the hook (states 2-4); cleared by
    s32 wolfLocked;                                // the Wolf's reply to msg 0xe (Crane_SetWolfLock); cleared by SetWolfLock(0) and by Crane msg
    u8 state;                                      // 0 idle, 1 lower the hook, 2 take the cargo, 3/4 swing, 5 release, 6 return, 7 wait for the Wolf in c
    u8 swingAnim1;                                 // animation of state 3, chosen per cargo class by msg 0x37 (Wolf 0xb, Sheep 7, SlidingIceCube 4) (0x4a
    u8 swingAnim2;                                 // animation of state 4 (Wolf 0xc, Sheep 8, SlidingIceCube 5)
    u8 releaseAnim;                                // animation of state 5, the drop (Wolf 0xd, Sheep 9, SlidingIceCube 6)
    u8 returnAnim;                                 // animation of state 6 (Wolf 0xe, Sheep 0xa, SlidingIceCube 1)
    u8 _pad079[0x7];
    Vec3s homeRot;                                 // rot saved by PostLoadInit, restored on entering state 0
    u8 _pad086[0x6];
    DropMsgArg drop;                               // msg 5 argument for the cargo: the crane's position plus a per-class offset (Wolf +0x8c/+0x13c, Sheep
    Box *cineBox;                                  // Scn_GetPropBox(CINEBOX). Tested in 2-D against the Wolf in state 7 and passed
    Box *cineSheepBox;                             // Scn_GetPropBox(CINESHEEPBOX), passed to Cine_Start
    Box *activationBox;                            // Scn_GetPropBox(ACTIVATIONBOX). No reader in the Crane's code
    u32 cine;                                      // CINE property: the intro cinematic id; non-zero starts the crane in state 7
    u32 cineText;                                  // CINETEXT property; its low byte indexes the class strings
    u32 cineFlag;                                  // CINEFLAG property, Cine_Start's flags
    void *cineTextStr;                             // Text_GetClassString(this, cineText), Cine_Start's text
    u8 _pad0b0[0x4];
    CamSetup *camera;                              // Scn_GetPropCamera(CAMERA): {u16 focal; s16 rot[3]; Vec3s eye}; started for a Wolf cargo b
    u8 _pad0b8[0x2];
    u16 idleSound;                                 // handle of sound 0x124, restarted on every entry to state 0; 0 from PostLoadInit
    s32 ready;                                     // 1 when there is no intro cinematic or once it has played; Reset returns to sta
};

class CreditsManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // CreditsManager_Init (override)
    virtual void Update();                         // CreditsManager_Update (override)
    virtual void Render(Camera *view);             // CreditsManager_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CreditsManager_HandleMessage (override)
    char *text;                                    // Text_GetClassString(this, TEXTNUM)
    union {
        u32 color;                                     // COLOR; bytes 0..2 are scaled for the fades
        u8 colorBytes[4];
    };
    s16 latchedScreenX;                            // projected x (512 wide) frozen while rot.x rises
    s16 latchedScreenY;                            // projected y (240 high)
    s16 timerMs;                                   // fade timer (g_dtRawMs)
    s16 prevRotX;                                  // rot.x of the last render; a rising rot.x (animated by a cinematic) latches the screen position
    u16 timeAppearMs;                              // TIME_APP (fade-in length; also, wrongly, the fade-out divisor)
    u16 timeDisappearMs;                           // TIME_DIS (fade-out length)
    u8 _pad054[0x1];
    u8 prevVisible;                                // (flags & 0x804) == 0 on the previous active update
    u8 state;                                      // CreditsState 0 wait, 1 fade in, 2 shown, 3 fade out
    u8 active;                                     // set by msg 0x12, cleared by msg 0x13 / end of fade out
    u8 posLatched;                                 // screen position frozen in +0x48/+0x4a
    u8 textJust;                                   // TEXTJUST, alignment passed to Text_Printf
};

class CrocodileLevel09 : public ScnMobile {
public:
#ifdef SDW_MEMBERS_CrocodileLevel09
    SDW_MEMBERS_CrocodileLevel09
#endif
    virtual void PostLoadInit();                   // CrocodileLevel09_Init (override)
    virtual void Update();                         // CrocodileLevel09_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CrocodileLevel09_HandleMessage (override)
    virtual void Reset();                          // CrocodileLevel09_Reset (override)
    void MoveAndTilt(Vec3s *, s16);
    void SetState(u8);
    Trajectory *traj;                              // Scn_GetPropTrajectory(TRAJ): the path
    TrajFollower follower;                         // path follower (speed 500, or 250 with WALK; bias 0x800; continuous; 3-D; arrive radius 50); pointInd
    u8 state;                                      // CrocodileLevel09State
    u8 stateAfterTurn;                             // direction state (2 or 3) resumed when the turn (state 5) ends
    u8 prevState;                                  // state before the last SetState; drives the waypoint bookkeeping
    u8 moveAnim;                                   // walk/run anim id: 7 when PROPERTY_CROCODILELEVEL09_WALK is set, else 3
    s16 waypoint;                                  // trajectory point currently steered to
    s16 otherWaypoint;                             // last waypoint reached; swapped with waypoint when the direction reverses
    s16 swapTmp;                                   // scratch for the waypoint swap
    s16 spinSpeed;                                 // state 7 per-frame yaw step, 0x22b decaying by 1/4 each frame
    s16 startFacing;                               // facing along the first path segment (+0x800, unmasked); restored by Reset
    s16 angleToWolf;                               // atan angle from the croc to the Wolf (12-bit, no +0x800)
    s16 bounceEntryVert;                           // bouncer's y when it entered deathBox; 0x8000 = none
    Vec3s homePos;                                 // trajectory point 0, ground-snapped
    Vec3s moveDelta;                               // per-frame displacement (ScaleByDt of the follower velocity), resolved by MoveAndTilt
    Vec3s scratchVec;                              // first-segment vector at Init, then Wolf - croc
    u16 pointCount;                                // traj->count, number of path points
    s32 deathBoxCount;                             // object count in deathBox on the previous frame (dead state); a rise triggers a scan
    Box *activationBox;                            // Scn_GetPropBox(ACTIVATIONBOX); dereferenced without a NULL check
    CollBox deathBox;                              // world box of the corpse: model box 0 scaled x/z *150/80 and y *200/80, offset by pos
    ScnObject *bouncer;                            // Wolf or FloatingBox that entered deathBox, pending the bounce test
};

class CrocodileLevel11 : public ScnBody {
public:
    virtual void PostLoadInit();                   // CrocodileLevel11_Init (override)
    virtual void Update();                         // CrocodileLevel11_Update (override)
    virtual void Render(Camera *view);             // CrocodileLevel11_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CrocodileLevel11_HandleMessage (override)
    virtual void Reset();                          // CrocodileLevel11_Reset (override)
    s16 HeadingToBoxCenter(Box *box);
    s16 HeadingToObject(ScnObject *object);
    s16 HeadingToPoint(Vec3s point);
    void PlayAnim(u16 id, s32 loop, s32 blend);
    void SetState(u8 value);
    void UpdateCamera();
    u8 state;                                      // CrocodileLevel11State (Update table, SetState table). Read by SetState before bein
    u8 relocatePhase;                              // CrocodileLevel11RelocatePhase in state 5: 0 turn toward the new box, 1 walk, 2 turn back toward Daff
    Vec3s homePos;                                 // Position after Init moved the croc to the x/z centre of homeBox. Restored by Reset unless dead.
    s16 homeFacing;                                // rot.y (+0x16) at Init. Restored by every Reset.
    s32 wolfFrozen;                                // Wolf MSG_FREEZE reply at SetState(5); cleared by msg 0xe and Reset; set to !(MSG_UNFREEZE reply) at
    Box *homeBox;                                  // BOXES entry chosen at Init (containing, or the nearest centre). Reset copies it to curBox.
    Box *curBox;                                   // Box the croc currently guards. Scanned with ObjGrid_QueryBoxPoints (3-D, origins only); its x/z rang
    ScnObject *boxObjects[64];                     // Output of ObjGrid_QueryBoxPoints (cap 64) for curBox (state 0) or bounceBox (state 6).
    s32 boxObjectCount;                            // Count returned by the last query. More than 1 in state 0 triggers the scan, since the croc counts it
    s32 prevBoxObjectCount;                        // Count at the last state-2 scan or dead-state query. A rise in state 6 triggers a bouncer search. Not
    s32 targetIndex;                               // Index in boxObjects of the Wolf (->state 3) or CannonBall (->state 4) found by the state-2 scan. The
    ScnObject *ball;                               // CannonBall being watched in state 4; set by SetState(4), which sends it msg 0x3387 arg 0.
    CollBox *bounceBox;                            // NULL from Init. SetState(4) points it at biteBox. Queried by the dead state with no NULL check.
    CollBox biteBox;                               // Model box 0 with each coord (s16)(c*200)/80 + pos (x2.5 about the croc, axis-aligned). Rebuilt by Se
    Box *cineBox;                                  // Scn_GetPropBox(CINEBOX), passed to Cine_Start.
    Box *cineSheepBox;                             // Scn_GetPropBox(CINESHEEPBOX), passed to Cine_Start (0 in Lvl-13).
    CollBox *activationBox;                        // Scn_GetPropBox(ACTIVATIONBOX). The Wolf's origin inside it (3-D) starts the one-time cinematic.
    u32 cineId;                                    // PROPERTY_CROCODILELEVEL11_CINE, Cine_Start id.
    u32 cineFlags;                                 // PROPERTY_CROCODILELEVEL11_CINEFLAG, Cine_Start flags (418 in Lvl-13).
    s32 cinePlayed;                                // Set to 1 when the activation cinematic ends (state 1). Never cleared after Init, so the cine plays o
    u8 cineTextIndex;                              // Low byte of PROPERTY_CROCODILELEVEL11_CINETEXT. Text_GetClassString index passed to Cine_Start as th
    u32 nextIdleAnimTime;                          // g_gameTime at which state 0 picks a new idle anim (4/5/6). Re-armed to now by SetState(0) and Reset;
    ScnObject *bouncer;                            // Wolf that entered bounceBox on the previous dead-state frame, pending the bounce test.
    s16 bounceEntryVert;                           // Bouncer's y when recorded. 0x8000 = none.
    u8 _pad1c6[0x6];
    u16 turnAnim;                                  // Relocation turn anim: 10 or 11, chosen by the turn direction at SetState(5), toggled for the final t
    s16 turnOffset;                                // Accumulated relocation turn, stepped by (angle difference)/10 per frame.
    ZoneList zones;                                // zone list {boxes, count}: Scn_FindIdList(BOXES) list. | count (+4): Count written by Scn_FindIdList.
    u32 remainDeadAtReset;                         // PROPERTY_CROCODILELEVEL11_REMAINDEADATRESET. If non-zero, Reset does not call SetState(0), even for
    ScnObject *daffy;                              // Scn_GetPropObject(DAFFY); DaffyLevel01 res 64 in Lvl-13. Only used as the rest heading at the start
    CamSetup *camera;                              // Scn_GetPropCamera(CAMERA) resource, a CamSetup. Only its eye position at +8 is used (UpdateCamera).
};

class Crowd : public ScnBody {
public:
    virtual void PostLoadInit();                   // Crowd_PostLoadInit (override)
    virtual void Update();                         // Crowd_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Crowd_HandleMessage (override)
    u8 wantAnimId;                                 // Animation the crowd should be playing; Update starts it whenever it differs from anim.animId (+0x5a)
    s32 reactionTimerMs;                           // Random 0..1499 ms value stored on each reaction message and decremented by g_dtMs, but only on the s
};

class CrumblyGround : public ScnBody {
public:
    virtual void PostLoadInit();                   // CrumblyGround_Init (override)
    virtual void Update();                         // CrumblyGround_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CrumblyGround_HandleMessage (override)
    virtual void Reset();                          // CrumblyGround_Reset (override)
    s32 GetFloor(GroundQuery *);
    void CountAndCapture();
    void PushObjectsDown();
    void SetState(u8);
    ScnObject *overlappingObjects[8];              // CountAndCapture passes this address as the ObjGrid_QueryBoxPoints output ( -46); indexed poin
    s32 overlapCount;
    Vec3s pushDelta;                               // Init's word stores /7d/89 set (0,10,0); PushObjectsDown passes its address to ScnObject::
    Vec3s unusedOffset;                            // Init stores (0,-30,0) into it (word stores /a1/ad). Its purpose is not established; the n
    CollBox triggerBox;
    u8 state;                                      // SetState compares its byte argument with the field and stores it; Update switches on this b
    ScnObject *victim;                             // cleared; stored, then the receiver of a virtual message -3a.
    s32 captureActive;                             // cleared; a DWORD 1 is stored; SetState case 2 compares it with 1.
};

class CrumblyPlat : public ScnBody {
public:
    virtual void PostLoadInit();                   // CrumblyPlat_PostLoadInit (override)
    virtual void Update();                         // CrumblyPlat_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // CrumblyPlat_HandleMessage (override)
    virtual void Reset();                          // CrumblyPlat_Reset (override)
    s32 GroundQuery(struct GroundQuery *);
    void CheckWolfOnTop();
    void PushRiders();
    void SetState(u8);
    ScnObject *hits[8];                            // Output buffer for ObjGrid_QueryBoxPoints: every object whose origin lies inside worldBox. Only 8 slo
    s32 hitCount;                                  // Number of objects the last grid query returned; the loop bound in both CheckWolfOnTop and PushRiders
    Vec3s pushDown;                                // Constant (0, +10, 0) - downward on the down-positive axis - the nudge CrumblyPlat_PushRiders applies
    Vec3s pushUp;                                  // Constant (0, -30, 0), i.e. upward, the nudge CrumblyPlat_PushRiders applies to Ralph. Only that unre
    CollBox worldBox;                              // The model's first collision box translated into world space once at PostLoadInit. It is both the tri
    u8 state;                                      // 0 intact (anim 2 looping, solid, answers ground queries), 1 crumbling (anim 1 one-shot, collision al
};

struct D3DDeviceInfo {
public:
    char strDesc[40];                              // Human-readable device name: the D3D device name ("Direct3D HAL", "RGB Emulation"...) for the prima
    GUID *pDeviceGUID;                             // Points at guidDevice in the same record. Passed to IDirect3D7::EnumZBufferFormats and IDirect3D7::Cr
    GUID guidDevice;                               // The D3D device CLSID. memcmp'd against IID_IDirect3DHALDevice and IID_IDirect3DTnLHalDevi
    u8 ddDesc[236];                                // Copy of the device's D3DDEVICEDESC7. Sub-offsets confirmed against the asm: +0x00 dwDevCaps, +0x04 d
    s32 bHardware;                                 // dwDevCaps & D3DDEVCAPS_HWRASTERIZATION (0x80000). A device is only kept when it is hardware or it is
    GUID *pDriverGUID;                             // DirectDraw driver GUID pointer, NULL for the primary display. Passed straight to DirectDrawCreateEx.
    GUID guidDriver;                               // The DirectDraw driver GUID that pDriverGUID points at (zero/unused for the primary display).
    u8 ddDriverCaps[380];                          // DDCAPS of the hardware driver (dwSize 0x17C). Only dwCaps2 (+8, tested for DDCAPS2_CANRENDERWINDOWED
    u8 ddHelCaps[380];                             // DDCAPS of the HEL (emulation) side, filled by IDirectDraw7::GetCaps and copied along but never read.
    SdwDdsd2Bytes modes[100];// Array of DDSURFACEDESC2 display modes, 100 slots. modes[0] is the width-0 "Windowed Mode" pseudo-ent
    u32 modeCount;                                 // Number of modes in the array; set to 1 by the windowed pseudo-entry, then incremented once per EnumD
    u32 modeIndex;                                 // Currently selected mode. The value that reaches D3DApp_CreateDevice is set by the launcher's WM_INIT
    s32 bDesktopCompatible;                        // 1 when the bpp-filtered mode list still began with the width-0 windowed pseudo-mode, i.e. the device
};

struct DIJoystickInfo {
public:
    char instanceName[80];                         // The controller's DIDEVICEINSTANCEA.tszInstanceName (+0x28 of the instance; strncpy 0x4f bytes at 0x4
    GUID guidInstance;                             // DIDEVICEINSTANCEA.guidInstance (+4 of the instance); copied and passed to IDire
};

class D3DApp {
public:
#ifdef SDW_MEMBERS_D3DApp
    SDW_MEMBERS_D3DApp
#endif
    virtual ~D3DApp();                                // D3DApp_ScalarDeletingDtor
    HRESULT CreateTextureSurface(IDirectDrawSurface7 **ppSurface, u32 width, u32 height);
    u8 StepDevice(u8);                                           /* (launcher) */
    u8 SetDeviceIndex(u8);
    u8 StepMode(u8);
    u8 SetModeIndex(u8);
    void GetDeviceName(char *);
    void GetModeString(char *);
    u32 GetTotalVideoMem();
    u32 GetFreeVideoMem();
    u32 GetTotalTextureMem();
    u32 GetFreeTextureMem();
    u32 GetModeMemCost();
    HRESULT CreateDevice();
    void ReleaseInterfaces();
    int HasJoystick();
    u8 StepJoystick(u8);
    u8 SetJoystickIndex(u8);
    void GetJoystickInstanceName(char *);
    HRESULT CreateMouseDevice(IDirectInputDevice8A **ppDevice);
    HRESULT CreateJoystickDevice(IDirectInputDevice8A **ppDevice);
    HRESULT CreateKeyboardDevice(IDirectInputDevice8A **ppDevice);
    void Frame_LimitFps(u32 maxFps);
    float Frame_GetFps();
    void DrawFpsText(s32 line);
    HRESULT SetRenderTargetSurface(IDirectDrawSurface7 *pNewTarget);
    void RestoreRenderTarget();
    u8 deviceReady;                                // Set to 1 at the end of D3DApp_CreateDevice. Main_Loop's whole render/update block is gated on it via
    HWND__ *hWnd;                                  // (HWND) The render window. Passed to SetCooperativeLevel, the DirectInput SetCooperativeLevel calls a
    HINSTANCE__ *hInstance;                        // (HINSTANCE) Module handle, used only for DirectInput8Create. Typed HINSTANCE__ * (the STRICT HINSTAN
    RECT clientRect;                               // Destination rectangle of the present. Fullscreen: SetRect(0,0,modeW,modeH). Windowed: GetClientRect
    IDirectDraw7 *pDD;                             // The DirectDraw7 object for the selected driver. Vtable uses seen: CreateClipper 0x10, CreateSurface
    IDirect3D7 *pD3D;                              // IDirect3D7 obtained by pDD->QueryInterface(IID_IDirect3D7, &this->pD3D) in D3DApp_CreateDev
    IDirect3DDevice7 *pD3DDevice;                  // IDirect3DDevice7 for the global g_pD3DApp; vtable 0x50 SetRenderState, 0x94 SetTextureSta
    D3DDeviceInfo devices[20];                     // By-value array of enumerated render devices, stride 0x34B4. 20 slots ((0x41E3C-0x2C)/0x34B4).
    u8 deviceCount;                                // Number of devices that survived the constructor's filtering.
    u8 deviceIndex;                                // Currently selected device. Read by D3DApp_CreateDevice, D3DApp_GetModeString, Render_Present and Hol
    IDirectDrawSurface7 *pPrimary;                 // Primary surface. Render_Present calls Flip (vtable 0x2C) on it in fullscreen or Blt (0x14) into clie
    IDirectDrawSurface7 *pBackBuffer;              // Render target. Source of the present Blt, target of IDirect3D7::CreateDevice, source of GetSurfaceDe
    IDirectDrawSurface7 *pZBuffer;                 // Depth buffer, created from the format picked by D3DEnum_ZBufferFormatCallback and AddAttachedSurface
    IDirectInput8A *pDInput;                       // DirectInput8 object; all three device wrappers get their IDirectInputDevice8 from it.
    DIJoystickInfo joysticks[20];                  // By-value array of attached game controllers, stride 0x60: char instanceName[0x50] (DIDEVICEINSTANCEA
    u8 joystickCount;                              // Number of attached controllers; D3DApp_HasJoystick returns it != 0.
    u8 joystickIndex;                              // Controller chosen in the launcher; D3DApp_CreateJoystickDevice uses it.
};

struct Menu {
public:
    MenuPage *items;                               // node array of this menu, loaded into g_menuPages by Menu_SetCurrent (e.g. for g_pauseMenu,
    s16 count;                                     // nodes in use; incremented directly through g_curMenu+4 by Menu_AddItems/Menu_AddPage, zeroed by 0x54
    s16 capacity;                                  // size of items[]: 50 for g_pauseMenu/g_pausedMenu, 4 for the confirm menu, n+2 in. Only 0x54
    s16 cursor;                                    // saved g_menuCursor (highlighted node index)
    s16 lastCursor;                                // saved g_menuLastCursor (node that had focus last frame)
    s16 curPage;                                   // saved g_menuCurPage: index of the open page, whose children are the listed entries
    s16 prevPage;                                  // saved g_menuPrevPage: page left last, receives UNFOCUS in Menu_NotifyPageChange
    u32 unused10;                                  // zero in every initialiser; not read by the menu engine. The rec
};

struct MenuBox {
public:
    char *text;                                    // question / body string
    MenuPage *pages;                               // the choice node array whose root the attached list walks (PorkyLevel01_PostLoadInit -0x45ab3
    Menu *list;                                    // attached choice list; its s16 at +4 is the row count and its u16 at +8 the cursor row
    u8 cursorRow;                                  // highlighted choice row, derived from list[+8]-1
    s32 fits;                                      // 1 when body plus list fits the window
    s16 rect[4];                                   // {x,y,w,h}, frame inflated by 4
    u32 bgColor;                                   // panel fill colour passed to Ui_DrawPanelFill
};

struct MenuPage {
public:
    u8 kind : 2;                                   // (bits) MenuItemFlags bits 0-1: 0 = page/submenu that CROSS enters, 1 = leaf item
    u8 hasHandler : 1;                             // (bits) bit 2 (0x04): +4 is a MenuHandler, else a char* label
    u8 disabled : 1;                               // (bits) bit 3 (0x08): disabled (grey, skipped by navigation)
    u8 selected : 1;                               // (bits) bit 4 (0x10): the highlighted row; a MenuHandler reads it as the blink argument of Text_PrintfStyled
    u8 flagsHigh : 3;                              // (bits) bits 5-7
    u8 childCount;                                 // number of child nodes; incremented by Menu_AddItems/Menu_AddPage on the parent; Menu_Update's draw l
    s16 parent;                                    // index of the parent node in the same array, -1 for the root (node 0)
    void *handler;                                 // union: MenuHandler void(u8 msg, MenuPage *self) when flags bit 2 is set, otherwise the char* label p
};

struct DialogBox {
public:
    s32 active;                                    // 1 while the dialogue is open; Dialog_Close zeroes it
    s32 inputLatch;                                // debounce flag cleared every frame Dialog_Update runs without a confirm
    ScnObject *sender;                             // the NPC that opened the box; receives nothing but is echoed back to the Wolf as the 0x0E/0x0F sender
    u8 selected;                                   // answer row latched on CROSS, copied from the draw box's cursorRow
    char *answerText[3];                           // answer labels from Text_GetClassString(base+1+2i)
    char *replyText[3];                            // reply strings from Text_GetClassString(base+2+2i), replayed by Dialog_UpdateAnswer
    char *questionText;                            // Text_GetClassString(base)
    MenuPage listA[5];                             // the choice list's nodes built by Menu_BuildList (n + 2 for up to three answers; the box's d
    Menu listB;                                    // the Menu over listA built by the same call (the box's drawBox.list)
    MenuBox drawBox;                               // the MenuBox handed to Ui_DrawMenuBox; its text is questionText and its list pointer is &listB
};

class DaffyElf : public ScnMobile {
public:
#ifdef SDW_MEMBERS_DaffyElf
    SDW_MEMBERS_DaffyElf
#endif
    virtual void PostLoadInit();                   // DaffyElf_PostLoadInit (override)
    virtual void Update();                         // DaffyElf_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyElf_HandleMessage (override)
    virtual void Reset();                          // DaffyElf_Reset (override)
    void SetState(u8 newState);
    u16 soundHandle;                               // channel of the calling sound 0x25 started in state 0 (Sound_Play, stored as a word
    u8 state;                                      // DaffyElf state: 0 calling (faces Ralph, sound 0x25), 1 question box open, 2 waiting for the cinemati
    u32 cineId;                                    // PROPERTY CINE (record +0x08); first argument of Cine_Start
    u32 cineTextIndex;                             // PROPERTY CINETEXT (+0x18); its low byte is the class string index of cineText
    u32 cineFlags;                                 // PROPERTY CINEFLAGS (+0x10); Cine_Start flags
    char *cineText;                                // Text_GetClassString(cineTextIndex), passed to Cine_Start as its text
    Box *cineBox;                                  // PROPERTY CINEBOX (Scn_GetPropBox +0x0c): in state 3 Ralph's x/z inside it starts the cinematic; also
    Box *cineSheepBox;                             // PROPERTY CINESHEEPBOX (+0x14): Cine_Start's sheep box
    u32 answerTextIndex;                           // PROPERTY ANSWERTEXTINDEX (+0x04): first class string of the question box (Dialog_Begin)
    u8 answerCount;                                // PROPERTY ANSWERNUMBER (+0x00) truncated to a byte: Dialog_Begin's answer count
    DialogBox dialog;                              // the question box: Dialog_Begin / Dialog_Update / Dialog_UpdateAnswer; answerText[] is what the choic
};

struct DaffyLevel01FlagBits {
public:
    s32 frozen : 1;                                // (bits) signed bit view of DaffyLevel01 +0x12c: bit 0 read as shl 31 / sar 31
    s32 train : 1;                                 // (bits) bit 1, shl 30 / sar 31
    s32 variant13 : 1;                             // (bits) bit 2, shl 29 / sar 31
    s32 reserved : 29;                             // (bits) remaining bits
};

class DaffyLevel01 : public ScnMobile {
public:
    virtual void PostLoadInit();                   // DaffyLevel01_Init (override)
    virtual void Update();                         // DaffyLevel01_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyLevel01_HandleMessage (override)
    virtual void Reset();                          // DaffyLevel01_Reset (override)
    void SetState(u8);
    void UpdateLipSync();
    u8 level;                                      // PROPERTY LEVEL; 13 selects the variant (flags bit2): anim set 7/2/4, box collision on
    u8 state;                                      // DaffyLevel01State
    s8 nbChoices;                                  // PROPERTY NBCHOICES (max 5)
    char *answerTexts[5];                          // string(ANSWERTEXTS+i)
    char *burntText;                               // string(TEXTBURNT); read as answerTexts[5]
    u32 answerVoices[5];                           // VOICE1..VOICE5 via the offset table
    u32 burntVoice;                                // VOICEBURNT; read as answerVoices[5]
    u32 burnt;                                     // msg 0 arg 0 (explosion): blackened
    u32 burntLineSaid;                             // burnt text already spoken once
    u8 answerIdx;                                  // chosen row (or 5 = burnt line)
    CamSetup *textCamera;                          // PROPERTY TEXTCAMERA camera setup {u16 focal; u16 rot[3]; Vec3s pos} for Camera_StartScripted
    MenuPage choicePages[7];                       // node array (root + nbChoices + 1) built by Menu_BuildList
    Menu choiceMenu;                               // Menu record written by Menu_BuildList; count +0xfc used for the box height
    MenuBox questionBox;                           // text = QUESTIONTEXT, +0x110 = &choicePages, +0x114 = &choiceMenu, cursorRow +0x118, fits +0x11c, rec
    DaffyLevel01FlagBits flags;                    // DaffyLevel01Flags: 1 Wolf frozen by this object, 2 Train level (anims 0/1), 4 LEVEL==13 variant
    char *choiceTexts[5];                          // string(CHOICETEXTS+i), drawn by DrawChoice0..4
    u32 menuJustOpened;                            // 1 on the frame of msg 1; blocks the confirm press on that frame; cleared at the end of Update
};

class DaffyLevel02 : public ScnMobile {
public:
#ifdef SDW_MEMBERS_DaffyLevel02
    SDW_MEMBERS_DaffyLevel02
#endif
    virtual void PostLoadInit();                   // DaffyLevel02_Init (override)
    virtual void Update();                         // DaffyLevel02_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyLevel02_HandleMessage (override)
    virtual void Reset();                          // DaffyLevel02_Reset (override)
    s32 IsWolfInBoxXZ(Box *);
    u8 DetectWolf();
    void DrawAlertIcon();
    void FaceWolf();
    void InitHeadRecord(u16 *);
    void SayGoalReached();
    void SetState(u8);
    void UpdateHeadSweep();
    u8 state;                                      // DaffyLevel02State
    u8 savedState;                                 // state to return to after the burnt line (state 0xC)
    Box *cineBox;                                  // PROPERTY CINEBOX: entering it starts the intro cinematic; also passed to Cine_Start
    Box *cineSheepBox;                             // PROPERTY CINESHEEPBOX (Cine_Start sheep box)
    Box *activationBox;                            // PROPERTY ACTIVATIONBOX: game area; leaving it cancels the game; the beam applies only inside (3D tes
    Box *goalBox;                                  // PROPERTY GOALBOX: win zone
    Box *startBox;                                 // PROPERTY STARTBOX: start zone (no detection inside)
    u32 cine[2];                                   // PROPERTY CINE, CINE2 (indexed by cineIndex +0xb5)
    u32 cineTextBase;                              // PROPERTY CINETEXT; line shown = string(CINETEXT + talkPhase)
    u32 cineFlags;                                 // PROPERTY CINEFLAGS
    s16 headSweepPhase;                            // 12-bit phase, += sweepSpeed*(g_dtMs>>1)
    s16 headYaw;                                   // sin(phase)*maxAngle/360, applied to joint 4
    s16 headSweepSpeed;                            // 1
    s16 headMaxAngle;                              // 70 (degrees)
    s16 relAngleToWolf;                            // (yawToWolf - (yaw+headYaw)) & 0xfff; in view when < 0x38. Also rotates the HUD head
    s16 yawToWolf;                                 // atan2 bearing + 0x800
    char *curText;                                 // current line string
    u8 talkPhase;                                  // DaffyLevel02TalkPhase (also the CINETEXT offset)
    u8 cineIndex;                                  // 0 = CINE, 1 = CINE2 (set after the first cinematic)
    u8 detectResult;                               // DaffyLevel02Detect from DetectWolf
    u8 textBurnt;                                  // PROPERTY TEXTBURNT string index (state 0xC, voice 0xD)
    s32 startBoxTimerMs;                           // ms spent in startBox while running (+= g_dtMs)
    s32 startBoxTimeoutMs;                         // nag threshold: 7000, or 3000 after being caught while carrying a sheep
    u32 voiceId;                                   // voice for the current line (0x10..0x16 Lvl-02, 0x1c..0x22 Lvl-03)
    u32 answerVoices[3];                           // voices for the 3 dialog answers (Dialog_UpdateAnswer): {0x19,0x17,0x18} Lvl-02 / {0x1f,0x20,0x21} Lv
    u32 isLevel2Variant;                           // 1 when no HiddenRocks (class 0x23) exists (Lvl-02): voice set and detect order
    s16 textBoxRect[4];                            // {50,50,412,140} for the Yes/No text box
    u32 gameRunning;                               // red-light game in progress
    u32 gameArmed;                                 // entering startBox will start a game
    u32 introDone;                                 // rules cinematic seen; kept across Reset
    u32 challengeWon;                              // set in SayGoalReached; talking then opens the 3-answer dialog
    u32 burnt;                                     // msg 0 arg 0; cleared after the burnt line
    u32 wolfFrozen;                                // result of Wolf msg 0xE; unfreeze sends msg 0xF
    u32 startBoxNagGiven;                          // nag (phase 9) given this stay in startBox
    Vec3s spawnRot;                                // rot at Init; restored on game start, win and Reset
    u8 _pad0fe[0xa];
    DialogBox dialog;                              // Dialog_Begin(first string 10, 3 answers) box, opened after a win
    u8 headBufToggle;                              // alternates 0/1 each icon draw
    u8 _pad191[0x3];
    u8 headRenderBufA[400];                        // head icon render buffer (ScnBody_RenderEx)
    u8 headRenderBufB[400];                        // second head icon buffer
    s32 alertIconColor;                            // index into g_samZoneColors for the icon frame; always 1
    ScnBody head;                                  // embedded ScnBody (0x64 bytes, vtable ScnBody) with model WAR_IDO_ATEROB01; yaw +0x4ce; tint +0x4d2/+
    u16 headRecord[10];                            // fake level record for the head (DaffyLevel02_InitHeadRecord)
};

class DaffyLevel09 : public ScnMobile {
public:
    virtual void PostLoadInit();                   // DaffyLevel09_PostLoadInit (override)
    virtual void Update();                         // DaffyLevel09_Update (override)
    virtual void Render(Camera *view);             // DaffyLevel09_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyLevel09_HandleMessage (override)
    virtual void Reset();                          // DaffyLevel09_Reset (override)
    void SetState(u8 newState);
    TrajFollower walkOut;                          // follower for the TRAJ path, started by SetState(3) with speed 0x145, bias 0x800, arrive radius 0x32
    TrajFollower walkBack;                         // follower for the TRAJRET path, started by SetState(6); stepped in state 6
    Trajectory *traj;                              // PROPERTY_DAFFYLEVEL09_TRAJ trajectory (Scn_GetPropTrajectory); the walk to the sign
    Trajectory *trajRet;                           // PROPERTY_DAFFYLEVEL09_TRAJRET trajectory; the walk back
    ScnObject *swirlSign;                          // the level's SwirlSign (Scenaric_FindByClass(89)). Queried with msg 0x3081 (= sign still on
    ScnObject *elmer;                              // the level's Elmer (Scenaric_FindByClass(90)); looked up but not read by this class
    u8 state;                                      // 0 holding the placard, 1 intro cinematic, 2 disguise cinematic, 3 walk out along TRAJ, 4/5 flip anim
    CamSetup *camera;                              // PROPERTY_DAFFYLEVEL09_CAMERATRAJ camera record (Scn_GetPropCamera); its Vec3s at +8 is the
    u16 camRotX;                                   // pitch of the state-3 scripted camera: atan2(dVert, sqrt(dx*dx+dz*dz)) in 4096ths, masked 0xfff (0x43
    u16 camRotY;                                   // yaw of the state-3 scripted camera: atan2(-dx, dz) in 4096ths, masked 0xfff
    u8 _pad0d8[0x4];
    s32 wolfFrozen;                                // non-zero while this object holds the Wolf's UI freeze (msg 0xe reply); released with msg 0
    u32 unke0;                                     // cleared by PostLoadInit and never read anywhere in the class
    Vec3s homePos;                                 // placed position, copied after SnapToGround; Reset teleports back to it
    u16 soundHandle;                               // handle of sound 0x6a started when the placard goes up; cleared by PostLoadInit and Reset,
    Box *cineBox;                                  // PROPERTY_DAFFYLEVEL09_CINEBOX; Cine_Start argument 3
    Box *cineSheepBox;                             // PROPERTY_DAFFYLEVEL09_CINESHEEPBOX; Cine_Start argument 4
    Box *activationBox;                            // PROPERTY_DAFFYLEVEL09_ACTIVATIONBOX; in state 7 Ralph inside it starts the intro cinemati
    ZoneList deguisBoxes;                          // PROPERTY_DAFFYLEVEL09_BOXDEGUIS id list: in state 7 Ralph inside one of them while wearin
    u32 cine;                                      // PROPERTY_DAFFYLEVEL09_CINE; played by state 1
    u32 cineDeguis;                                // PROPERTY_DAFFYLEVEL09_CINEDEGUIS; played by state 2
    u32 cineFlag;                                  // PROPERTY_DAFFYLEVEL09_CINEFLAG; Cine_Start argument 2 for state 1
    u32 cineDeguisFlag;                            // PROPERTY_DAFFYLEVEL09_CINEDEGUISFLAG; Cine_Start argument 2 for state 2
    s32 placardTimeMs;                             // ms left with the placard up: set to 2000 by msg 0 and counted down by g_dtMs in state 0;
    u32 cinePlayed;                                // latched when any of this object's cinematics has run; blocks the ACTIVATIONBOX trigger in state 7 (0
    u32 deguisCinePlayed;                          // latched when the disguise branch has run once; afterwards state 7 goes straight
    u8 cineText;                                   // PROPERTY_DAFFYLEVEL09_CINETEXT; the index passed to Text_GetClassString for state 1's dia
    u8 cineTextDeguis;                             // PROPERTY_DAFFYLEVEL09_CINETEXTDEGUIS; the same for state 2
    ScnBody placard;                               // a second body constructed in place by the factory and initialised from a synthesised reco
    u16 placardRecord[10];                         // the 0x14-byte WAR record synthesised by Scn_BuildRecordFromExport(2...) for the placard
    u32 placardValid;                              // 1 when the synthesised record resolved and the placard body was initialised
};

class DaffyMilitary : public ScnMobile {
public:
#ifdef SDW_MEMBERS_DaffyMilitary
    SDW_MEMBERS_DaffyMilitary
#endif
    virtual void PostLoadInit();                   // DaffyMilitary_PostLoadInit (override)
    virtual void Update();                         // DaffyMilitary_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyMilitary_HandleMessage (override)
    virtual void Reset();                          // DaffyMilitary_Reset (override)
    void FaceWolf();
    void UpdateTalkAnim();
    void SetState(u8 newState);
    DialogBox dialog;                              // the TALKABOUT question box: Dialog_Begin (SetState 3), Dialog_Update (Update state 3, 0x43
    ScnObject *iceCube;                            // the level's IceCube (Scenaric_FindByClass(60, &iceCube, 1)); Reset sends it msg 0x32 (re
    u32 talkAboutText;                             // PROPERTY TALKABOUTTXT (props +0x14): first string id of the question, Dialog_Begin's firstStringId (
    s32 talkTimer;                                 // 4.12 s spent in the answer / punish states while the letterbox is up (+= g_dt); from 0x2
    char *cineText;                                // PROPERTY CINETXT string (Text_GetClassString), read only when FLAGCINE & 0x80, else NULL ( -0
    char *punishText;                              // PROPERTY PUNISHTEXT string (Text_GetClassString): the line said in state 5 with voice 0xd
    u32 cineFlags;                                 // PROPERTY FLAGCINE (props +8): Cine_Start's startFlags; bit 0x80 = CINETXT is given
    u16 sound;                                     // a sound handle: zeroed in PostLoadInit, stopped and zeroed by Reset; nothing in
    u16 cineId;                                    // PROPERTY CINE (props +0): the release cinematic started in state 2
    u8 state;                                      // DaffyMilitaryState: 0 frozen in the ice cube, 1 free (talkable), 2 release cinematic, 3 question box
    u8 talkAboutChoiceCount;                       // PROPERTY TALKABOUTNBCHOICE (props +0x10): Dialog_Begin's answerCount
    s8 wolfFrozen : 1;                             // (bits) bit 0: cleared by PostLoadInit, Reset and msg 0xE (another object took over the Wolf freeze); nothin
    s8 burnt : 1;                                  // (bits) bit 1: set by msg 0 with arg 0 (explosion) while free: blackened; read shl 6 / sar 7 (signed) at 0x4
    s8 punished : 1;                               // (bits) bit 2: the punish line has been given once (set with SetState 5, read shl 5 / sar 7 at 0
};

class DaffyScene : public ScnMobile {
public:
#ifdef SDW_MEMBERS_DaffyScene
    SDW_MEMBERS_DaffyScene
#endif
    virtual void PostLoadInit();                   // DaffyScene_PostLoadInit (override)
    virtual void Update();                         // DaffyScene_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyScene_HandleMessage (override)
    virtual void Reset();                          // DaffyScene_Reset (override)
    char *PickIdleChatterLine();
    char *PickResultLine();
    void SetState(u8);
    void StartRandomTalkAnim(s32);
    u8 state;                                      // current scene state (Update dispatches on it)
    u8 nextState;                                  // pending state
    u8 _pad07e[0xb];
    u8 cameraCount;                                // number of speech cameras in speechCameras
    u16 chatterCount;                              // chatter counter
    u16 talkTimer;                                 // talk timer
    char *text;                                    // current subtitle text
    void *properties;                              // the designer property record
    CamSetup *speechCameras[5];                    // Scn_GetPropCamera results
    CamSetup *activeCamera;                        // camera of the current speech
    s32 wolfFrozen;                                // 1 while the scene holds Ralph frozen
    u32 voice;                                     // current voice id
    ScnObject *crowds[3];                          // the Crowd objects of the scene
    u8 crowdCount;                                 // entries in crowds
    u8 _pad0c5[0x7];
};

struct TextBox {
public:
    char *text;                                    // string to render
    s8 confirmChoice;                              // Written by Ui_DrawTextBox only
    s32 fits;                                      // 1 when lineCount+3 fits in the font's row count; the paging UI is only run when set
    s16 rect[4];                                   // {x, y, w, h} in virtual HUD space; the frame is drawn inflated by 4 on every side
    u32 bgColor;                                   // panel fill colour, forced to 0x080808 by Ui_DrawTextBox
};

struct TrainingHelpSlot {
public:
    u32 type;                                      // TYPE_n (PostLoadInit)
    Trajectory *trajectory;                        // resolved TRAJECTORY_n; count at +0, nodes at +2
};

struct TrainingLineBits {
public:
    s32 jokeDone : 1;                              // (bits) signed bit view of DaffyTrainingLevel +0xbc: bit 0 read shl 31 / sar 31 by Reset; set by
    s32 helpPending : 1;                           // (bits) bit 1, shl 30 / sar 31 in Update; set by Reset (or 2), cleared by PostLoadInit/Update (an
    s32 reserved : 30;                             // (bits) remaining bits
};

class DaffyTrainingLevel : public ScnMobile {
public:
#ifdef SDW_MEMBERS_DaffyTrainingLevel
    SDW_MEMBERS_DaffyTrainingLevel
#endif
    virtual void PostLoadInit();                   // DaffyTrainingLevel_PostLoadInit (override)
    virtual void Update();                         // DaffyTrainingLevel_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyTrainingLevel_HandleMessage (override)
    virtual void Reset();                          // DaffyTrainingLevel_Reset (override)
    s32 EatTextTag(u32);
    u32 MoveTo(Vec3s, s32, u8);
    void ParseTextTags();
    void PlayPendingVoice();
    void SetHelpLine(u32);
    void SetLine(u8, u32);
    void SetMode(u8, u8);
    u8 mode;                                       // SetMode stores arg 1; Update/HandleMessage test it
    u8 _pad07d[0x1];
    u8 watchState;                                 // SetMode stores arg 2; Update switches on it (0..4) driving the Watch
    Vec3s savedWolfPos;                            // SetMode mode 1 copies g_pWolf's +0xc/+0xe/+0x10 here, and Update restores the W
    s32 wolfMoved;                                 // Reset writes the literal 1 and 0, Update only tests it against 0
    TextBox confirmBox;                            // Update fills text 0x8c, rect 0x98..0x9e, bgColor 0xa0 and passes &this->+0x8c to Ui_DrawTextBox; rea
    u8 _pad0a4[0x8];
    char *currentText;                             // written (g_trainingEmptyText), read
    u8 wolfFrozen;                                 // set 1 when the Wolf accepts msg 0xe, cleared after msg 0xf (SetMode)
    u8 confirmTag;                                 // DtlConfirmTag: set 1 / cleared; 2 while Update shows the yes/no box
    u8 cameraNotify;                               // Reset sets 1 (byte); Update sends msg to helpCamera and clears
    u8 initNotify;                                 // PostLoadInit sets 1/0 (byte); Update notifies helpCamera once
    s32 retryPhase;                                // Reset increments and clamps to 1; SetMode mode 2 zeroes
    s32 replayingHelp;                             // set 1 by HandleMessage's MSG_USE arm, cleared by Reset, zeroed by PostLoadInit and by SetMode mode 2
    TrainingLineBits lineFlags;                    // PostLoadInit clears bit 1, Reset sets it (|=2), Update tests bit 1 and clears it at 0x43
    u32 fromPreviousMask;                          // FROMPREVIOUS property (prop +0x5c = record+0x70;); Reset tests bit helpIndex
    u32 helpCount;                                 // counts the slots whose TRAJECTORY_n resolved, not whose BOX_n resolved - the gate tests
    u32 helpIndex;                                 // current help point; compared unsigned (jbe/ja) in SetMode
    u32 trajNodeIndex;                             // index into the current help point's trajectory node list â€” Reset's walk target is *(this+0x12c+idx*8
    u32 savedHelpIndex;                            // Update watch state 1 copies helpIndex
    s32 challengeDuration;                         // PostLoadInit: record+0x98 * 100
    u32 helpTextIds[20];                           // indexed dword store; low-byte fetch
    TrainingHelpSlot helpSlot[20];                 // +0x128+i*8 is TYPE_n ( from table+6) and +0x12c+i*8 is the resolved TRAJECTORY_n id list (0x
    Box *helpBox[20];                              // the resolved BOX_n id-list entry ( loads table+2 = BOX; stores it to this+4*helpCou
    Box *startBox;                                 // Scn_GetPropBox(record,0x7c)
    Box *finishBox;                                // Scn_GetPropBox(record,0x80)
    ScnObject *cameras;                            // CAMERAS property: Scn_GetPropObject(record,0x50); msg 0xd00 target
    ScnObject *cinematics;                         // CINEMATICS property: Scn_GetPropObject(record,0x54); msg 0x1400 target
    u8 cameraFlags[2];                             // PostLoadInit zeroes two bytes
    u16 animPhase;                                 // Reset/SetMode zero (word); Update idle anim step
    u16 initFlag;                                  // DtlJumpPhase: the phase of a hop, read and stepped by MoveTo; set to 1 by PostLoadInit, S
    s32 movementDone;                              // MoveTo sets 1/0
    u32 lastSpeechMs;                              // SetMode mode 1 stamps it with g_gameTimeMs; Update's idle fallback tests g_gameTimeMs -
    u32 currentTime;                               // MoveTo: g_gameTime*1000>>12
    u32 lastTargetTime;                            // MoveTo
    u32 jumpDuration;                              // MoveTo: 400 or Anim_GetDurationMs
    u8 gait;                                       // MoveTo gait of the current target
    Vec3s moveStart;                               // MoveTo copies pos
    Vec3s target;                                  // MoveTo target x/z words
    u8 _pad252[0x1];
    s8 deathDelay;                                 // Update watch state 3 countdown
    u8 useLatched;                                 // set 1 by HandleMessage's MSG_USE arm alongside the Wolf freeze; zeroed by PostLoadInit
    u8 stopTag;                                    // set 1 / cleared
    s32 viewTag;                                   // stored
    u32 viewStart;                                 // g_gameTime stamp
    s32 lockTag;                                   // stored
    s32 dialogueRestart;                           // Update/Reset
    s32 robotSequence;                             // SetMode/Update
    s32 forcedFreeze;                              // SetMode/Update
    u8 _pad270[0x4];
    ScnObject *robot;                              // Update: class 0xa5 found in the help box
    ScnObject *watch;                              // Scenaric_FindByClass(0x67,&this->watch,1)
    u32 pendingVoice;                              // stored, tested
    u32 jokeVoice1;                                // record+0x88
    u32 jokeVoice2;                                // record+0x8c
    u32 helpVoice;                                 // HELPVOICE property (prop +0x6c); written by PostLoadInit, not read by the class code
    u32 jokeEnabled;                               // PROPERTY JOKEENABLED (prop +0x70); used as a BITMASK, Reset tests 1 << helpIndex â€” not a plain boole
};

class DaffyWheel : public ScnMobile {
public:
    virtual void PostLoadInit();                   // DaffyWheel_Init (override)
    virtual void Update();                         // DaffyWheel_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DaffyWheel_HandleMessage (override)
    virtual void Reset();                          // DaffyWheel_Reset (override)
    void StartCamera(CamSetup *, u32, s32);
    u8 state;                                      // 0 intro (start both scripted cameras, anim 0x27, go to 2), 1 play anim 0x29 then 2, 2 play anim 0x28
    u16 unk7e;                                     // Cleared by Reset and again when msg 0x6182 re-arms the camera; no reader in the class, and the class
    u8 unk080[8];                                  // Eight bytes the class never reads or writes: ScnMobile ends at 0x7c, own accesses are only +0x7c, +0
    CamSetup *camShot;                             // Camera setup {u16 focal; u16 rot[3]; Vec3s pos} from PROPERTY_DAFFYWHEEL_IDCAMERA (4), used for the
    CamSetup *camShotBegin;                        // Camera setup from PROPERTY_DAFFYWHEEL_IDCAMBEGIN (0), used for the opening shot in state 0 with CamS
};

struct DanceStep {
public:
    u16 step;                                      // queued dance step (PushStep word store; ClearGhostQueue)
    DancingGhost *partner;                         // partner ghost for the step
    Vec3s point;                                   // target point, copied by value ( ; /27e2/27f8)
    uptr argument;                                  // step argument. Record stride 0x14: PopStep copies 20 bytes
};

struct DancingGhostFlagBits {
public:
    u8 tambourine : 1;                             // (bits) bit view of DancingGhost.danceFlags (+0xf4): a tambourine ghost (mov cl,[+0xf4]; and cl,1 at 0x44174
    u8 paired : 1;                                 // (bits) partner coordination bit (UpdateState)
    u8 leader : 1;                                 // (bits) partner coordination bit (UpdateState)
    u8 reserved : 5;                               // (bits) remaining bits
};

class DancingGhost : public ScnBody {
public:
#ifdef SDW_MEMBERS_DancingGhost
    SDW_MEMBERS_DancingGhost
#endif
    virtual void PostLoadInit();                   // DancingGhost_PostLoadInit (override)
    virtual void Update();                         // DancingGhost_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DancingGhost_HandleMessage (override)
    virtual void Reset();                          // DancingGhost_Reset (override)
    s32 CarryStep(DancingGhost *, Vec3s *, Vec3s *, u16, s32);
    s32 PushStep(u16, DancingGhost *, Vec3s, uptr);
    s32 StepToward(Vec3s *, u16, s32, s32);
    s32 UpdateCarry();
    void Despawn();
    void PopStep();
    void ReleasePartnerWait();
    void StartDanceCamera(Vec3s *, Vec3s *);
    DanceStep steps[6];                            // the step queue (queueCount +0xdc counts it, max 6)
    u16 queueCount;                                // 0..6; PushStep refuses at 6 ( cmp 6/jl); the PopStep decrement is
    s32 newStep;                                   // set to 1 when a step is pushed
    s32 stepDone;                                  // cleared on a new step
    u32 elapsedMs;                                 // step clock, accumulated only while unpaused (UpdateState); compared unsigned
    u16 soundHandle;                               // cleared by Init, stopped and cleared by Despawn
    Box *chatBox;                                  // Scn_GetPropBox result stored by Init
    DancingGhostFlagBits danceFlags;               // flag byte; bit view DancingGhostFlagBits (bit 0 tambourine, bits 1/2 partner coordination)
    AltModel altTambourine;                        // AltModel: outAlts for id-list 0xc0
    AltModel model;                                // AltModel: outMain; 0x108+0x10 = 0x118 = carryPhase
    u16 carryPhase;                                // UpdateCarry compares with 0/1 and writes 1
    u16 animPhase;                                 // UpdateState case 13 clears and increments it
    Vec3s homePos;                                 // copied from pos by Init
    Vec3s homeRot;                                 // copied from rot by Init
    TrajFollower traj;                             // TrajFollower: TrajFollower_Init(this+0x128, g_dgTrajectory, 500, 0x800, 1, 1, 0x32); 0x128+0x20 = 0x
    Vec3s previousStep;                            // cleared by Init ( /6f4/700); StepToward stores the per-axis movement
};

class Sprite {
public:
#ifdef SDW_MEMBERS_Sprite
    SDW_MEMBERS_Sprite
#endif
    void DrawThunkAt(u32 *layer, s32 x, s32 y, u32 color, u32 flip);
    s32 LoadFromRes(u16 resType);                                /* Sprite_LoadFromRes */
    void Draw(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u32 flipMode); /* Sprite_Draw */
    void DrawFrame(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u32 frame, u32 flipMode);
    void DrawThunk(u32 *layer, s32 x0, s32 y0, s32 x1, s32 y1, u32 color, u32 flipMode);
    void *texture;                                 // non-NULL means the sprite is loadable; Sprite_Draw returns immediately when it is 0
    u8 _pad004[0x2];
    u16 texPage;                                   // texture page index; +4 gives the RenderPoly.type written to g_draw2dPolyType
    s16 widthMinus1;                               // the stored value is (bitmap width - 1), not the width. Same for +0xA
    s16 height;                                    // sprite height in texels
    u8 u;                                          // sub-rect origin U within the 256px texture page
    u8 v;                                          // sub-rect origin V within the 256px texture page
};

class DancingGhostManager : public ScnLogic {
public:
#ifdef SDW_MEMBERS_DancingGhostManager
    SDW_MEMBERS_DancingGhostManager
#endif
    virtual void PostLoadInit();                   // DancingGhostManager_PostLoadInit (override)
    virtual void Update();                         // DancingGhostManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DancingGhostManager_HandleMessage (override)
    virtual void Reset();                          // DancingGhostManager_Reset (override)
    s32 UpdateBeatTrack();
    void AbortRound();
    void DespawnDanceSet();
    void FindTwoNearestGhosts();
    void HandleBeatResult();
    void ParseSequences(void *);
    void SetGhostSpot(const Box *);
    void SetState(u8, s32);
    CamSetup *danceCams[4];                        // four Scn_GetPropCamera(0x38/0x3c/0x40/0x44); read as {u16 focal; s16 pitch; s16 yaw; s16 roll; Vec3s
    u16 beatSound;
    Sprite spriteSquare;
    Sprite spriteTriangle;
    Sprite spriteCircle;
    Sprite spriteCross;
    Sprite spriteNote;
    Sprite spriteBarNote;
    Sprite spriteFrame;
    Sprite *currentSprite;
    Sprite spriteFrameDark;
    Sprite spriteTrack;
    u16 currentStep;
    u16 drawnStep;
    u16 previousStep;
    u16 padMask;
    u16 pressedMask;
    s32 eventCode;                                 // pending FAILURE code, no value means success. 1 = right button more than 200 ms
    s32 reactionQueued;
    s32 wolfFrozen;
    s32 externalFreeze;
    s32 roundArmed;
    s32 insideRing;
    s32 insideHole;
    s32 outsideRing;
    s32 insideSafe;
    s32 insideDanceZone;
    s32 pressHandled;
    u8 _pad120[0x4];
    s32 danceCompleted;
    s32 unk128;                                    // written 0 once by Create and read nowhere in â€” dead field, the same pa
    char *sequenceText[4];
    char *stepClearedText;
    u16 sequenceButtons[4][20];
    u16 sequenceLengths[4];
    u8 _pad1e8[0x4];
    u16 beatIndex;
    s32 beatElapsedMs;
    s32 beatTimeMs;                                // timestamp of the current beat; SetState cases 2, 3 AND 4 set it to g_gameTimeMs
    s32 pressTimeMs;
    s32 pressLateMs;
    s32 pressLateMinus250Ms;                       // pressTimeMs - (beatTimeMs+250). Its two tests are unrea
    u32 beatPeriod;
    s32 danceClock;                                // round clock in g_dt units; advanced only while eventCode == 0 AND !Game_IsPaused AND !Map_IsOpen
    DancingGhost *nearestGhost;
    DancingGhost *secondNearestGhost;
    s32 nearestDistance;
    u8 state;
    u16 screenWidth;
    u16 screenHeight;
    u16 cellW;                                     // spriteCross.widthMinus1*2 + 2, i.e. exactly twice the cross sprite's width (Sprite.csv +8 stores wid
    u16 cellH;                                     // spriteCross.heightMinus1 + 1, i.e. exactly the cross sprite's height
    u16 trackY;
    u16 trackWidth;
    u16 trackHeight;
    u16 trackX;
    s32 noteLeft;
    s32 noteRight;
    s32 noteTop;
    s32 noteBottom;
    u16 clearLineHeight;
    u16 clearBoxY;
    u16 clearBoxWidth;
    u16 clearBoxHeight;
    u16 clearBoxX;
    u16 stepClearedLen;
    s16 clearTextY;
    Box *cineBox;
    char *cineText;
    u32 cineId;
    u32 cineFlags;
    s32 cineTriggered;
    s32 cineDone;
    ScnObject *escapedSheep;
    PrayingGhost *prayingGhosts[4];
    u16 prayingCount;
    DancingGhost *dancers[8];
    DancingGhost *tambourineGhosts[8];
    u16 dancerCount;
    u16 tambourineCount;                           // entries in tambourineGhosts; the store is ( is the preceding load)
    s16 stepIndex;
    Box *stepBoxes[4];
    Box *ringBox;
    Box *holeBox;
    Box *safeBox;
    Box *danceBox;
    Box *boxRemove;                                // FILLED IN: PROPERTY_..._BOX_REMOVE (prop 0xc) â€” the box the escaped sheep must stand in for the tear
    Vec3s stepCenter;
    Vec3s ghostSpot;
};

struct StringBank {
public:
    u16 listCount;                                 // number of string lists; list 0 is the UI list, list 1+classId belongs to scenaric class classId. Ins
    char **lists;                                  // array of pointers to string lists; each list is a u8 count followed by that many NUL-terminated stri
};

struct WarFile {
public:
    WarHeader *header;                             // == blob: start of the in-memory WAR image. Read as the WarHeader (+4 version, +8 colour, +0xc resour
    u32 *table;                                    // blob + 0x10: resource table of u32 {type<<24 | byte offset}. The same array GetResourceType/Install_
    u8 *blob;                                      // malloc(fileSize) buffer. File bytes 4.. are read to blob+4, so blob offsets equal file offsets; blob
};

struct Dav {
public:
    DavHeader *header;                             // g_pDav->header == blob after Load_DAV; +0 'VDX7' magic, +0x14 relocated pointer to the DAV directory
    u8 *blob;                                      // malloc'd whole .DAV image (size from directory+0xe); freed by Dav_Free
    WarFile war;                                   // the embedded WarFile {header, table, blob}: Load_WAR/Load_WarMeshes/Load_FreeWAR take &g_pDav->war (
    StringBank strings;                            // the level's .MLT string bank (StringBank: listCount +0x14, lists +0x18; lists[0] is the single mallo
};

struct DavBitmapRec {
public:
    u16 u;                                         // texel x origin on the page. Tex_CornerUV base for the horizontal corners in MCard_DrawSlotIcons.
    u16 width;                                     // width in texels; every reader subtracts 1 (MCard_Init, MCard_DrawSlotIcons, AnimSprite/Sprite loader
    u16 v;                                         // texel y origin on the page
    u16 height;                                    // height in texels (readers subtract 1)
    u16 page;                                      // texture page, returned by TexAtlas_GetPage. 10-byte records in the table at DAV directory+0xa.
};

struct DavHeader {
public:
    char magic[4];                                 // 'VDX7': Load_DAV strncmp's 4 bytes against sprintf('VDX7')
    u8 _pad004[0x10];
    SDW_DAVPTR(DavDirectory) dir;                             // file offset of the DAV directory, relocated to a pointer by Load_DAV ( on the 0x44-byte prob
};

struct DefusableMineBits {
public:
    u8 hit : 1;                                    // (bits) bit 0 (1-byte unsigned unit)
    u8 failed : 1;                                 // (bits) bit 1 (1-byte unsigned unit)
    u8 wolfFrozen : 1;                             // (bits) bit 2 (1-byte unsigned unit)
    u8 played : 1;                                 // (bits) bit 3 (1-byte unsigned unit)
};

class InlineEmitter1 {
public:
#ifdef SDW_MEMBERS_InlineEmitter1
    SDW_MEMBERS_InlineEmitter1
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[1];                              // 1 x 0xC position records
    Particle particleBuf[1];                       // 1 x 8-byte particle records
};

struct MineFlagBits {
public:
    u8 exploded : 1;                               // (bits) bit 0 (1-byte unsigned unit) | Byte loads and stores at 4d4f4c/4d5127 prove bit 0 of +0xa7.
};

class Mine : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Mine_Init (override)
    virtual void Update();                         // Mine_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Mine_HandleMessage (override)
    virtual void Reset();                          // Mine_Reset (override)
    virtual void SetState(u8 state);               // Mine_SetState
    ScnObject *FindTrigger(u16 radius);
    ScnObject *InitModels(void *record);
    AltModel normalModel;                          // Main-model snapshot (ScnObject_InitWithAltModels outMain), restored by Mine_Reset when the exploded
    AltModel explodedModel;                        // Alternate model from id-list 3 (the shared explosion model, same list as Dynamite); swapped in by Mi
    u32 deadline;                                  // g_gameTime deadline. SetState(1): now + 0x5000 (5 s). SetState(3): now + 0x2aa (0.1665 s). GroundMin
    Vec3s savedPos;                                // Position after SnapToGround in Mine_Init; msg 0x13 refreshes it while in world; Mine_Reset restores
    u8 state;                                      // MineState (0-5 shared, 6-8 DefusableMine only)
    MineFlagBits mineFlags;                        // bit 0 = explodedModel is shown (set by SetState(4), cleared by Mine_Init/Mine_Reset); DefusableMine
};

class DefusableMine : public Mine {
public:
    virtual void PostLoadInit();                   // DefusableMine_Init (override)
    virtual void Update();                         // DefusableMine_Update (override)
    virtual void Render(Camera *view);             // DefusableMine_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DefusableMine_HandleMessage (override)
    virtual void Reset();                          // DefusableMine_Reset (override)
    virtual void SetState(u8 state);               // DefusableMine_SetState (override)
    void DrawBeatLights();
    void DrawGlyph(s32 glyph);
    void DrawPanel();
    InlineEmitter1 ringFx;                         // 1-slot ground ring pulse (Emitter_UpdateFade with g_defusableMineRingFxParams) spawned only in state
    u32 phaseOffset;                               // 0x3000 - ((CODEINIT<<12)/100 % 0x3000); phase = (g_gameTime + phaseOffset) % 0x3000. CODEINIT is in
    u32 code;                                      // CODE: packed 2-bit DefuseSymbols, symbol i = (code >> (2i & 31)) & 3, first symbol in the low bits
    u32 seqIndex;                                  // current step of the playback/input sequence (reset by SetState(7/8))
    u32 codeLength;                                // CODELENGTH (2-4 in shipped levels); compared unsigned
    u32 stepStart;                                 // g_gameTime when the current step began; beat = stepStart + 0x1000
    u16 finalSound;                                // handle of sound 0x84 played in the final playback step's 2 s post-wait; stopped by SetState
    u8 dormant;                                    // 1 while phase < 0x1800 (first 1.5 s of each 3 s cycle): no proximity check and the DEFUSE prompt is
    u8 lightsShown;                                // last beat-light count that played the 0x7e tick; 0xff at each step start
    DefusableMineBits dmFlags;                     // DefusableMineFlags
};

struct DialogueShownBits {
public:
    u8 shown : 1;                                  // (bits) g_dialogueShownFlags bit 0 as a bitfield VIEW of that u8 (the ProgressRuntimeFlagBits prece
};

struct DialogueShownFlags {
public:
    union {
        u8 all;                                        // g_dialogueShownFlags as the plain byte: Cine_Update reads it as & 1
        DialogueShownBits bits;                    // the same byte as the DialogueShownBits view: Dialogue_Show writes bit 0 with byte bitfield code (0x5
    };
};

class Diamond : public ScnBody {
public:
    virtual void PostLoadInit();                   // Diamond_PostLoadInit (override)
    virtual void Update();                         // Diamond_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Diamond_HandleMessage (override)
    virtual void Reset();                          // Diamond_Reset (override)
    void SetState(u8 newState);
    void TeleportZone();
    u16 soundHandle;                               // Sound_Play channel: 0x95 while the gem slides, 0x8f once it is seated; Update
    u8 pad066[2];                                  // alignment before the box pointer
    Box *zone;                                     // ZONE property (Scn_GetPropBox prop 8); the rectangle whose objects the seated gem
    u8 unk06c[32];                                 // never read or written by the class
    Trajectory *traj;                              // TRAJ property (Scn_GetPropTrajectory prop 4); only its first and last points are
    u8 sliding : 1;                                // (bits) bit 0 of the byte at +0x90: set while a non-zero turn arrives (or al,1), cleared by PostLoa
    Vec3s startPos;                                // traj->pts[0]; the gem interpolates from here
    Vec3s endPos;                                  // traj->pts[count-1]; SetPosition target once progress reaches 0xc00
    Vec3s delta;                                   // endPos - startPos, per axis
    s16 progress;                                  // sum of the signed turn amounts of msg 0x42; the gem is seated at 0xc00
    s32 percent;                                   // progress * 100 / 0xc00; the interpolation weight, in percent
    Vec3s homePos;                                 // pos at load with the vertical replaced by endPos.y; Reset puts the gem back here
    u8 pad0b2[2];                                  // alignment after the home position
    s32 teleported;                                // 0 at load, 1 once the zone has been teleported; never read by this class
    u8 state;                                      // 0 loose, 1 seated; compared and stored by SetState and tested by HandleMessage (
    s16 teleportDx;                                // width in x of BOXTELEPORT (max.x - min.x): the offset every object in the zone is
};

class DoorLevel : public ScnBody {
public:
    virtual void PostLoadInit();                   // DoorLevel_PostLoadInit (override)
    virtual void Update();                         // DoorLevel_Update (override)
    virtual void Render(Camera *view);             // DoorLevel_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DoorLevel_HandleMessage (override)
    virtual void Reset();                          // DoorLevel_Reset (override)
    const char *GetLevelLabel(s8 level);
    Box *backBox;                                  // PROPERTY_DOORLEVEL_IDBOXBACK (prop 0); standing in it means Ralph approaches the door from the back,
    s8 state;                                      // 0 uninitialised (Update starts the idle animation and moves to 1), 1 idle, 3 walk-through requested
    s8 levelNumber;                                // PROPERTY_DOORLEVEL_LEVELNUMBER (prop 4), a disc level index 0..17 indexed into g_levelLabels. It is
    s8 enteredFromBack;                            // Set by MSG_QUERY_ACTION when Ralph stands inside backBox; with it set AND levelNumber non-zero, Upda
    s8 wolfFacingDoor;                             // Set by MSG_USE to 1 when the heading from the door to Ralph is within 0x400 (90 degrees) of the door
};

class DoorMechanism : public ScnMobile {
public:
    virtual void PostLoadInit();                   // DoorMechanism_PostLoadInit (override)
    virtual void Update();                         // DoorMechanism_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DoorMechanism_HandleMessage (override)
    u32 running;                                   // 1 while the driven platform is moving: the class's only field and its only behaviour is to advance a
};

class DoorWorld : public ScnBody {
public:
    virtual void PostLoadInit();                   // DoorWorld_PostLoadInit (override)
    virtual void Update();                         // DoorWorld_Update (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // DoorWorld_CustomCollide (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // DoorWorld_HandleMessage (override)
    virtual void Reset();                          // DoorWorld_Reset (override)
    CollBox *triggerBox;                           // One box out of the model's own box list (model+0xc = {u32 count; CollBox[]}): box[1] when the object
    ScnObject *doorLevel;                          // PROPERTY_DOORWORLD_IDDOORLEVEL (prop 4): the DoorLevel (class 170) this world-map door belongs to. Q
    CamSetup *openCamera;                          // PROPERTY_DOORWORLD_CAMERAOPEN (prop 0) via Scn_GetPropCamera: the camera setup record played while t
    s16 timer;                                     // Millisecond accumulator used twice: state 3 waits past 1000 ms before starting the open animation, s
    s8 useOpenSequence;                            // 1 when the open request arrived and the door has an openCamera, so states 2-5 play the full camera p
    u8 state;                                      // 0 decide from the DoorLevel answer, 1 inert, 2 start the open camera, 3 wait 1 s then play anim 1, 4
};

class Dragon : public ScnMobile {
public:
#ifdef SDW_MEMBERS_Dragon
    SDW_MEMBERS_Dragon
#endif
    virtual void PostLoadInit();                   // Dragon_Init (override)
    virtual void Update();                         // Dragon_Update (override)
    virtual void Render(Camera *view);             // Dragon_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Dragon_HandleMessage (override)
    virtual void Reset();                          // Dragon_Reset (override)
    s16 NegSign(s16);
    s32 ApproachSphere();
    s32 CheckWolfInRockZone();
    s32 IsWolfInFireReach();
    s32 IsWolfNear();
    s32 ShouldFly();
    s32 UpdateSkid();
    s32 VerticalMove(s32);
    void BackAwayFromWolf();
    void CalcAnglesTo(Vec3s *, Vec3s *, Vec3s *, s32);
    void ChooseState();
    void FaceTarget(Vec3s *);
    void MoveToTarget(s32);
    void ScaleVector(Vec3s *, s32, s32, s32, s32, s32);
    void SetState(u8);
    void StartSkid(Vec3s *);
    void TryMoveInZone(Vec3s *);
    void UpdateDust();
    void UpdateState();
    Box *boxFirstContact;                          // Scn_GetPropBox(BOXFIRSTCONTACT). The Wolf's x/z inside it ends state 0x19 (first-contact cinematic).
    u32 flagFirstContact;                          // PROPERTY_DRAGON_FLAGFIRSTCONTACT, Cine_Start flags for state 0x1a (42 in Lvl-09).
    u16 cineFirstContact;                          // PROPERTY_DRAGON_CINEFIRSTCONTACT, cine id for state 0x1a (0 in Lvl-09).
    u8 _pad086[0x6];
    u32 flagChronoBall;                            // PROPERTY_DRAGON_FLAGCHRONOBALL, Cine_Start flags for state 0x20 (2090).
    u16 cineChronoBall;                            // PROPERTY_DRAGON_CINECHRONOBALL, cine id for state 0x20 (1).
    s32 wolfDistXZ;                                // Vec3s_DistXZ(pos, Wolf pos), computed at the start of every Update. Used for all range tests (300/35
    s32 runLatch;                                  // Cleared by every SetState. State 7 sets it to the Wolf's running flag (run anim 2, speed 1200, dust)
    s32 idleLoopsLeft;                             // Anim loops left in idle states 0xd (1..3) and 0xf (1..12).
    Box *flyBoxes[5];                              // Scn_GetPropBox(BOXFLY1..BOXFLY5). The dragon inside any of them makes ShouldFly true. All 0 in Lvl-0
    Box *zone;                                     // Scn_GetPropBox(ZONE): the leash. TryMoveInZone and UpdateSkid drop any move ending outside it (3-D).
    Box *rockZone;                                 // Scn_GetPropBox(ROCKZONE). The Wolf or the dragon in it (x/z) triggers flight; the Wolf in it at >= 1
    Vec3s rockZoneTarget;                          // (centre x, min.y = top, centre z) of ROCKZONE; target of state 0x14 (horizontal only). (6801,-414,-1
    u8 _pad0c2[0x2];
    s16 flyHeight;                                 // Height climbed since take-off (0 at SetState(0x11)). The rise stops at >= 160; the descent stops bel
    u32 riseAnimMs;                                // Anim_GetDurationMs(anim 0x11). The climb covers 160 units over this duration.
    u32 descendAnimMs;                             // Anim_GetDurationMs(anim 0x14). The descent covers 160 units over this duration.
    s32 flyToRockZone;                             // Set when the Wolf is in ROCKZONE far away, or a flying dragon is outside ROCKZONE while the Wolf is
    s32 landTimerMs;                               // -1 = idle; set to 5000 on the first frame the dragon flies without a reason (ShouldFly false); -= g_
    InlineEmitter16 dustEmitter;                   // dust emitter with inline storage (0xd8..0x23c): the factory forms it, slotPool = +0x24 (
    EmitterDriftParams dustParams;                 // Emitter_UpdateDrift params {s32 100, -20, 0xa30, 0x146; u16 sizeStart 40 (+0x24c), sizeEnd 120 (+0x2
    Vec3s skidStep;                                // Last frame step kept when a skid starts; halved each frame by UpdateSkid.
    Vec3s skidDirSign;                             // NegSign of each skidStep component, written by StartSkid and never read (dead).
    Vec3s sphereTarget;                            // Time-machine sphere position from msg 0x38. Face and approach target in states 0x15 and 0x1b.
    Vec3s arrivePos;                               // Arrival point from msg 0x3a, passed to SetPosition.
    s32 sphereDist;                                // Vec3s_Dist(pos, sphereTarget) when msg 0x38 arrived. Acceptance test (499/999) and ApproachSphere di
    u8 state;                                      // DragonState. SetState returns early if unchanged; Init sets the sentinel 0x26 first.
    u8 savedState;                                 // State saved by the msg 0x38 wait branch; restored by state 0x24 only (0x23 always resumes in 0).
};

struct DrawMsgArgs {
public:
    Camera *view;                                  // render view, argument 1 of ScnBody_RenderTinted
    u32 color;                                     // tint colour, argument 2; its three low bytes are halved while fxFlags & 0
    u16 amount;                                    // tint amount, argument 3; replaced by 0x1000 while fxFlags & 0x20000
    u16 mode;                                      // argument 4
};

struct DynamiteFlagBits {
public:
    u8 exploded : 1;                               // (bits) bit 0 (1-byte unsigned unit)
};

class InlineEmitter3 {
public:
#ifdef SDW_MEMBERS_InlineEmitter3
    SDW_MEMBERS_InlineEmitter3
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[3];                              // 3 x 0xC position records
    Particle particleBuf[3];                       // 3 x 8-byte particle records
};

class Dynamite : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Dynamite_Init (override)
    virtual void Update();                         // Dynamite_Update (override)
    virtual void Render(Camera *view);             // Dynamite_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Dynamite_HandleMessage (override)
    virtual void Reset();                          // Dynamite_Reset (override)
    void SetState(u8);
    InlineEmitter3 emitter;                        // 3-slot inline emitter; factory takes its address and calls Emitter_Reset 0x52db
    s32 fuse;                                      // remaining fuse in 1/4096 s ticks; decremented by g_dt while lit (state 3)
    s32 stateTimestamp;                            // g_gameTime at the last state change (respawn wait 0x3000 ticks)
    s32 fuseTotal;                                 // (COUNTDOWN ms << 12) / 1000
    AltModel normalModel;                          // model before the blast; its address is passed to InitWithAltModels
    AltModel explodedModel;                        // model after the blast; its address is passed to InitWithAltModels
    Vec3s homePos;                                 // spawn position; its address is taken and (Init / Reset / Ha
    u16 soundHandle;                               // stored Sound_Play handle: word reads (the Sound_Stop path), word writes at 0x4b
    u8 state;                                      // 0 idle, 3 lit, 4 exploding, 5 hidden waiting 3 s, 6 fading back in
    DynamiteFlagBits dynFlags;                     // bit 0 set while the explosion model is shown
};

class ElasticTree : public ScnLogic {
public:
    virtual void PostLoadInit();                   // ElasticTree_Init (override)
    virtual void Update();                         // ElasticTree_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // ElasticTree_HandleMessage (override)
    virtual void Reset();                          // ElasticTree_Reset (override)
    Vec3s *pullUpPoint;                            // Scn_GetPropBox(IDPOINTUP)+4 = &box.min; handed to the elastic by msg 0x1a80
    ScnObject *camRestrict;                        // IDCAMREST CameraRestriction; switched off on the first Update
    u8 unk48;                                      // zeroed by Init; no reader found
};

class Elmer : public ScnMobile {
public:
#ifdef SDW_MEMBERS_Elmer
    SDW_MEMBERS_Elmer
#endif
    virtual void PostLoadInit();                   // Elmer_PostLoadInit (override)
    virtual void Update();                         // Elmer_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Elmer_HandleMessage (override)
    virtual void Reset();                          // Elmer_Reset (override)
    s32 MoveToward(Vec3s *target, s32 canRefuse);                /* Elmer_MoveToward */
    void SetState(u8 newState);                                  /* Elmer_SetState */
    u8 state;                                      // ElmerState 0..15; written at the end of Elmer_SetState and switched on by Update
    char *rabbitText;                              // RABBITTEXT (property 24) localised line via Text_GetClassString; spoken when the swirl si
    char *daffyText;                               // DAFFYTEXT (property 8) localised line via Text_GetClassString; spoken when the sign says
    s16 speed;                                     // walk speed in units/s: 0x1f4 at load and while Ralph is not the rabbit, 0x3e8 once he is ( /0
    s16 waitTimerMs;                               // countdown in ms for state 8: 0xfa0 on entering state 8, 0x1f4 from msg 0x76 (0
    u8 unk08c[8];                                  // not touched by this class
    s32 heading;                                   // 12-bit bearing toward the move target computed by Elmer_MoveToward and used as the sin/co
    s32 shootTimerMs;                              // -1 at load; set to 0x400 when Elmer is inside the moving box in state 14; decremented by
    u8 unk09c[4];                                  // not touched by this class
    s32 sweepRate;                                 // 0x1000 / (anim 0 duration in ms / 2): the phase rate that sweeps the gun barrel in state
    u16 sweepPhase;                                // 12-bit phase accumulated from g_dtMs in state 5, reset to 0 when the animation ends
    s32 restart;                                   // 1 = the trajectory follower must pick its nearest waypoint again when state 5 is entered;
    s32 wolfFrozen;                                // result of Wolf msg 0xe / 0xf (freeze / unfreeze during the shooting sequence):;
    s32 unk0b0;                                    // zeroed by PostLoadInit and never read
    u8 unk0b4[4];                                  // not touched by this class
    s32 wolfSpotted;                               // Ralph is inside DETECTBOX and answers msg 0x410 and the swirl sign answers msg 0x3080: re
    s32 canAmbush;                                 // set when the XYZ distance to Ralph passes 0x15f90 and cleared on Reset and on entering st
    s32 bulletBusy;                                // result of Bullet msg 0x3201 (bullet state != 5) every frame
    s32 wolfIsRabbit;                              // cached Wolf msg 0x40c (wearing the rabbit costume) read in Elmer_MoveToward; a change swi
    s32 wolfFar;                                   // distance squared to Ralph > 0x31704
    s32 wolfNear;                                  // distance squared to Ralph < 0x62e08
    s32 selfInBox;                                 // Elmer's own position is inside MOVINGBOX; forced to 1 while shootTimerMs >= 0
    s32 wolfInBox;                                 // Ralph is inside MOVINGBOX
    Vec3s step;                                    // this frame's movement, the input and output of Collide_ResolveMove
    u8 unk0de[6];                                  // not touched by this class
    Vec3s wolfPos;                                 // copy of g_pWolf->pos taken at the top of every Update
    Vec3s homePos;                                 // TREESECTION's position, ground-snapped at load: where Elmer stands and what he walks back
    Box *movingBox;                                // MOVINGBOX (property 20): the patrol area
    Box *detectBox;                                // DETECTBOX (property 12)
    Box **exceptBoxes;                             // EXCEPTMOVINGBOX (property 16) id list via Scn_FindIdList: holes inside the moving box that
    u16 exceptBoxCount;                            // count written by Scn_FindIdList and the loop bound
    u8 unk0fe[2];                                  // alignment before the ContactInfo
    ContactInfo contact;                           // Collide_ResolveMove output
    ScnObject *blocker;                            // contact.wallObj kept when the state-5 walk is blocked; state 6 waits until it is 0x3840 a
    ScnObject *swirlSign;                          // SWIRLSIGN (property 28); asked msg 0x3080 for which season the sign shows
    ScnObject *treeSection;                        // TREESECTION (property 36); its position is Elmer's home and it is told msg 0x3181 when he s
    ScnObject *bullet;                             // BULLET (property 0)
    ScnObject *daffy;                              // DaffyLevel09 (class 104), found by Scenaric_FindByClass; never read by this class
    CamSetup *camera;                              // CAMERA (property 4): {focal; rotX; rotY; rotZ; Vec3s pos} used by Camera_StartScripted (0x4
    Trajectory *trajectory;                        // TRAJECTORY (property 32): u16 point count then Vec3s points
    TrajFollower follower;                         // initialised with speed 0x50, heading bias 0x800, continuous heading, 3D distance and arrive radius 0
    s16 waypoint;                                  // the follower's waypoint index that this class drives itself: stepped by waypointDir when the followe
    s16 waypointDir;                               // +1 or -1; negated every time the 1000 ms patrol pause expires ( ...) so Elmer walks
    s32 pauseTimerMs;                              // 1000 ms pause between patrol waypoints; counts down by g_dtMs
    s32 turnRate;                                  // Math_ApproachAngle angular-velocity accumulator
    s16 newFacing;                                 // Math_ApproachAngle result stored before it is copied into rot.y
    u16 soundHandle;                               // Sound_Play handle of the walk sound 0x100; stopped at the top of Elmer_SetState (
};

struct EmitterColumnParams {
public:
    s32 riseSpeed;                                 // vertical speed per second
    s32 life;                                      // particle life in ticks
    s32 period;
    s16 sizeStart;                                 // size at birth
    s16 sizeEnd;                                   // size at death
    u8 sheetIndex;                                 // sprite sheet
};

struct EmitterFadeParams {
public:
    s32 life;                                      // particle life in ticks
    s32 fadeStart;                                 // age at which the fade to 0 begins (comment of Emitter_UpdateFade; read later in it)
    s32 spawnInterval;                             // ticks between spawns; Wolf_UpdateTrailFx sets 0x4b000 / speed
    s16 sizeStart;                                 // size at birth
    s16 sizeEnd;                                   // size at death
    u8 sheetIndex;                                 // sprite sheet
};

struct EmitterPerfumeParams {
public:
    s32 hSpeed;
    s32 vSpeed;                                    // vertical drift per second
    s32 life;                                      // particle life in ticks
    s32 spawnInterval;                             // ticks between spawns
    s16 sizeStart;                                 // size at birth
    s16 sizeEnd;                                   // size at death
    s16 range;                                     // vertical distance from the reference height over which the drift fades to 0: strength = (range - |re
    u8 sheetIndex;                                 // sprite sheet
};

struct EmitterTrailParams {
public:
    s32 life;                                      // particle life in ticks, 0x7fffffff = permanent
    s32 param4;                                    // read; 150 in both of the Wolf's footprint blocks; meaning not read
    u16 size;                                      // particle size
    u8 sheetIndex;                                 // sprite sheet
};

class FacingCamera : public ScnLogic {
public:
    virtual void PostLoadInit();                   // FacingCamera_PostLoadInit (override)
    virtual void Render(Camera *view);             // FacingCamera_Render (override)
};

class FallingGate : public ScnBody {
public:
    virtual void PostLoadInit();                   // FallingGate_Init (override)
    virtual void Update();                         // FallingGate_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FallingGate_HandleMessage (override)
    void FinishMove();
    AltModel mainModel;                            // base model slot filled by ScnObject_InitWithAltModels in FallingGate_Create
    AltModel gateModels[5];                        // WAR_IDO_AGRILL01..05 (id list: 147,150,148,161,198); indexed by modelIndex
    u32 modelIndex;                                // TYPE decoded to 0..4 (after the 10 is removed); indexes gateModels and g_fallingGateAnims
    u8 state;                                      // FallingGateState
    s32 closedAtStart;                             // CLOSED property, but only when a Train (class 0x87) exists; otherwise always 1
    s32 closed;                                    // current state (1 closed); the reply to msg 0x72
    u8 _pad0d4[0x4];
    s32 busy;                                      // 1 while moving; switch messages are ignored. Never cleared after a single-activation move, which is
    s32 onlyOneActivation;                         // ONLYONEACTIVATION property
    s32 wolfFrozen;                                // the gate froze the Wolf for its camera shot; msg 0xf is sent when the move ends
    s32 cameraActive;                              // a scripted camera shot started by msg 0x1f is running
    s32 cutCamera;                                 // CUTCAMERA property: nonzero â†’ CamScriptFlags 2 (cut in, blend out); 0 â†’ 3 (blend in and out)
    s32 cameraReleased;                            // 1 at Init and 0 when a shot starts; set to 1 only in a compiled-out if(0) branch
    s32 keepCamera;                                // TYPE >= 10: the camera is not released at the end of the move, only by msg 0x20
    CamSetup *camera;                              // IDCAMERA setup {u16 focal; u16 rot[3]; Vec3s pos}
    CollBox *solidBox;                             // ScnObject_GetFirstSolidBox; the crush test region once translated to pos
    s32 retriggered;                               // set when msg 0x1f arrives while busy or already toggled; cleared by msg 0x20; suppresses a new camer
    u32 unk100;                                    // zeroed by Init; no reader found
};

class FallingGate2 : public ScnBody {
public:
#ifdef SDW_MEMBERS_FallingGate2
    SDW_MEMBERS_FallingGate2
#endif
    virtual void PostLoadInit();                   // FallingGate2_PostLoadInit (override)
    virtual void Update();                         // FallingGate2_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FallingGate2_HandleMessage (override)
    virtual void Reset();                          // FallingGate2_Reset (override)
    void SetState(u8);
    u8 gateState;                                  // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 riseTimer;                                 // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 fallingTimeMs;                             // PROPERTY FALLINGTIME (prop 0xc), used as the DIVISOR of the fall's playback speed â€” takes o
    u32 openAnimMs;                                // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    u32 riseAnimMs;                                // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 closed;                                    // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 busy;                                      // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 oneShot;                                   // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 wolfFrozen;                                // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 boxRaised;                                 // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 firstSenderOn;                             // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    s32 singleButton;                              // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    CamSetup *camera;                              // the camera property (Scn_GetPropCamera(record, 4) in PostLoadInit); when set, the opening state star
    CollBox *solidBox;                             // first Box of the SHARED model's box list ( [this+8]->+0xc, +4 past the count). The gate edit
    CollBox worldBox;                              // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    ScnObject *firstSender;                        // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
    u16 *storedRecord;                             // named from its uses in src/objects/fallinggate2.cpp (the name and access width come from the code th
};

class FallingRock : public ScnBody {
public:
    virtual void PostLoadInit();                   // FallingRock_PostLoadInit (override)
    virtual void Update();                         // FallingRock_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FallingRock_HandleMessage (override)
    void Trigger();
    void Unused_Nop();
    u8 state;                                      // 0 armed/waiting for the Wolf, 1 falling, 2 landed (camera hold), 3 finished. Set to 3 at init when t
    CamSetup *camRecord;                           // PROPERTY_FALLINGROCK_CAMERA (4): the camera setup {u16 focal; u16 rot[3]; Vec3s pos} used for the sc
    Box *activationBox;                            // PROPERTY_FALLINGROCK_ACTIVATIONBOX (0). Triggers on the Wolf's x and z only â€” the vertical pair (+6/
    u32 camHoldMs;                                 // How long the scripted camera is held after landing. Hard-coded to 500; the designers' PR
    u32 camElapsedMs;                              // Milliseconds accumulated (g_dtMs) since the rock landed, compared against camHoldMs to end the shot.
    u16 fallTicksLeft;                             // Game-clock ticks until impact: five reference steps of descent, then one before impact.
    u16 impactSoundHandle;                         // Sound_Play handle for the impact sound (id 0x44). Zeroed at init and stored on landing, but never re
};

struct FanReport {
public:
    u16 classId;                                   // class id of the reporting object
    u16 proximity;                                 // its proximity value
};

class Fan : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Fan_Init (override)
    virtual void Update();                         // Fan_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Fan_HandleMessage (override)
    virtual void Reset();                          // Fan_Reset (override)
    void Blow(s16);
    void SetState(u8, s32);
    void StopSound();
    void UpdateSound();
    Vec3s homePos;                                 // spawn position: Init after the ground snap, msg 9 arg 1 (container release); Reset returns here when
    u16 sound;                                     // handle of the whirr: 0x6b one-shot start, then 0x6c loop (Fan_UpdateSound); stopped by Fan_StopSound
    FanReport lastReport;                          // FanReport: classId = class id of the nearest attracted object relayed by msg 0x34 (the Perfume forwa
    u32 reportTime;                                // g_gameTime of the last stored report; msg 6 answers only while g_gameTime - reportTime <= 0x800 (0.5
    u8 state;                                      // FanState, also the index into g_fanStateAnims
    u8 soundStarted;                               // 0 = the next Fan_UpdateSound plays the start whirr 0x6b; 1 = the start was played, keep the 0x6c loo
    u16 pad8e;                                     // tail padding to sizeof 0x90; never accessed
};

struct FileHandle {
public:
    s32 fd;                                        // _open handle; File_Open returns -1 when it is -1
    s32 size;                                      // file size from lseek(end) in File_Open (also returned)
    u32 remaining;
    u32 field_0c;                                  // never read or written by File_Open/Read/ReadAt/Seek/ReadChecked/Close; INFERRED from the frames: Loa
};

class FireBall : public ScnBody {
public:
    virtual void PostLoadInit();                   // FireBall_PostLoadInit (override)
    virtual void Update();                         // FireBall_Update (override)
    virtual void Render(Camera *view);             // FireBall_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FireBall_HandleMessage (override)
    virtual void Reset();                          // FireBall_Reset (override)
    void RenderFlames(Camera *view);
    void SetState(u8);
    u8 state;                                      // FireBall_SetState's state: 0 = hidden, waiting for msg 0x5981 (a victim to burn); 1 = burning over t
    u8 flameTick;
    Vec3s flameOffsets[3];                         // offsets of the three flame sprites from pos, re-rolled on entering state 1 and at each wrap of flame
    AnimSprite flameSprite;                        // the flame sprite sheet (resource 0xd0, AnimSprite_InitFromRes in PostLoadInit)
};

class Firefly : public ScnBody {
public:
    virtual void PostLoadInit();                   // Firefly_PostLoadInit (override)
    virtual void Update();                         // Firefly_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Firefly_HandleMessage (override)
    u8 _pad064[0x8];
    s32 restartPending;                            // Set to 1 when the blink cycle ends and the random pause starts; its Update arm restarts
    s32 playingAnim0;                              // Set when animation 0 has been started; while set, each reported finish restarts animation 1 â€” the fl
    s32 playingAnim1;                              // Set when animation 1 has been started; its branch starts animation 2 on finish and clears
    s32 playingAnim2;                              // Set when animation 2 has been started; its branch ends the cycle by arming a new pause.
    s32 pauseMs;                                   // Countdown in ms between blinks, seeded Rand_Range(1000,3000). While it is above zero (signed jg at 0
    u8 _pad080[0x10];
    u8 anim1Repeat;                                // Times animation 1 has been replayed this blink. At 2 the loop ends and the counter is reset to 0 at
    u8 pad091[3];                                  // Alignment tail: obj_size is 0x94 and the last field is the u8 at 0x90. Added so the size assert in w
};

class Fish : public ScnBody {
public:
    virtual void PostLoadInit();                   // Fish_Init (override)
    virtual void Update();                         // Fish_Update (override)
    virtual void Render(Camera *view);             // Fish_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Fish_HandleMessage (override)
    void SetState(u8 state);
    Vec3s homePos;                                 // placed position; the fish teleports back here in state 0
    Vec3s swimVel;                                 // 250 u/s along the facing-quadrant axis
    Vec3s frameStep;                               // Vec3s_ScaleByDt(swimVel) this frame
    Vec3s leapOffset;                              // 489 along the same axis; applied when leap anim 2 ends
    u8 state;                                      // FishState
    u8 offscreenTicks;                             // not-drawn updates in state 0; swims again at > 1
    s32 disappearDistSq;                           // DISAPPEAR_DIST squared; XZ distance from home that ends a swim
    u32 wasOnScreen;                               // inst.flags & INST_F_DRAWN copied in Render, zeroed in Update
};

class InlineEmitter4 {
public:
#ifdef SDW_MEMBERS_InlineEmitter4
    SDW_MEMBERS_InlineEmitter4
#endif
    void RenderFlat(Camera *view, s32 forward);
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[4];                              // 4 x 0xC position records
    Particle particleBuf[4];                       // 4 x 8-byte particle records
};

class FloatingBox : public ScnMobile {
public:
#ifdef SDW_MEMBERS_FloatingBox
    SDW_MEMBERS_FloatingBox
#endif
    virtual void PostLoadInit();                   // FloatingBox_PostLoadInit (override)
    virtual void Update();                         // FloatingBox_Update (override)
    virtual void Render(Camera *view);             // FloatingBox_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FloatingBox_HandleMessage (override)
    virtual void Reset();                          // FloatingBox_Reset (override)
    void ReleaseContents();
    void SetState(u8 value, s32 effect);
    u32 boxFlags;                                  // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    u8 state;                                      // state 4 runs anim 0, started by the CALLER; SetState case 4 starts no animati
    Box *waterBox;                                 // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    CollBox worldBox;                              // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    ScnObject *overlapList[64];                    // in Update state 2 the loop skips every entry whose classId is not 0, so only th
    s32 overlapCount;                              // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    ScnObject *content;
    AltModel mainModel;                            // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    AltModel floatModel;                           // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    u8 _pad1c0[0x2];
    u16 hitClass;                                  // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    s32 bounceTime;                                // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s bounceVelocity;                          // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s savedRotation;                           // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s bounceRotation;                          // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    s16 pushSpeed;                                 // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    s16 pushAngle;                                 // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    u16 lastMoveResult;                            // u16, not s16: both writers store AX from Collide_ResolveMove (documented u16) and every read is movz
    ContactInfo contact;                           // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s frameMove;                               // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s velocity;                                // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    s32 restoreUnset;                              // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    u8 restoreState;                               // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s restorePos;                              // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s emitPos;                                 // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    Vec3s floatOffset;                             // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    InlineEmitter4 splashEmitter;                  // 4-slot inline emitter; the count byte is the emitter's own +0x21, i.e. this+0x241, not th
    EmitterFadeParams splashParams;                // PostLoadInit sets sheetIndex = 1, not 0 â€” SetState(1) is the one that sets 0 a
    u32 splashTime;                                // named from its uses in src/objects/floatingbox.cpp (the name and access width come from the code the
    s32 pushRemainder = 0;
};

struct FlockScentSource {
public:
    ScnObject *obj;                                // the scent source (Perfume bottle, etc.) registered by Flock_AddScentSource
    u32 rangeSq;                                   // squared attraction range
    s16 heading;                                   // heading of the scent cone
    u16 pad0a;                                     // padding to the 0xc-byte entry stride of g_flockScentSources
};

class Flute : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Flute_Init (override)
    virtual void Update();                         // Flute_Update (override)
    virtual void Render(Camera *view);             // Flute_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Flute_HandleMessage (override)
    virtual void Reset();                          // Flute_Reset (override)
    void SetState(u8 value);
    InlineEmitter16 noteFx;                        // 16-slot music-note emitter with inline storage: the factory sets slotPool = +0xa0, particles = +0x16
    ScnObject *sam;                                // first Sam (class 1) found by Scenaric_FindByClass at Init; receives msg 0x481 within 300 units; NULL
    Vec3s homePos;                                 // spawn position (Init; msg 9 arg 1); Reset returns here when IN_WORLD
    u16 playSound;                                 // handle of the tune 0x25 (LOOP|POSITIONAL|NO_RETRIGGER), started by msg 0x15; stopped by every Flute_
    u8 state;                                      // FluteState: 0 world, 1 held (silent; notes in flight age out), 2 playing
    u8 pad1ed[3];                                  // tail padding to sizeof 0x1f0
};

struct FmvList {
public:
    u32 count;                                     // number of clips; App_InitGameSystems writes 3 into g_fmvListIntro and 1 into g_fmvListCre
    char clips[8][256];                            // clip file names, 0x100 apart ( ; Video_PlaySequence i
};

class FogManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // FogManager_Init (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FogManager_HandleMessage (override)
    u8 _pad040[0x4];
    u32 levelFogColor;                             // g_pFrustrum->fogColor captured at init (the level's own fog)
    u32 fog2Color;                                 // FOG2_COLOR property; applied on time-machine msg 0x3B when TimeMachine_IsInPresent(sender) != 0
};

struct Font {
public:
    u32 color;                                     // current text colour, stored as rgb | 0x05000000
    u16 texPage;                                   // texture page index of the font sheet; Text_DrawGlyph writes texPage + 4 as the RenderPoly type, the
    u16 reserved;                                  // explicitly zeroed by Font_LoadFromRes and never read anywhere in the binary
    u8 cellW;                                      // width in TEXELS of one glyph cell on the sheet; the U span sampled per glyph (8 for the debug font,
    u8 cellH;                                      // height in TEXELS of one glyph cell (0x10 for both fonts); the V span and row stride
    u8 glyphWidth;                                 // advance width of one character in HUD units
    u8 glyphHeight;                                // on-screen glyph height in HUD units (0xF for both fonts); used for the vertical clip test and the qu
    u8 cellWShift;                                 // log2 of cellW (3 for the 8-wide debug font, 4 for the 16-wide game font). Written by Font_LoadFromRe
    u8 cellHShift;                                 // log2 of cellH (4 in both fonts). Written by Font_LoadFromRes, no reader found
    u8 cols;                                       // columns that fit in the current text window (windowW / glyphWidth); recomputed by Text_SetFont and T
    u8 rows;                                       // rows that fit in the current text window (windowH / lineHeight)
    u8 sheetU;                                     // U origin of the font sheet's sub-rect within the 256-pixel texture page; glyph U = sheetU + col*cell
    u8 sheetV;                                     // V origin of the font sheet; glyph V = sheetV + row*cellH
    u8 lineHeight;                                 // vertical advance per line; added to g_textCursorY on wrap and on '\n'
    u8 pad;                                        // tail padding of the 0x14-byte entry; never written or read
};

struct RiverCargo {
public:
    u16 node;                                      // index of the path point it moves toward (FrozenRiver_FindNextNode; ++)
    s32 moving;                                    // 1 while it moves toward node; 0 while another cargo holds that node (FrozenRiver_IsNodeTaken, 0x4c35
    s32 active;                                    // floe slots: 1 while the floe is in the water (FrozenRiver_LaunchFloe), 0 once the crane has
    ScnObject *obj;                                // the Wolf, the Sheep or the SlidingIceCube
    s32 waitingCrane;                              // 1 once the crane has been sent msg 0x37 for it at the last node; MoveCargo then does noth
    s32 done;                                      // 1 once the crane has taken it (msg 0x3e); FrozenRiver_UpdateCargo skips it
    s32 spinRemainder = 0;
};

class FrozenRiver : public ScnLogic {
public:
    virtual void PostLoadInit();                   // FrozenRiver_PostLoadInit (override)
    virtual void Update();                         // FrozenRiver_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // FrozenRiver_HandleMessage (override)
    virtual void Reset();                          // FrozenRiver_Reset (override)
    u16 FindNextNode(Vec3s *p);
    s32 AddCargo(ScnObject *obj);
    s32 IsNodeTaken(u16 node, RiverCargo *self);
    void UpdateCargo();
    void MoveCargo(RiverCargo *c);
    void SpinCargo(RiverCargo *c);
    void LaunchFloe();
    Trajectory *path;                              // Scn_GetPropTrajectory(IDWATERTRAJ): the course from the water to the crane; the last poin
    RiverCargo cargo[2];                           // [0] the Wolf, [1] a Sheep (FrozenRiver_AddCargo)
    RiverCargo floes[3];                           // the level's SlidingIceCubes (Scenaric_FindByClass(0x4f, 3))
    ScnObject *crane;                              // the level's Crane (Scenaric_FindByClass(0x52)); none disables the river. Sent ms
    u32 nodeDist;                                  // Vec3s_DistXZ from the moved cargo to its node, written by every FrozenRiver_MoveCargo; co
    u32 floeTimer;                                 // ms since the last floe launch (+= g_dtMs); a floe launches once it passes 10000 (
    u8 _pad0c8[0x4];
    ZoneList waterBoxes;                           // IDWATERBOX list (Scn_FindIdList): msg 0x36 registers a sender inside one of them (
    CamSetup *camera;                              // Scn_GetPropCamera(CAMERA): started once when the Wolf reaches the last point
    Vec3s floeStart;                               // where floes are put in: path point 0 with y = top of waterBoxes[0] + 110, or 1
    u8 floesActive;                                // floes in the water (LaunchFloe ++, -- when the crane takes one); FrozenRiver_Updat
    u8 floeCount;                                  // SlidingIceCubes found
    u8 enabled;                                    // 1 from PostLoadInit, 0 without a Crane; Update does nothing unless it is 1
    u8 floeOnCrane : 1;                            // (bits) set when the crane takes a floe (msg 0x3e), cleared by a SlidingIceCube's msg 0x36 (0x4c363
    u8 wolfInRiver : 1;                            // (bits) cargo[0] holds the Wolf; cleared when the crane takes him
    u8 sheepInRiver : 1;                           // (bits) cargo[1] holds a Sheep; cleared when the crane takes it
    u8 craneBusy : 1;                              // (bits) msg 0x3e's argument bit 0: the crane has its hook on a cargo; no crane call while set (0x
    u8 camStarted : 1;                             // (bits) the river camera was started for the Wolf at the last point; cleared by msg 0x3e (0x4c2f8
};

class Frustrum {
public:
#ifdef SDW_MEMBERS_Frustrum
    SDW_MEMBERS_Frustrum
#endif
    virtual ~Frustrum();                              // Frustrum_ScalarDeletingDtor
    void SetProjection(float nearZ, float farZ, float fovRad, float viewDistance);
    void SetFov(float fovRad);
    void SetFovFromTan(float tanHalfFov);
    void SetFogStartEnd(u8 enable, u32 color, s32 mode, float fogStart, float fogEnd);
    void SetFog(u8 enable, u32 color, s32 mode, float range);
    void SetFogColor(u32 color);
    void EnableFog(u8 enable);
    void GetFogRangeRaw(float *start, float *end);
    void GetFogRangeWorld(float *start, float *end);
    void BuildProjectionMatrix(Mat44 *dest);
    void SetViewDistance(float distance, u8 moveFog);
    void SetViewDistanceNormalized(float t, u8 moveFog);
    float GetViewDistanceNormalized();
    u32 GetFogValue(float z, u8 useRawRange);
    float NormalizeDepth(float z);
    float DenormalizeDepth(float t);
    void BuildFogRamp();
    D3DApp *d3dApp;                                // the D3DApp the frustum pushes render states to; its +0x28 is the IDirect3DDevice7
    float nearZ;                                   // near plane, 1.0f at construction
    float farZ;                                    // far plane, 100.0f at construction
    float fovRad;                                  // this is the HORIZONTAL field of view, not the vertical one. Screen_SetProjection sets tanHalfFov = 5
    float tanHalfFov;                              // tan(fovRad / 2.0) cached alongside the angle
    u8 cullDisabled;                               // When non-zero, Instance_UpdateVisibility marks every instance visible (
    u8 flipWinding;                                // inverts the signed-area facing test in the immediate mesh draw; 1 at construction
    float viewportWidth;                           // d3dApp rect right minus left, as a float
    float viewportHeight;                          // d3dApp rect bottom minus top, as a float
    float aspect;                                  // holds viewportHeight/viewportWidth after Frustrum_Construct, but Screen_SetProjection overwrites it
    float viewDistance;                            // the reference draw distance the fog band is anchored to; 100.0f at construction, changed by Frustrum
    u8 fogEnabled;                                 // gate for Frustrum_GetFogValue and the D3DRENDERSTATE_FOGENABLE write; 0 at construction
    u32 fogColor;                                  // D3DRENDERSTATE_FOGCOLOR value, masked to 24 bits by Frustrum_SetFog
    s32 fogMode;                                   // selects the ramp curve in Frustrum_BuildFogRamp: 1 = exp, 2 = exp squared, 3 = linear (the construct
    float fogStart;                                // near edge of the fog band, pushed as D3DRENDERSTATE_FOGSTART; 1.0f at construction
    float fogEnd;                                  // far edge of the fog band, pushed as D3DRENDERSTATE_FOGEND; 1.0f at construction
    float fogScale;                                // 255.0f / (fogEnd - fogStart), cached for the per-vertex fog lookup in the mesh submission code. It i
    u32 fogRamp[256];                              // precomputed vertex fog factors in the top byte of each word (0xFF000000 = no fog, 0 = fully fogged),
};

class GameState {
public:
    void Game_SetFlags(u32 mask, s32 on);
    void Game_ResetState();
    s32 Game_CanOpenMenu();
    s32 animDt;                                    // IS g_animDt; zeroed by Game_ResetState
    u8 _pad004[0xc];
    Dav *pDav;                                     // IS g_pDav; zeroed by Game_ResetState
    s32 fadeTimer;                                 // IS g_fadeTimer; zeroed by Game_ResetState
    u32 flags;                                     // IS g_gameFlags ( + 0x18 =): the existing GameFlags enum applies unchanged. Game_Res
    u8 cineState;                                  // IS the letterbox/cinematic state byte: 0 idle, 1 bars animating, 2 bars fully open (subtitl
    u8 camDebugMode;                               // camera debug mode, passed as argument 2 of Camera_Update by Game_Frame: 0 normal, 1 stick
};

class GeyserIn : public ScnBody {
public:
    virtual void PostLoadInit();                   // GeyserIn_PostLoadInit (override)
    virtual void Update();                         // GeyserIn_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GeyserIn_HandleMessage (override)
    virtual void Reset();                          // GeyserIn_Reset (override)
    void SetAnimState(u8);
    void SetState(u8);
    u8 state;                                      // Update dispatches on it; SetState's final byte store is.
    u8 animState;                                  // SetAnimState compares the incoming byte, then stores it after the animation switch; Update
    u16 corkPushCount;                             // counts MSG_PUSH pulses to the corking rock (which is this->victim +0x90, not a separate pointer); re
    s32 timerMs;                                   // ms countdown, minus g_dtMs each frame. PostLoadInit also seeds it with 0x3e8 at
    s32 activePeriod;                              // PostLoadInit: property 0x20 shifted by 10, DWORD store; SetState(1) copies it to the tim
    s32 transitDelayMs;                            // property 0x1c multiplied by 1000, DWORD store; SetState(3) copies it.
    s32 corkPeriod;                                // property 0x18 shifted by 10, DWORD store; SetState(4) loads it.
    s32 idlePeriod;                                // property 0xc shifted by 10, DWORD store; SetState(0) loads it.
    s32 pullTimerMs;                               // stores of 840 and 190; Update tests it signed for non-negative and decrement
    Vec3s pullOffset;                              // three signed word components copied from pos onward, then the victim's position is subtr
    Vec3s frameDelta;                              // word copy from pullOffset /6d/87, signed word multiplication by low16(g_dtMs), division b
    CollBox *detectBox;                            // Scn_GetPropBox(record,4) result stored; the pointer is loaded for ObjGrid_Qu
    ScnObject *victim;                             // cleared; receiver of the virtual msg 0x31 -452; its position/rotation and cla
    ScnObject *geyserController;                   // Scn_GetPropObject(record,0x10) result stored; receives msg 0x2800. Name inferred from th
    ScnObject *geyserOut;                          // Scn_GetPropObject(record,0x14) result stored; receiver of 0x2880-0x2888.
    CamSetup *cameraSetup;                         // the msg 0x2885 return, stored; the camera arguments use u16 offsets 0, 2, 4, 6 and a Vec
    u16 soundHandle;                               // zeroed by a word store; passed to Sound_Stop in SetAnimState and to Sound_IsPlaying in U
    u8 flags;                                      // bit 2 is NOT ALWAYSMANAGE â€” prop 0 is never read by the class (only three prop-getter calls exist: 0
};

class GeyserManger : public ScnLogic {
public:
    virtual void PostLoadInit();                   // GeyserManger_PostLoadInit (override)
    virtual void Update();                         // GeyserManger_Update (override)
    u16 boxCount;                                  // Number of detection boxes in PROPERTY_GEYSERMANGER_BOXDETECT, returned through Scn_GetPropIdList's c
    CollBox **boxes;                               // The BOXDETECT id list: boxCount box pointers. Update tests Ralph's x and z against each box's min +4
    u16 geyserCount;                               // How many of the GEYSERnn properties resolved to a live GeyserIn (class 72) or GeyserOut (class 73);
    ScnObject *geysers[16];                        // The managed GeyserIn/GeyserOut objects. Every frame they all receive msg 0x5d with argument 1 while
};

class GeyserOut : public ScnBody {
public:
    virtual void PostLoadInit();                   // GeyserOut_PostLoadInit (override)
    virtual void Update();                         // GeyserOut_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GeyserOut_HandleMessage (override)
    virtual void Reset();                          // GeyserOut_Reset (override)
    void SetState(u8 newState);
    void SetAnimState(u8 newAnim);
    u8 state;                                      // GeyserOutState: 0 idle, 1 erupting (the spit logic), 2 stopping (passenger released, end anim), 3 di
    u8 animState;                                  // 3 idle (anim 1), 4 erupt start (anim 2), 5 jet loop (anim 0, looping), 6 end (anim 3, update mode 0)
    s32 timer;                                     // ms countdown to the automatic stop: set to spitDelay by SetState(1), decremented by g_dtM
    s32 wolfSpeedPerUnit;                          // 4000 / |TRAJECTORYWOLF[1] - TRAJECTORYWOLF[0]|: the per-unit speed falloff used for the W
    s32 objSpeedPerUnit;                           // the same quotient computed a second time; used for every other class, speed = 2500 - this
    Vec3s startPoint;                              // TRAJECTORYWOLF's first point: the geyser mouth. The distance from a carried object to it
    CollBox *spitBox;                              // PROPERTY SPITBOX (Scn_GetPropBox(record, 0x10)): the box the eruption carries objects in; q
    TrajFollower objPath;                          // follower over PROPERTY TRAJECTORY, speed 0, heading bias 0x800, continuous heading, 3-D d
    TrajFollower wolfPath;                         // follower over PROPERTY TRAJECTORYWOLF, same settings: the path Ralph is pushed along
    ScnObject *passenger;                          // the object the vortex handed over (msg 0x2886 argument); cleared by SetState(2)
    void *camera;                                  // PROPERTY CAMERA (Scn_GetPropCamera(record, 4)); returned to msg 0x2885, never us
    s32 spitDelay;                                 // how long one eruption lasts, in ms: set by msg 0x2883 and copied into timer by SetState(1
    u32 flags;                                     // GeyserOutFlags: 1 ALWAYSSPIT, 2 CAMERAFORSALAD, 4 camera taken, 8 held off by msg 0x2881, 0x10 the U
    u16 soundHandle;                               // handle of the looping jet sound 0x12f, started when the camera is within (squared) and stop
};

struct GhostTravel {
public:
    Vec3s target;                                  // destination point
    s16 speed;                                     // units per second along the leg; 400 patrol/flee, 200 slow patrol
    s16 heading;                                   // facing to approach the target with: atan2(dx, dz) + 0x800
    s16 arriveDist;                                // 50: Ghost_StepTravel reports arrival below this distance
};

class Ghost : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Ghost_Init (override)
    virtual void Update();                         // Ghost_Update (override)
    virtual void Render(Camera *view);             // Ghost_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Ghost_HandleMessage (override)
    virtual void Reset();                          // Ghost_Reset (override)
    void SetState(u8 newState);
    void StartCaptureCamera();
    void StartChaseCamera(s32 behindWolf);
    s16 FindNearestNode(Vec3s p);
    void BuildNodeGraph(u16 trajListId);
    s16 FindNodeAt(s16 x, s16 z);
    s16 PickNextNode(s16 node, Vec3s target, s32 randomPick, s32 away);
    void SetTravel(GhostTravel *t, Vec3s from, Vec3s to, s16 speed);
    s32 StepTravel(GhostTravel t, Vec3s *outVel, u16 *outHeading);
    s16 TurnToward(s16 target);
    u8 CheckWolfProximity();
    s16 curNode;                                   // index into g_ghostNodes of the node the ghost last reached; set from Ghost_FindNearestNode at init/r
    s16 nextNode;                                  // index of the node being travelled to (Ghost_PickNextNode result); node pair used for the p
    s16 avoidNode;                                 // node index the picker must not turn back to; 0x25 (= no node) at init, reset and on msg 0x4001 (0x44
    u8 _pad082[0x6];
    Box *activationBox;                            // PROPERTY_GHOST_ACTIVATIONBOX via Scn_GetPropBox(record, 0); only ever tested against the
    GhostTravel travel;                            // current travel leg: target point, speed, heading and arrive radius; filled by Ghost_SetTravel (0x44e
    s32 chaseRange;                                // state 7 leash: set to 99999 on entering the chase and then to wolfDist every frame (0x44c
    u8 state;                                      // 0 idle .. 9 boo-wait; written by Ghost_SetState and twice directly in Update ( =
    u8 prevState;                                  // state before the last Ghost_SetState; Ghost_PickNextNode treats prevState == 1 specially
    s32 rethink;                                   // set by message 0x4002; Ghost_PickNextNode consumes it and then return
    ScnObject *hoover;                             // the Hoover (class 131) found by Scenaric_FindByClass at init; told 0x4583 when the suck f
    Vec3s homePos;                                 // position of the nearest node at init, pushed through SetPosition and re-applied on reset
    Vec3s capturePos;                              // the Wolf's position when the capture (state 4) started; the capture step holds him there
    s32 vulnerable;                                // argument of message 0x4001 from the Hoover; non-zero makes the ghost blue and sends it to
    s32 wolfInBox;                                 // whether the Wolf was inside activationBox on x and z when Init/Reset ran; never
    s32 beingSucked;                               // 1 from the Hoover's suck message 0 until the next reset; blocks the idle state's SetVisib
    u32 master;                                    // PROPERTY_GHOST_MASTER (record property +4, read in Ghost_Create); non-zero on exactly one g
    s32 wolfBooed;                                 // set by message 0x70 (the Wolf's boo broadcast) in states 8/9; polled as Wolf message 0x41
    s32 cameraArmed;                               // 1 at init and reset; the capture state starts the scripted camera only while it is set (0
    s32 unkCC;                                     // zeroed by Init, Reset and message 0xE; nothing in the class ever sets it
    u8 booCount;                                   // number of boos answered; incremented on entering state 9 and at 3 the ghost gives up and
    s16 fadeDir;                                   // +1 fading to the vulnerable blue, -1 fading back, 0 idle; set from the 0x4001 argument ( / 0
    u8 tintR;                                      // low byte of the tint colour being faded
    u8 tintG;                                      // middle byte of the faded tint colour
    u8 tintB;                                      // high byte of the faded tint colour
    u8 _pad0d7[0x1];
    u8 baseR;                                      // the model's own tint bytes, saved at init and restored when the fade ends
    u8 baseG;                                      // saved tint byte 1
    u8 baseB;                                      // saved tint byte 2
    u8 _pad0db[0x1];
    u8 fadeSteps;                                  // Ten reference steps per fade; at zero the end colour is written.
    u32 tintScratch;                               // member used as scratch while packing tintB/tintG/tintR into the tint colour
    u8 _pad0e4[0x4];
    s32 wolfDist;                                  // Vec3s_DistXZ(wolf.pos, pos) each frame, forced to 3000 while the Wolf is in the ghost cos
    s16 stepR;                                     // per-frame tint delta toward the vulnerable blue 0x6E: 0x6E - baseR
    s16 stepG;                                     // 0xB4 - baseG
    s16 stepB;                                     // 0xFA - baseB
    s32 turnRate;                                  // angular velocity accumulator handed to Math_ApproachAngle by Ghost_TurnToward
    ScnObject *ghosts[15];                         // the other Ghosts (class 120) from Scenaric_FindByClass; messaged 0x4002 when one comes wi
    u16 ghostCount;                                // count written by Scenaric_FindByClass
    ScnBody capturedSheep;                         // embedded ScnBody built from export 0xBF; shown at the ghost's position during the capture
    ScnRecordSynth capturedSheepRecord;            // synthetic record for that body (Scn_BuildRecordFromExport)
    s32 hasCapturedSheep;                          // 1 when the export resolved and the body was initialised
    s32 showCapturedSheep;                         // 1 while the capture animation runs; Render draws the embedded body only then and only in
    s32 fireballTimerMs;                           // 1150 ms at capture, counted down by g_dtMs; at 0 the FireBall is told 0x5981 every frame
    u16 soundHandle;                               // handle of the last Sound_Play, stopped before the next one
    s32 fadeRemainder = 0;
};

class GhostCostume : public ScnBody {
public:
    virtual void PostLoadInit();                   // GhostCostume_Init (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GhostCostume_HandleMessage (override)
    Vec3s homePos;                                 // Same as SheepCostume.homePos (identical code).
};

class GhostHalo : public ScnBody {
public:
    virtual void PostLoadInit();                   // GhostHalo_PostLoadInit (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GhostHalo_HandleMessage (override)
};

struct GhostNode {
public:
    Vec3s pos;                                     // node position; vertical is always written 0
    s16 pad6;                                      // not written by Ghost_BuildNodeGraph
    GhostNode *links[4];                           // neighbours, appended by Ghost_BuildNodeGraph in both directions
    u8 linkCount;                                  // number of entries in links
    u8 pad19;                                      // padding before index
    s16 index;                                     // the node's own index in g_ghostNodes; how a neighbour pointer is turned back into an inde
};

struct GoalFlagBits {
public:
    u8 cinematic : 1;                              // (bits) bit 0 (1-byte unsigned unit)
    u8 noSheep : 1;                                // (bits) bit 1 (1-byte unsigned unit)
    u8 triggered : 1;                              // (bits) bit 2 (1-byte unsigned unit)
    u8 reserved : 5;                               // (bits) bits 3..7 (1-byte unsigned unit)
};

class Goal : public ScnLogic {
public:
    virtual void PostLoadInit();                   // Goal_Init (override)
    virtual void Update();                         // Goal_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Goal_Reset (override)
    u32 winRadiusSq;                               // winRadius squared; Ralph's horizontal dist^2 to the goal must be <= this
    Box *cinBox;                                   // PROPERTY_GOAL_CINBOX resolved (passed to Cine_Start)
    Box *cinSheepBox;                              // PROPERTY_GOAL_CINSHEEPBOX resolved
    char *cinText;                                 // PROPERTY_GOAL_TEXTFORCIN resolved: the class string Text_GetClassString returns, or 0 without CINE_H
    u32 cinFlags;                                  // PROPERTY_GOAL_FLAGSFORCIN
    u16 cinId;                                     // PROPERTY_GOAL_CINEMATIC
    u16 winRadius;                                 // modelBox.max.x * 90 / 128 = 138 for the standard goal; also the sheep search radius
    GoalFlagBits goalFlags;                        // bit0 use cinematic, bit1 no sheep required, bit2 already triggered
};

class GoldenCoins : public ScnBody {
public:
    virtual void PostLoadInit();                   // GoldenCoins_Init (override)
    virtual void Update();                         // GoldenCoins_Update (override)
    virtual void Render(Camera *view);             // GoldenCoins_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GoldenCoins_HandleMessage (override)
    virtual void Reset();                          // GoldenCoins_Reset (override)
    void SetState(u8 next);
    u8 state;                                      // GoldenCoinsState 0..4 (GoldenCoins_SetState).
    s32 appearTimer;                               // Set to 0x3c00 (3.75 s of g_gameTime) by SetState(3); no reader found in the class.
    s32 inInventory;                               // 1 after msg 7 (stored in the inventory), 0 after msg 8 (equipped); makes msg 0x4a80 report the coin
    s32 claimedBySam;                              // Set by msg 0x4a83 (Sam pocketing it); msg 2 then offers nothing to the Wolf. Cleared by SetState 0 a
    s32 revealed;                                  // Set to 1 when the MineDetector's msg 0x2f reveals a buried coin; cleared only by Init. Reset picks b
    Vec3s homePos;                                 // Ground-snapped start position (Init); SetState(0) and Reset move the coin here.
    s32 renderScale;                               // Uniform render scale, 0 meaning 0x400; set by Sam msg 0x4a82 while pocketing (0x2aa, 0x155), cleared
    s32 hintPending;                               // 1 after msg 0x4a84 (Sam near the Wolf while it holds this coin); consumed by the next msg 6.
    u16 hintDist;                                  // Sam-Wolf XZ distance from msg 0x4a84, or the outDist that Scenaric_FindBestInRadius writes (600 when
};

class GossamerOnde : public ScnBody {
public:
    virtual void PostLoadInit();                   // GossamerOnde_PostLoadInit (override)
    virtual void Update();                         // GossamerOnde_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GossamerOnde_HandleMessage (override)
    virtual void Reset();                          // GossamerOnde_Reset (override)
    void SetState(u8);
    u8 state;                                      // 0 = shockwave expanding (animation playing), 1 = idle/hidden. Only GossamerOnde_SetState writes it,
    u16 radius;                                    // Current radius of the expanding ring, growing by g_dt*0x36b>>12 (875 u/s) per frame; a candidate is
    u16 vertTolerance;                             // Maximum absolute vertical difference for the wave to knock Ralph over; set to 0xB4 (180) on launch.
    MoveModifyArg *moveArg;                        // The MSG_MODIFY_MOVE 0x1a payload (MoveModifyArg {Vec3s delta; u16 flags; Vec3s velocity}) cached on
    Vec3s wolfPush;                                // Radial push applied to Ralph, normalised to magnitude 40 away from the wave centre with the vertical
    Vec3s sheepPush;                               // The same 40-unit radial push for the sheep, but run through Collide_ResolveMove on the shee
    s32 dist;                                      // Scratch: horizontal distance from the wave centre to the candidate, from sqrt(dx^2+dz^2) via __ftol
    s32 wolfPushed;                                // Set to 1 the frame the ring passes Ralph; afterwards the stored wolfPush keeps being applied, and MS
    s32 sheepPushed;                               // The same latch for the sheep (set, tested).
    ScnObject *sheep;                              // g_pSheepOutOfZone captured when the wave launches; the wave registers itself as a rider on
};

class Gossamer_Boss : public ScnMobile {
public:
#ifdef SDW_MEMBERS_Gossamer_Boss
    SDW_MEMBERS_Gossamer_Boss
#endif
    virtual void PostLoadInit();                   // Gossamer_Boss_PostLoadInit (override)
    virtual void Update();                         // Gossamer_Boss_Update (override)
    virtual void Render(Camera *view);             // Gossamer_Boss_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Gossamer_Boss_HandleMessage (override)
    virtual void Reset();                          // Gossamer_Boss_Reset (override)
    void SetState(u8 newState);
    s32 IsInBox(const Vec3s *p, s32 usePhaseBox);
    s32 IsInSmashBox(const Vec3s *p, s32 index);
    void FallStep();
    void WalkHome();
    void StartHitCamera();
    void ChaseStep();
    void TurnStep(u8 nextState);
    void MoveTo(Vec3s *target);
    void UpdateFacingDelta();
    u8 phaseSecs[3];                               // designer properties at prop offsets 0x18/0x1c/0x20, kept as bytes (PostLoadInit).
    u8 state;                                      // GossamerBossState; switch subject of Update and of SetState (0x4507
    u8 prevState;                                  // previous state saved by SetState
    u8 pathIndex;                                  // index into path->pts, advanced by msg 0x43 ( add al,1) and read by the return step (
    s16 shakeTicks;                                // remaining game-clock ticks until the shockwave; negative after triggering
    s16 hitIndex;                                  // phase counter: 0xffff at Init/Reset, +1 per msg 0x43 ( add ax,1); indexes phaseBo
    s16 phaseOverlapX;                             // pos.x - phaseBoxes[hitIndex]->max.x when the last phase box is left
    s16 turnAccum;                                 // turn accumulated by TurnStep in 4096ths; compared with turnNeeded
    s16 turnRefAngle;                              // reference heading for TurnStep, facing - 0x800
    s16 turnDir;                                   // +1 / -1 sense of the current turn
    s16 turnDirLatch;                              // turnDir latched on the first frame of a turn; a reversal aborts the turn
    u16 spinFacing;                                // facing saved when state 8 starts and restored every frame of it
    u32 wolfDist;                                  // XZ distance to Ralph, refreshed at the top of Update; unsigned in every compare (
    u32 timerMs;                                   // state timer in ms; += g_dtMs
    u32 timerLimitMs;                              // limit for timerMs ( cmp)
    u32 facingDelta;                               // |Ralph's facing - the boss's facing| wrapped to <= 3000, written by UpdateFacingDelta
    u32 spinDurationMs;                            // copy of timerLimitMs kept for the dust column's frame selector
    u16 turnNeeded;                                // total turn a TurnStep must accumulate: 0x800 / 0x1000 / 0x3000 by state (0x450ad
    s32 wolfHeld;                                  // reply to Ralph's freeze message 0xe; msg 0xf releases him
    s32 showHitBody;                               // hitBody is rendered and animated while set
    s32 chasingFlat;                               // set when Ralph has been flattened; the chase step then polls msg 0x41c
    s32 smashStarted;                              // state 0xb: the smash animation has been launched
    s32 hasChaseTarget;                            // non-zero while chaseTarget is valid (msg 0x42 arg +0x1c4); switches states 0x11/0x12
    s32 caughtWolf;                                // set when state 0xe starts within 100 units of Ralph (msg 0x41d); decides state 0x12 vs 0x10 (0x45011
    s32 pathShifted;                               // one-shot: the path points are shifted by the msg 0x43 argument only once
    s32 showSpinBody;                              // spinBody is rendered while set
    Vec3s toTarget;                                // target - pos each frame
    Vec3s velocity;                                // units per second for this frame
    Vec3s frameDelta;                              // Vec3s_ScaleByDt(velocity): the move applied this frame
    Vec3s spawnPos;                                // position at the start of the fight; Reset puts the boss back there
    Vec3s fxOrigin;                                // dust-column origin: pos with the vertical raised 0xdc
    Vec3s fxAnchor;                                // fxOrigin pushed 100 units away from the camera through fxFrame
    Vec3s arenaMaxSave;                            // arenaBox->max as loaded; Reset restores it
    Vec3s arenaMinSave;                            // arenaBox->min as loaded; Reset restores it
    Vec3s homePos;                                 // the point the boss walks back to (path point, or its position at Reset)
    Vec3s fallStep;                                // per-frame fall/slide vector of the knock-back, and the landing spot handed to SetPosition (
    Mat34s fxFrame;                                // fixed quarter-turn about the vertical used to offset the dust column toward the camera; rot[4] is se
    InlineEmitter3 fx;                             // 3-slot dust column driven by Emitter_UpdateColumn; an inline-storage emitter (0x60 bytes)
    EmitterColumnParams fxParams;                  // Emitter_UpdateColumn parameter block of the dust column: riseSpeed 0, life 1000, period 1000, size 0
    u16 soundHandle;                               // channel handle of the spin / impact sound
    Trajectory *path;                              // PROPERTY at offset 0x14 (Scn_GetPropTrajectory): the walk-back points
    ScnBody hitBody;                               // extra body built from export 0x95; drawn at the boss position while showHitBody
    ScnBody spinBody;                              // extra body built from export 0xd8; drawn at fxOrigin while showSpinBody
    ScnRecordSynth bodyRecord;                     // scratch record shared by the two Scn_BuildRecordFromExport calls
    u8 _pad27e[0x2];
    CamSetup camSetup;                             // CamSetup: focal = CAMERA property word 0; the whole 16-byte block is overwritten by *camProp in the
    CamSetup *camProp;                             // PROPERTY CAMERA at offset 0 (Scn_GetPropCamera returns a CamSetup *)
    Box *phaseBoxes[3];                            // PROPERTY boxes at offsets 4/8/0xc: one arena box per phase
    Box *arenaBox;                                 // PROPERTY box at offset 0x10: the fight arena
    CollBox *modelBox;                             // the model's first collision box (inst_model->boxes->boxes); widened while the boss falls
    CollBox modelBoxSave;                          // modelBox min/max as loaded; Reset restores them
    Box smashBoxes[3];                             // copies of *phaseBoxes[i] with min.x and max.x shifted by -(5000+i); tested by IsInSmashBo
    ScnObject *onde;                               // the level's GossamerOnde (class 0xb6) from Scenaric_FindByClass; polled with msg 0x6c dur
    ScnObject *chaseTarget;                        // msg 0x42 arg: the sender's +0x1c0 pointer, chased in state 0x11
};

class TrailEmitter {
public:
#ifdef SDW_MEMBERS_TrailEmitter
    SDW_MEMBERS_TrailEmitter
#endif
#ifdef SDW_EXTRA_TrailEmitter
    SDW_EXTRA_TrailEmitter
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[16];                             // 16 x 0xC position records
    Particle particleBuf[16];                      // 16 x 8-byte particle records
};

class TrajPatrol {
public:
#ifdef SDW_MEMBERS_TrajPatrol
    SDW_MEMBERS_TrajPatrol
#endif
    void Reverse();
    Trajectory *traj;                              // the trajectory: u16 count then count Vec3s
    s16 pointIndex;                                // index of the point being steered toward; steps by +1 or -1 depending on forward
    s16 speed;                                     // speed in units/s multiplying the normalised direction
    s16 headingBias;                               // constant added to the atan2 heading; 0x800 here
    u16 heading;                                   // cached 12-bit heading toward the current point
    s32 arriveRadiusSq;                            // squared arrival radius ( squares the init argument)
    u32 continuousHeading;                         // recompute the heading every step instead of only on a point change
    u32 use3dDistance;                             // include the vertical axis in the distance and the output velocity
    u32 moving;                                    // set to 1 by every step and to 0 by init
    u32 advanced;                                  // 1 when a point was consumed during this step
    u32 forward;                                   // 1 = walk the points forwards, 0 = backwards; init sets 1 and the Gossamer flips it when a
    u32 forceAdvance;                              // one-shot: makes the next step consume a point whatever the distance; set when blocked and
};

class Gossamer_Lev08 : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Gossamer_Lev08_PostLoadInit (override)
    virtual void Update();                         // Gossamer_Lev08_Update (override)
    virtual void Render(Camera *view);             // Gossamer_Lev08_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Gossamer_Lev08_HandleMessage (override)
    virtual void Reset();                          // Gossamer_Lev08_Reset (override)
    s32 IsInActionZone(Vec3s *p);
    void SetState(u8 newState);
    s32 ChaseWolf();
    s32 UpdateStruggle();
    s32 IsWolfOnHighGround();
    u16 NearestFleePoint();
    Box *pastZone;                                 // IDPASTACTIONZONE (prop 0x34) via Scn_GetPropBox; the zone he patrols/chases in while inFu
    Box *cineBox;                                  // IDCINEBOX (prop 0x14) via Scn_GetPropBox; its presence chooses the start state 9 (0x4525d
    Box *cineBox2;                                 // IDCINEBOX2 (prop 0x18) via Scn_GetPropBox; box of the second cinematic, played from msg 0
    Box *futureZone;                               // IDFUTURACTIONZONE3 (prop 0x30) via Scn_GetPropBox; his zone while inFuture != 0 (
    ZoneList futureZones;                          // IDFUTURACTIONZONE (prop 0x28) id list: Scn_FindIdList into boxes/count; searched by IsInA
    CollBox *myBox;                                // his model's first collision box; half of the box-overlap test that carries the Wolf while
    CollBox *wolfBox;                              // g_pWolf's model's first collision box, the other half of that test
    u8 state;                                      // current state, the switch selector of Update and written by SetState
    u8 prevState;                                  // state before the last SetState; state 7 (play one animation) switches on it when the anim
    u8 mashCount;                                  // button presses counted while struggling out of his hug; > 6 within 1500 ms frees Ralph (0
    u8 squeezeTicks;                               // updates counted in state 1; a camera shake at 5 and at 0x10; reset by SetState(1)/SetStat
    u8 grabKind;                                   // 0 = grabbed by msg 0x39 (time portal), 1 = grabbed by msg 0x3a (carried), 0xff = release done; drive
    u8 hugStage;                                   // 0 none, 1 hug started (state 4 first press), 2 hug broken; cleared by PostLoadInit/Reset
    u8 pad0a2[2];                                  // tail padding before the s32 at 0xa4
    s32 distToTarget;                              // Vec3s_DistXZ to the current target, the divisor of every velocity in this class
    u32 timer;                                     // ms accumulator (+= g_dtMs)
    u32 mashStartMs;                               // timer when the struggle's first button press arrived
    u32 mashLastMs;                                // timer at the last struggle button press; 800 ms without one resets the struggle (
    u32 timerLimit;                                // ms limit compared with timer in states 0 and 11
    Vec3s target;
    Vec3s vel;                                     // velocity in units/s, fed to Vec3s_ScaleByDt; also reused as an absolute position for SetP
    Vec3s step;                                    // this frame's translation: Vec3s_ScaleByDt output and Collide_ResolveMove in/out
    u8 pad0ca[2];                                  // tail padding before the pointer at 0xcc
    Vec3s *grabPos;                                // &sender->pos stored by msg 0x38 and steered to in state 3
    Vec3s startPos;                                // his position after PostLoadInit's SnapToGround; Reset puts him back here
    u8 pad0d6[2];                                  // tail padding before the ContactInfo at 0xd8
    ContactInfo contact;                           // Collide_ResolveMove output used by states 0 and 7; contact.wallObj at 0xdc is polled for
    s32 unusedF4;                                  // written 0 by PostLoadInit and Reset; never read
    s32 fleeing;                                   // 1 once the second cinematic has sent him down IDFLEEINGTRAJ; gates the movement half of s
    s32 hugging;                                   // 1 while Ralph is being squeezed and the struggle input is being counted
    Trajectory *fleeTraj;                          // IDFLEEINGTRAJ (prop 0x24) via Scn_GetPropTrajectory; the path he runs when he flees
    TrajPatrol patrol;                             // the follower along fleeTraj, initialised with speed 1000, bias 0x800 and arrive radius 0
    s32 inFuture;                                  // toggled by msg 0x39 (the time portal); selects futureZone(s) instead of pastZone in IsInAc
    s32 wolfHeld;                                  // 1 while he holds Ralph frozen: set from !g_pWolf->HandleMessage(this, 0xf, 0) and cleared with msg 0
    s32 cinePending;                               // set by msg 0x11; state 0 turns it into the flee animation and clears it
    s32 hasFled;                                   // 1 after the flee has been started; msg 0x1e does nothing once it is set
    s32 patrolActive;                              // 1 = state 0 walks the trajectory; cleared when the state-0 timer runs out
    EmitterDriftParams dustParams;                 // his dust: {10, -20, 0x800, 0x155, 0x28, 0x5a, 0} written by PostLoadInit
    TrailEmitter dust;                             // 16-particle dust trail, constructed by the factory and reset by PostLoadInit
    u16 soundHandle;                               // handle of the looping sound started by SetState(0); stopped there and by Reset
    u8 pad2be[2];                                  // tail padding before the u32 at 0x2c0
    u32 cineId;                                    // IDCINEMATIC (prop 0x1c)
    u32 cineTextId;                                // CINETEXT (prop 0x08)
    u32 cineId2;                                   // read from prop 0x1c as well: IDCINEMATIC again, not IDCINEMATIC2 (prop 0x20), which nothi
    u32 cineText2Id;                               // CINETEXT2 (prop 0x0c)
    u32 cineFlags;                                 // CINEFLAGS (prop 0x00)
    u32 cineFlags2;                                // CINEFLAGS2 (prop 0x04)
    char *cineText;                                // Text_GetClassString(cineTextId), or 0 when the string is empty
    char *cineText2;                               // Text_GetClassString(cineText2Id), or 0 when the string is empty
    u8 cineStage;                                  // 0 none, 1 arrival cinematic started, 2 flee cinematic started
    u8 pad2e1[3];                                  // tail padding to sizeof 0x2e4
};

struct GroundMineFlagBits {
public:
    u16 onMover : 1;                               // (bits) bit 0 (2-byte unsigned unit) | The word read/modify/write at 4d59a9/4d59c3 is bit 0 of +0x120.
    u16 falling : 1;                               // (bits) bit 1 (2-byte unsigned unit)
    u16 triggered : 1;                             // (bits) bit 2 (2-byte unsigned unit)
    u16 countingDown : 1;                          // (bits) bit 3 (2-byte unsigned unit)
};

class GroundMine : public Mine {
public:
    virtual void PostLoadInit();                   // GroundMine_Init (override)
    virtual void Update();                         // GroundMine_Update (override)
    virtual void Render(Camera *view);             // GroundMine_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // GroundMine_HandleMessage (override)
    virtual void Reset();                          // GroundMine_Reset (override)
    s32 RayToHazard(const Vec3s *origin, const Vec3s *direction, s32 limit);
    u32 FallStep(u16 flags);
    u32 Move(Vec3s *delta, ContactInfo *contact, u16 flags, const Vec3s *velocity);
    ZoneList hazardBoxes;                          // zone list {boxes, count}: BOXES id-list (Scn_FindIdList). Non-empty = REGION mode: a hidden minefiel
    ZoneList safeBoxes;                            // zone list {boxes, count}: TRAJECTORIES id-list: safe-path boxes carved out of the hazard boxes (Lvl-
    InlineEmitter3 fuseFx;                         // 3-slot countdown column (Emitter_UpdateColumn with g_groundMineFuseFxParams, drawn as billboards); p
    u32 fuseDelay;                                 // TIMEINS * 0x800 - 1 ticks (0 if TIMEINS = 0 -> explodes at once), i.e. TIMEINS x 0.5 s
    u16 beepSound;                                 // channel handle of the armed beep 0x96, replayed while in state 2/3, stopped otherwise
    u16 radius;                                    // RADIUS: trigger radius (vertical band +-radius/2); ring grows to half-size radius*2
    GroundMineFlagBits gmFlags;                    // GroundMineFlags
    u16 pad122;                                    // padding
    InlineEmitter4 ringFx;                         // 4-slot ground ring pulse showing the danger radius (Emitter_UpdateFade, drawn flat) while armed or c
    EmitterFadeParams ringParams;                  // EmitterFadeParams: life = Emitter_UpdateFade params p[0]: particle life 0x4000 (4 s); fadeStart = p[
    s32 fallTime;                                  // g_dt accumulated while gmFlags & 2, capped at 0x1e000 (30 s); fall speed = clamp(fallTime*2000>>12,
    ScnObject *mover;                              // Object dragging the mine (msg 0x18 sets it if empty, 0x19 clears it for the matching sender): the Ma
};

struct GroundQuery {
public:
    Vec3s pos;                                     // query point; y is overwritten with the surface height (msg 0xd argument)
    Vec3s normal;                                  // surface normal out, 4.12 (up = (0,-0x1000,0))
};

class HairDryer : public ScnMobile {
public:
    virtual void PostLoadInit();                   // HairDryer_Init (override)
    virtual void Update();                         // HairDryer_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // HairDryer_HandleMessage (override)
    virtual void Reset();                          // HairDryer_Reset (override)
    void Blow();
    void SetState(u8);
    Vec3s homePos;                                 // spawn position (Init; msg 9 arg 1); Reset returns here when IN_WORLD
    u16 blowSound;                                 // handle of the looping blow sound 0x11b (requested every frame in state 2); stopped by HairDryer_SetS
    u8 state;                                      // HairDryerState: 0 world, 1 held, 2 blowing
    u8 pad85[3];                                   // tail padding to sizeof 0x88
};

class Heap {
public:
#ifdef SDW_MEMBERS_Heap
    SDW_MEMBERS_Heap
#endif
    HeapBlock *FindPrevBlock(HeapBlock *block);
    void *AllocAligned(u32 size, u32 align);
    void *AllocSmall(u32 size);
    void BitmapFill(u32 *bitmap, u32 nBits);
    void CarveBlock(HeapBlock *block, u32 blockSize, u32 need);
    void FreeSmall(void *ptr);
    void InitArena(u8 *start, u8 *end, u32 align);
    void InitAux(u32 value, u32 unused);
    void InitSmallPools(u32 poolBytes, u32 minSlot, u32 maxSlot);
    void Stub3_54da63(u32 a, u32 b, u32 c);
    void Stub3_54da70(u32 a, u32 b, u32 c);
    void Stub_54da4d();
    void Stub_54da58();
    void TermArena();
    void TermAux();
    void TermSmallPools();
    void Init(u8 *block, u32 size);
    void Term();                                                 /* Heap_Term */
    void *Alloc(u32 size);
    void Free(void *ptr);
    HeapBlock *firstBlock;
    HeapBlock *freeHead;                           // Head of the free list; head->prev points to itself. Every free/unlink path compares against it. Once
    HeapBlock *endSentinel;                        // Permanent 0x10-byte block at ((end-0x10)&~alignMask)-8, header 0x10 (0x11 when the block before it i
    u32 alignMask;                                 // align-1, where align = max(CeilPow2(requested), 4); 3 for both shipped heaps. Allocation need = (siz
    u32 alignInvMask;                              // ~(align-1), the AND mask used to round sizes and the sentinel address.
    u32 auxParam;                                  // Written only by Heap_InitAux (0x8000 on the > 1 MB path), never read anywhere; stays 0 in t
    u32 freeCount;                                 // Number of blocks on the free list, sentinel excluded; used as the search-loop bound by Heap_Alloc/He
    u32 smallMinSlot;                              // Slot size of pool 0 in bytes (power of 2, at least 8). Pool p's bitmap word covers smallMinSlot << (
    u32 smallMaxSlot;                              // Largest size served by the pools = smallMinSlot << (nPools-1). Heap_Alloc tries Heap_AllocSmall when
    u32 smallPoolBytes;
    u32 smallPool0Slots;                           // Slot count of pool 0 = total/(nPools*smallMinSlot); pool p has smallPool0Slots >> p slots (Heap_Allo
    u16 smallMinShift;                             // log2 of the smallest small-block slot; pool i has slots of 1 << (smallMinShift + i) bytes (Heap_Free
    u16 smallPoolShift;                            // log2 of each small pool's region size; pool index = (ptr - smallBase) >> smallPoolShift
    u8 *smallBase;                                 // start of the small-block pools; Heap_Free sends ptr in [smallBase, smallEnd) to Heap_FreeSm
    u32 *smallBitmapWords;                         // Start of the bitmap words, just after the nPools-entry pointer table (= smallBitmaps + nPools). Writ
    u8 *smallEnd;                                  // end (exclusive) of the small-block pools, range test in Heap_Free
    u32 **smallBitmaps;                            // Per-pool bitmap pointers (MSB-first, set bit = free). This is also the user pointer of the single He
};

struct HeapBlock {
public:
    u32 sizeFlags;                                 // Block size in bytes including this 8-byte header (bits 2..27, mask 0x0ffffffc) | 2 = this block is f
    HeapBlock *next;                               // Free block: next on the free list (the sentinel's points to itself). Allocated block: the magic 0x98
    HeapBlock *prev;                               // Free block: previous on the free list (the head's points to itself). Allocated block: first word of
};

class HeapOfLeaf : public ScnBody {
public:
    virtual void PostLoadInit();                   // HeapOfLeaf_Init (override)
    virtual void Update();                         // HeapOfLeaf_Update (override)
    virtual void Render(Camera *view);             // HeapOfLeaf_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // HeapOfLeaf_HandleMessage (override)
    virtual void Reset();                          // HeapOfLeaf_Reset (override)
    s32 PointInPolygon(Vec3s *);
    void ComputeRadius();
    void DetectMovers();
    u8 disabled;                                   // set when IDPOLY is missing or has < 2 points, or GLOBALZONE is missing
    u16 radius;                                    // max Manhattan XZ distance from pos to the polygon points (pre-test)
    Box *zone;                                     // GLOBALZONE box; its x/z extent is the grid query rectangle
    ScnObject *guardian;                           // IDGUARDIAN; gets msg 0x1D on each rustle (the bull in Level 4 and B1)
    Trajectory *polygon;                           // IDPOLY record, a Trajectory {u16 count; Vec3s pts[count]} used as a polygon (x/z used)
    u8 _pad074[0x4];
    Vec3s *pipPrev;                                // scratch previous-vertex pointer of HeapOfLeaf_PointInPolygon
    Vec3s *pipCur;                                 // scratch current-vertex pointer of HeapOfLeaf_PointInPolygon
    EmitterDriftParams driftParams;                // EmitterDriftParams: hSpeed = EmitterDriftParams p[0] = 10 (horizontal drift per second); vSpeed = p[
    InlineEmitter16 leafFx;                        // 16-slot leaf-particle emitter with inline storage (0x98..0x1fc): factory sets slotPool = +0
};

class HiddenRocks : public ScnLogic {
public:
    virtual void PostLoadInit();                   // HiddenRocks_Init (override)
    virtual void Update();                         // HiddenRocks_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // HiddenRocks_HandleMessage (override)
};

class HitSwitch : public ScnLogic {
public:
    virtual void PostLoadInit();                   // HitSwitch_PostLoadInit (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // HitSwitch_HandleMessage (override)
    virtual void Reset();                          // HitSwitch_Reset (override)
    ScnObject *target;                             // The object named by PROPERTY_HITSWITCH_TARGETS, resolved; receives msg 0x2c when a bull
};

class Hive : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Hive_PostLoadInit (override)
    virtual void Update();                         // Hive_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Hive_HandleMessage (override)
    virtual void Reset();                          // Hive_Reset (override)
    void AimCameraAt(Vec3s);
    void SetState(u8);
    u8 state;                                      // 0 idle, 1 shaking (hit by a cannonball), 2 camera follows the swarm, 3 camera follows Sam, 4 releasi
    u8 beeIndex;                                   // Cursor into bees[] for both broadcast loops. In state 4 it is ALSO passed as msg 0x3980's argument,
    u32 beeCount;                                  // Result of Scenaric_FindByClass(107 Bees, bees, 4): how many Bees swarms exist in the level. Loop bou
    u32 swarmPresent;                              // 1 while a Bees swarm is circling this hive (set by msg 0x3e03). Cleared at Init, when a non-mother h
    u32 isMother;                                  // PROPERTY_HIVE_MOTHER (prop 4). A non-mother hive that is hit broadcasts msg 0x3981 to move the swarm
    u32 samInHoney;                                // Latched to 1 by msg 0x67 (HoneyPot sends it with arg = Sam while Sam stands in the spill) when the a
    u32 trackSwarm;                                // Set to 1 by msg 0x3e04 (the Bees report the swarm has relocated to this hive); makes state 1 aim the
    u32 uiFrozen;                                  // Result of sending the Wolf SCN_MSG_UI_FREEZE 0xe when a cannonball hits. SetState(0) uses it to gate
    ScnObject *bees[4];                            // The level's Bees objects (class 107), filled by Scenaric_FindByClass with max 4 - exactly the 0x98..
    ScnObject *swarm;                              // The Bees object that announced itself with msg 0x3e03; its position is what states 1 and 2 point the
    ScnObject *sam;                                // The level's Sam from Scenaric_FindByClass(1, &sam, 1). State 3 aims the camera at him. The return co
    CamSetup *camera;                              // PROPERTY_HIVE_CAMERA (prop 0) via Scn_GetPropCamera: the camera setup record {u16 focal; s16 rot[3];
    s32 samWatchTime;                              // PROPERTY_HIVE_TIME (prop 8), in ms: how long the camera stays on Sam after the swarm is released. Lv
    s32 samWatchTimer;                             // Millisecond countdown of state 3, loaded from samWatchTime when msg 0x67 first reports Sam in the ho
    s32 beeReleaseTimer;                           // Millisecond countdown between bee releases in state 4: reloaded with 0x96 = 150 after each msg 0x398
};

class RenderPoly {
public:
#ifdef SDW_MEMBERS_RenderPoly
    SDW_MEMBERS_RenderPoly
#endif
    virtual ~RenderPoly();                            // RenderPoly_VectorDeletingDtor
    void Assign(const RenderPoly *src);                          /* RenderPoly_Assign */
    void InitFromBsFlat(const BsPolyFlat *src);
    void InitFromBsGouraud(const BsPolyGouraud *src);
    void InitFromBsTexFlat(const BsPolyTexFlat *src);
    void InitFromBsTexGouraud(const BsPolyTexGouraud *src);
    void InitFromBsBlendFlat(const BsPolyBlendFlat *src);
    void InitFromBsBlendGouraud(const BsPolyBlendGouraud *src);
    u32 type;                                      // 1 opaque untextured, 2/3 sorted blend modes, >=4 texture index+4 (bit 0x8000 masked off)
    PolyTri poly;                                  // the embedded PolyTri (vptr, constructed at this+8 by RenderPoly_Ctor); its three v
    float *verts;                                  // 0x60-byte vertex block (3 x 0x18 untextured or 3 x 0x20 textured TL vertices)
    float sortZ;                                   // depth key for the sorted list (sum of vertex z when batcher+0x40 == 1)
};

class HoleFX {
public:
#ifdef SDW_MEMBERS_HoleFX
    SDW_MEMBERS_HoleFX
#endif
    virtual ~HoleFX();                                // HoleFX_ScalarDeletingDtor
    void Init(D3DApp *app, void *viewport, float radius, u8 useCubicBezier);
    void SetShading(float scale, float bias);
    void UpdateVertices(int unusedBatcherArg);
    HRESULT CaptureScreen(int useSecondBuffer, u8 force);
    void Draw(PolyBatcher *unusedBatcher);
    u8 CreateVertexBuffer();
    void BuildMesh();
    u8 BuildCircleMask();
    void Bezier2_EvalWithTangent(float *outPos, float *outTangent, const float *p0, const float *p1, const float *p2, float t);
    void Bezier3_Eval(float *outPos, float *outTangent, const float *p0, const float *p1, const float *p2, const float *p3, float t);
    u32 Color_GreyFromScalar(float v);/* inline: the cubic curve's extra control point, the curve's start moved toward the hole centre by * viewportHeight / 50 */
    u8 hasCaptureSurface;                          // nonzero selects the blocking transition mode
    float pos[3];                                  // screen x, y, z of the hole quad
    float alpha;                                   // alpha/scale term animated only in blocking mode
    D3DApp *app;                                   // the renderer wrapper; +0x28 is the IDirect3DDevice7 and +0x41E40/+0x41E44 the two surfaces the wipe
    Frustrum *viewport;                            // the Frustrum whose viewport it wipes (typed from its field reads - nearZ +0x08, viewportWidth +0x1c,
    u8 useCubicBezier;                             // selects the cubic Bezier path (four control points with an extra point pulled toward the h
    float radius;                                  // base radius of the hole ring; each ray direction is normalised and scaled by it before the Bezier sw
    Vec3f rayDirs[8];                              // the 8 outward directions of the radial fan, set by HoleFX_BuildMesh to the compass points of the vie
    SdwVertexBuffer *vertexBuffer;          // SdwVertexBuffer of 0x41 vertices, FVF 0xC4 (XYZRHW|DIFFUSE|SPECULAR, 0x18 bytes each). Lock i
    RenderPoly tris[120];                          // the 120 triangles of the wipe mesh, constructed in place by the ctor and destroyed backwards from +0
    float shadeScale;                              // colour shading scale: read only by the fmul in the colour function; set by Hole
    float shadeBias;                               // colour shading bias: read only by the fadd in the colour function; set by HoleF
    SdwTexture *captureSurface;                    // IDirectDrawSurface7 the wipe blits the frozen frame into; its existence is what selects blocking mod
    u32 captureWidth;                              // dwWidth read back from the capture surface; divides each vertex x to give its U
    u32 captureHeight;                             // dwHeight read back from the capture surface; divides each vertex y to give its V
    u8 captured;                                   // latch set the first time HoleFX_CaptureScreen runs; blocks any further capture unless the force argu
    Texture *maskTexture;                          // 256x256 format-2 (ARGB4444) texture holding the circular alpha mask used by the non-blocking wipe; +
    float screenRadius;                            // sqrt((vpW*vpW + vpH*vpH)/2) â€” the radius at which the non-blocking hole covers the whole screen
};

class HoneyPot : public ScnMobile {
public:
    virtual void PostLoadInit();                   // HoneyPot_Init (override)
    virtual void Update();                         // HoneyPot_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // HoneyPot_HandleMessage (override)
    virtual void Reset();                          // HoneyPot_Reset (override)
    void SetState(u8);
    u8 state;                                      // HoneyPotState; written only by HoneyPot_SetState
    u32 beeCount;                                  // Scenaric_FindByClass(107 Bees, bees, 5) result; loop bound for msg 0x67
    u8 _pad084[0x4];
    Vec3s homePos;                                 // = pos at Init and on msg 9 arg 1; msg 0x55 moves the pot back here
    CollBox spillBox;                              // query box rebuilt around the pot (x/z +-150, y from pos-150 to pos); min +0x94, max +0x9a; flags wor
    ScnObject *bees[5];                            // Bees objects (class 107) that receive msg 0x67 (arg Sam) while Sam stands in the spill
    ScnObject *hiveMother;                         // PROPERTY_HONEYPOT_HIVEMOTHER (Scn_GetPropObject offset 0); also gets msg 0x67; not NULL-checked
    ScnObject *sam;                                // Sam (Scenaric_FindByClass(1)); his origin is tested in msg 2
    u16 launchSoundHandle;                         // Sound_Play(0xA6) handle on msg 0xC; zeroed by Init/Reset, never read or stopped
    u16 landSoundHandle;                           // Sound_Play(0xA7) handle on msg 0x25; zeroed by Init/Reset, never read or stopped
};

class Hoover : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Hoover_Init (override)
    virtual void Update();                         // Hoover_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Hoover_HandleMessage (override)
    virtual void Reset();                          // Hoover_Reset (override)
    void SetState(u8);
    Vec3s homePos;                                 // = pos at Init (if in world) and on msg 9 arg 1; Reset moves the Hoover here and snaps it to the grou
    u8 state;                                      // HooverState (0 armed, 1 off, 2 sucking)
    Box *activationBox;                            // PROPERTY_HOOVER_ACTIVATIONBOX (Scn_GetPropBox offset 0); stored by Init and never read
    ScnObject *ghosts[15];                         // Ghost objects (class 120) from Scenaric_FindByClass(0x78, â€¦, 15); receive 0x4001 and the suck reques
    ScnObject *battery;                            // sender of 0x4581 (the master Battery); gets 0x4404 arg 1 at suck start and arg 0 at suck end. Uninit
    u16 ghostCount;                                // count for ghosts[]
    u32 inHand;                                    // 1 after a Wolf held-action query (msg 0x17); 0 after msg 7 (stored), Init, Reset. Gates arming on 0x
    u32 charged;                                   // 1 on 0x4581 (5 batteries), 0 on 0x4582; gates arming on msg 8
    u32 wolfFrozen;                                // return of Wolf msg 0xE (MSG_FREEZE) when a suck starts; cleared when msg 0xF (MSG_UNFREEZE) succeeds
    u16 suckSoundHandle;                           // Sound_Play(0x6B, vol 0x33) handle while sucking; stopped by SetState(1/2)
};

class IceCube : public ScnBody {
public:
    virtual void PostLoadInit();                   // IceCube_PostLoadInit (override)
    virtual void Update();                         // IceCube_Update (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // IceCube_CustomCollide (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // IceCube_HandleMessage (override)
    virtual void Reset();                          // IceCube_Reset (override)
    void SetState(u8 newState);
    CollBox box;                                   // world collision box = first solid model box + pos (Box_Translate); CustomCollide sweeps
    ScnObject *prisoner;                           // PROPERTY OBJ (Scn_GetPropObject(record, 0)): the object frozen inside; sent msg 0x41 whe
    s32 meltTime;                                  // 4.12 s of heat received (+= g_dt only on frames with the heated bit); 0x1000 (1 s) melts t
    u8 state;                                      // IceCubeState: 0 frozen (anim 2), 1 melting (anim 0, paused when not heated), 2 melted (waits for the
    u8 melted : 1;                                 // (bits) bit 0: set on entering state 2; blocks further heat (msg 0x41); a Reset keeps a melted cube gone (0x
    u8 heated : 1;                                 // (bits) bit 1: set by msg 0x41 (the hair dryer) and cleared by the next Update; the melt only advances on fr
    u8 noDaffy : 1;                                // (bits) bit 2: set in PostLoadInit unless the level has a DaffyMilitary (class 0x39, loop)
};

class IceGround : public ScnBody {
public:
    virtual void PostLoadInit();                   // IceGround_PostLoadInit (override)
    virtual void Update();                         // IceGround_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // IceGround_HandleMessage (override)
    virtual void Reset();                          // IceGround_Reset (override)
    u8 state;                                      // 0 intact/idle, 1 kill received (start breaking), 2 break animation running. Reset does NOT clear it,
    CamSetup *camera;                              // PROPERTY_ICEGROUND_IDCAMERA (0): the scripted camera record played while the ice breaks; NULL means
    u32 broken;                                    // 1 once the break animation has finished. While set, HandleMessage returns 0 for EVERY me
};

struct IdleAnimEntry {
public:
    u16 animId;
    u8 loopsMin;                                   // lower bound of the loop count rolled with Rand_Range
    u8 loopsMax;                                   // upper bound
};

struct InflatableFlagBits {
public:
    u8 inflated : 1;                               // (bits) bit 0 (1-byte unsigned unit): inflated
    u8 burnt : 1;                                  // (bits) bit 1 (1-byte unsigned unit): burnt (black tint; deflated by Reset)
    u8 rest : 6;                                   // (bits) bits 2..7 (1-byte unsigned unit)
};

class InflatableSheep : public ScnMobile {
public:
    virtual void PostLoadInit();                   // InflatableSheep_Init (override)
    virtual void Update();                         // InflatableSheep_Update (override)
    virtual void Render(Camera *view);             // InflatableSheep_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // InflatableSheep_HandleMessage (override)
    virtual void Reset();                          // InflatableSheep_Reset (override)
    s32 CheckDeathZone();
    s32 CheckSquashed(s32 popAfter);
    void ApplyFall(Vec3s *delta);
    void Pop();
    void Respawn();
    void SetDeflated();
    void SetInflated();
    void SetState(u8 value, s32 blend);
    void UpdateTint();
    InlineEmitter4 emitter;                        // embedded water-ripple emitter with inline storage (0x7c..0xf0): factory sets slotPool = +0x
    s32 fallTime;                                  // g_dt accumulated while falling (states 2, 7); drives ApplyFall; zeroed on landing/floating/drop
    u32 stateTime;                                 // g_gameTime at Respawn / state 12 entry / Init / Reset; 3 s hidden wait and 1 s wobble (a g_gameTime
    s32 driftSpeed;                                // water-flow drift speed (Math_ApproachLinear toward the zone flow speed, max 2000, accel 50, decel 10
    Vec3s homePos;                                 // respawn position: = pos at Init and on msg 9 arg 1
    s16 carryYaw;                                  // (own facing - carrier facing) & 0xFFF when the Wolf lifts it inflated; attach rotation, restored on
    s16 driftHeading;                              // heading of the water drift, stepped toward the zone flow heading at 0x2800/s
    u8 state;                                      // InflatableSheepState
    u8 carryJoint;                                 // joint index from msg 4 arg; reused by AttachLink_SetParams after inflating
    InflatableFlagBits isFlags;                    // InflatableSheepFlags: bit 0 inflated, bit 1 burnt (black tint; deflated by Reset)
};

class InlineEmitter10 {
public:
#ifdef SDW_MEMBERS_InlineEmitter10
    SDW_MEMBERS_InlineEmitter10
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[10];                             // 10 x 0xC position records
    Particle particleBuf[10];                      // 10 x 8-byte particle records
};

class InlineEmitter32 {
public:
#ifdef SDW_MEMBERS_InlineEmitter32
    SDW_MEMBERS_InlineEmitter32
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[32];                             // 32 x 0xC position records
    Particle particleBuf[32];                      // 32 x 8-byte particle records
};

class InlineEmitter6 {
public:
#ifdef SDW_MEMBERS_InlineEmitter6
    SDW_MEMBERS_InlineEmitter6
#endif
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[6];                              // 6 x 0xC position records
    Particle particleBuf[6];                       // 6 x 8-byte particle records
};

class InlineEmitter8 {
public:
#ifdef SDW_MEMBERS_InlineEmitter8
    SDW_MEMBERS_InlineEmitter8
#endif
    void RenderFlat(Camera *view, s32 fwd);
    ParticleEmitter base;                          // the ParticleEmitter this inline-storage emitter is: its inline constructor points slotPool at slotBu
    Vec3f slotBuf[8];                              // 8 x 0xC position records
    Particle particleBuf[8];                       // 8 x 8-byte particle records
};

struct InputBinding {
public:
    InputDevice *dev;                              // the device the action is bound to: InputMgr_BindAction stores it at table+8*slot
    u16 code;                                      // button / DIK scan code on that device: stored at table+8*slot+4; InputMgr_Poll reads dev-
};

struct InputBindingIdx {
public:
    u8 devIdx;                                     // device index of a binding (1 keyboard, 2 joystick, 4 mouse; enum MenuBindingDevice): out[0] of Input
    u16 code;
};

class InputDevice {
public:
#ifdef SDW_MEMBERS_InputDevice
    SDW_MEMBERS_InputDevice
#endif
    virtual ~InputDevice();                           // InputDevice_ScalarDeletingDtor
    virtual long SetAcquired(u8 acquire);          // InputDevice_SetAcquired
    virtual long Update();                         // InputDevice_Update
    void AllocButtons(u16 buttonCount, D3DApp *app);
    void SetAxisCalibration(float sx, float sy, float sz, float ox, float oy, float oz);
    void GetAxisCalibration(float *sx, float *sy, float *sz, float *ox, float *oy, float *oz);
    u16 GetPressedButtons(u16 *out, u16 maxCount);
    u32 GetObjectName(u32 diObjOffset, char *out);
    u8 acquired;                                   // 1 while the DirectInput device is acquired; Update refuses to run when it is 0.
    u8 deviceKind;                                 // 0 = joystick, 2 = keyboard, 4 = mouse: base ctor writes 0, joystick 0, keyboar
    u8 isDigital;                                  // 1 = digital device: InputMgr_Poll turns axisX/axisY into D-pad bits by sign against 0.0f (0
    float axisX;                                   // Normalised X axis: clamp(raw/65535, -1, +1) * scaleX + offsetX. The input manager reads these.
    float axisY;                                   // Normalised Y axis, same formula.
    float axisZ;                                   // Normalised Z axis (throttle / PgUp-PgDn on the keyboard), same formula.
    u8 *buttonPressed;                             // Per-button output array: 1 when the button counts as pressed this frame after the blocked/repeat rul
    u16 buttonCount;                               // Number of buttons, capped at 0x100. 0x100 for the keyboard, 0xC for the joystick, 4 for the mouse.
    D3DApp *pApp;                                  // Owning D3DApp; used for its hWnd in SetCooperativeLevel and to reach IDirectInput8.
    s32 rawX;                                      // Raw axis value in DirectInput units (-0xFFFF..+0xFFFF), written by the device-specific Update before
    s32 rawY;                                      // Raw Y axis, same convention.
    s32 rawZ;                                      // Raw Z axis, same convention.
    u8 *buttonDown;                                // Per-button current physical state, written by the device Update (high bit of DIJOYSTATE.rgbButtons f
    float axisScaleX;                              // Per-axis gain applied after normalisation; 1.0 by default, set by InputDevice_SetAxisCalibration fro
    float axisScaleY;                              // Per-axis gain for Y.
    float axisScaleZ;                              // Per-axis gain for Z.
    float axisOffsetX;                             // Per-axis bias added after scaling; 0.0 by default (centre calibration).
    float axisOffsetY;                             // Per-axis bias for Y.
    float axisOffsetZ;                             // Per-axis bias for Z.
    s32 rawRX;                                     // right stick X, raw (-0xFFFF..+0xFFFF); only the controller sets it
    s32 rawRY;                                     // right stick Y, raw
    float axisRX;                                  // right stick X, clamp(raw/65535, -1, +1); no calibration
    float axisRY;                                  // right stick Y, same
    u8 *buttonBlocked;                             // Per-button suppression mask: a non-zero entry forces buttonPressed to 0 for that button even while i
    u8 allowHeld;                                  // 1 (the constructor default) means a button that was already down last frame still reports pressed; 0
    u8 *buttonDownPrev;                            // Previous frame's buttonDown snapshot, updated at the end of InputDevice_Update.
    IDirectInputDevice8A *pDevice;                 // The DirectInput device. Vtable uses: GetCapabilities 0x0C, EnumObjects 0x10, GetProperty 0x14, SetPr
};

class InputMgr {
public:
#ifdef SDW_MEMBERS_InputMgr
    SDW_MEMBERS_InputMgr
#endif
    virtual ~InputMgr();                              // InputMgr_ScalarDeletingDtor
    u8 SaveBindingTable(u8 toRegistry, const char *dir, const char *name, InputBinding *table, InputDevice *master);
    u8 CreateDevices(u8 wanted);
    u8 IsDeviceDigital(u8 devIdx);                               /* 1 if the device is digital (or absent) */
    u8 PollDevice(u8 devIdx);
    u8 SelectDevice(u8 devIdx);
    u8 GetCurrentDeviceIdx();
    void SetAxisCalibration(float sx, float sy, float sz, float ox, float oy, float oz);
    u8 BindAction(u8 slot, u8 devIdx, u16 code);
    u8 BindMenuKey(u8 slot, u16 code);
    u8 GetBinding(u8 slot, InputBindingIdx *out);
    long RecreateDevice(u8 devIdx);
    void ResetEdges();
    long SetAcquiredAll(u8 acquire);
    long SetAcquired(u8 devIdx, u8 acquire);
    void SetActiveFlags(u8 flag);
    long Poll();
    long GetAnyPressedRaw(InputBinding *out);
    long GetAnyPressed(InputBindingIdx *out);
    u8 LoadConfig(const char *dir);
    void ApplyFixedPadMapping();                     // binds the controller's twelve buttons as delivered (input_mgr.cpp)
    void ApplyDefaultKeyChanges();                   // the port's changes to the disc's default keys (input_mgr.cpp)
    u8 SaveConfig(const char *dir);
    u8 LoadBindingTable(u8 fromRegistry, const char *dir, const char *name);
    InputDevice *DeviceFromIndex(u8 devIdx);
    u8 IndexFromDevice(InputDevice *dev);
    u8 FindBinding(const InputBinding *b);
    u8 FindBindingByIndex(const InputBindingIdx *b);
    u8 IsReservedKeyBinding(const InputBinding *b);
    u8 IsReservedKey(const InputBindingIdx *b);
    void Input_GetKeyName(u32 diObjOffset, char *out, u32 outSize);
    u8 CheckDevices();
    u8 DeviceHasBindings(u8 devIdx);
    u8 axisX;                                      // stick X 0..255 (0x80 centre)
    u8 axisY;                                      // stick Y 0..255 (0x80 centre)
    u8 rightX;                                     // right stick X 0..255 (0x80 centre; always centre on the keyboard)
    u8 rightY;                                     // right stick Y 0..255 (0x80 centre)
    u16 padBits;                                   // PS1-layout active-low pad word built by InputMgr_Poll (0x1000 Triangle ... 0x0001 Select)
    u16 menuNav;                                   // arrow-key nav word with auto-repeat (0xffef up, 0xffbf down, 0xffdf right, 0xff7f left; 0xffff none)
    u8 pausePressed;                               // Pause or P pressed this frame (edge)
    u8 escPressed;                                 // Esc pressed this frame (edge)
    u8 enterPressed;                               // Enter pressed this frame (edge)
    s32 navRepeatMs;                               // auto-repeat countdown: 500 first, then 125
    Keyboard *keyboard;
    Joystick *joystick;
    Mouse *device3;                                // the mouse device: ctor (vtable, deviceKind 4). InputMgr passes 12 buttons (push 0x
    u8 presentMask;                                // 1 keyboard, 2 joystick, 4 third
    InputDevice *current;                          // device read by InputMgr_Poll
    InputBinding *bindings;                        // current binding table: &padTable or &keyTable
    InputDevice *preferred;                        // device to return to when it responds again
    InputBinding padTable[16];                     // 16 x {InputDevice *dev; u16 code; u16 pad} for the pad
    InputBinding keyTable[16];                     // 16 x binding for the keyboard
    u16 lastNav;                                   // previous nav value for the auto-repeat
    u8 pausePrev;
    u8 escPrev;
    u8 enterPrev;
};

class InstantHoover : public ScnBody {
public:
    virtual void PostLoadInit();                   // InstantHoover_Init (override)
    virtual void Update();                         // InstantHoover_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // InstantHoover_HandleMessage (override)
    virtual void Reset();                          // InstantHoover_Reset (override)
    Box *FindDockBox();
    ScnObject *FindNearestSocket();
    u8 CountMartiansInRange();
    void DrawHud();
    void SuckMartians();
    u8 state;                                      // InstantHooverState (1 idle, 2 dropped, 3 open, 4 anim)
    u16 suckRange;                                 // PROPERTY_INSTANTHOOVER_HORRANGE is loaded here and immediately overwritten with 300; capture radius
    u16 vertRange;                                 // PROPERTY_INSTANTHOOVER_VERTRANGE; never read
    u16 dockRadius;                                // inscribed radius (min half-extent on x/z) of the ACTIVEBOX pad found by FindDockBox; docking require
    u16 openTimeMs;                                // PROPERTY_INSTANTHOOVER_OPENTIME (ms) - length of state 3 (effectively halved, see engine notes)
    Sprite hudIcon;                                // GameRes 193 DAV_IDI_IGHICONC, drawn at (15,200)-(47,219)
    AnimSprite hudDigits;                          // GameRes 130 DAV_IDI_ITICPTR_, digit sheet for the remaining-Martian counter
    ScnObject *martians[10];                       // InstantMartian objects (class 145)
    u8 _pad0dc[0x14];
    ScnObject *marvin;                             // Marvin (class 193); msg 0x6480 arg = all Martians captured
    ScnObject *sockets[15];                        // InstantSocket objects (class 194); nearest one gets 0x6501 plug / 0x6502 unplug
    ScnObject *dockedSocket;                       // nearest socket at the last successful dock; only tested for non-NULL (msg 7), never cleared
    u32 martianCaptured[10];                       // 1 once SuckMartians has taken martians[i]; Reset re-validates with msg 0x4C81
    u8 _pad15c[0x14];
    u32 openTimerMs;                               // ms spent in state 3 (incremented by g_dtMs twice per frame)
    u32 hudTint;                                   // HUD panel colour: g_uiTintColor, flashing to 0xAA0000 every other frame while a Martian is in range
    u8 capturedCount;                              // Martians captured so far
    u8 martianCount;                               // FindByClass(145) count (10 on Lvl-17)
    u8 socketCount;                                // FindByClass(194) count
    u32 martianInRange;                            // set in state 1 from CountMartiansInRange; written only, no reader in the class
    u32 docked;                                    // 1 after a successful dock in state 2; 0 on pickup/drop/Init/Reset. Gates HUD, sucking, the remote (m
    u8 _pad184[0x4];
    u8 nextAction;                                 // InstantHooverNext: what state 4 does when the anim ends
    u16 dockBoxCount;                              // count of the ACTIVEBOX id-list
    Box *dockBox;                                  // pad found by FindDockBox in the last state-2 frame
    Box **dockBoxes;                               // PROPERTY_INSTANTHOOVER_ACTIVEBOX id-list (10 pads on Lvl-17)
    Vec3s homePos;                                 // = pos at Init; Reset returns here unless KEPT
    u16 soundHandle;                               // vestigial: only ever zeroed; Reset calls Sound_Stop on it
};

class InstantMartian : public ScnMobile {
public:
#ifdef SDW_MEMBERS_InstantMartian
    SDW_MEMBERS_InstantMartian
#endif
    virtual void PostLoadInit();                   // InstantMartian_PostLoadInit (override)
    virtual void Update();                         // InstantMartian_Update (override)
    virtual void Render(Camera *view);             // InstantMartian_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // InstantMartian_HandleMessage (override)
    virtual void Reset();                          // InstantMartian_Reset (override)
    s32 HoldWolf(s32 hold);
    void SetState(u8 newState);
    s16 Scale(s16 v);
    u16 CollideAndSlide(Vec3s *delta, ContactInfo *info, u16 cutoff, u16 flags, s32 noMove);
    u16 MoveHalves(Vec3s *delta, ContactInfo *info, u16 cutoff, u16 flags);
    s32 InsideRunBoxes(Vec3s p);
    u8 state;                                      // InstantMartianState 0..0xe; SetState stores its argument here, Update switches on
    u8 behaviour;                                  // from PROPERTY_INSTANTMARTIAN_MARTIENBEHAVIOR: designer 1 -> 1, 4 -> 3, anything else 0 ( /0x4
    s32 turnTimer;                                 // ms left before the martian may turn again while walking ( -= g_dtMs, 0 clears `turning`; res
    s32 stateTimer;                                // ms left in the current state: 0x400 at load, 0x800 or 0 entering state 2 ( /0x457d
    u32 sitAnimMs;                                 // duration of animation 0xf in ms (Anim_GetDurationMs); the per-frame fraction of the sit
    s16 heading;                                   // walk heading, 12-bit ( = rot.y + 0x800 & 0xfff)
    s16 turning;                                   // non-zero while the new walking heading is held: set to 0x800, cleared when t
    s32 holdingWolf;                               // nonzero while Ralph is attached to the martian's joint 0x17; the setter sends the Wolf mess
    s32 shrunk;                                    // set by message 0x56; every size and speed the class uses is then divided by 3, and the so
    u32 colForTraj;                                // PROPERTY_INSTANTMARTIAN_COLFORTRAJ; nonzero = collide while following the trajectory (0x4
    Vec3s nextPos;                                 // the position the walk step would reach (pos + step), tested against the run boxes before it is taken
    Vec3s homePos;                                 // placed position after SnapToGround, restored by Reset through SetPosition
    Vec3s target;                                  // the point to walk to: message 0x50 copies the sender's position here
    Vec3s step;                                    // per-frame movement for the sit move: (target - pos) scaled by g_dt/sitAnimMs
    Vec3s toTarget;                                // target - pos at the moment state 3 starts
    Vec3s wolfPos;                                 // Ralph's position, 4 units above him, taken when the martian sits on him and used as its o
    ScnObject *lasers[55];                         // every Laser (class 0x96) of the level, up to 55, from Scenaric_FindByClass
    u16 laserCount;                                // how many of lasers[] are filled
    u16 voice;                                     // Sound_Play handle for the running sound 0xdf; SetState stops it first
    Box *boxes[5];                                 // PROPERTY_INSTANTMARTIAN_BOX..BOX5, the boxes it may run inside; null properties are skipped (0x455dd
    Box *checkpointBox;                            // PROPERTY_INSTANTMARTIAN_CHECKPOINTBOX
    Trajectory *traj;                              // PROPERTY_INSTANTMARTIAN_TRAJECTORY
    TrajPatrol patrol;                             // the TRAJECTORY patrol (TrajPatrol_Init, speed 600, radius 0x32; TrajPatrol_Step
    u16 boxCount;                                  // how many of boxes[] are filled
    u16 boxIndex;                                  // index into boxes[] of the box the martian is currently running inside
};

class InstantSocket : public ScnBody {
public:
    virtual void PostLoadInit();                   // InstantSocket_Init (override)
    virtual void Update();                         // InstantSocket_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // InstantSocket_HandleMessage (override)
    virtual void Reset();                          // InstantSocket_Reset (override)
};

struct InteractScan {
public:
    CollBox selfBox;                               // scanner's first model box translated to its position
    CollBox candBox;                               // candidate box translated to the candidate's position
    s32 primaryBoxGap;                             // box-gap threshold, starts at boxDist, lowered by an accepted primary box; gates both outp
    s32 primaryPointDist2;                         // squared point radius, starts at pointRadius^2, lowered by an accepted primary point; gate
    s32 secondaryBoxGap;                           // written on an accepted secondary box; never read by the scanner
    s32 secondaryPointDist2;                       // written on an accepted secondary point; never read by the scanner
    Vec3s selfPos;                                 // copy of the scanner's pos
    Vec3s candPos;                                 // copy of the candidate's pos
};

class Jail : public ScnBody {
public:
    virtual void PostLoadInit();                   // Jail_PostLoadInit (override)
    virtual void Update();                         // Jail_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Jail_HandleMessage (override)
    virtual void Reset();                          // Jail_Reset (override)
    s32 AtTargetHeight();
    void SetState(u8 value);
    u8 state;
    s16 targetVert;                                // PostLoadInit writes it from pos.y (+0xe), not from heightTop; it only equals heightTop b
    s16 heightTop;                                 // the highest of the three path heights (sorted in PostLoadInit); access width proven by the byte matc
    s16 heightMiddle;
    s16 heightBottom;
    s32 delay;                                     // ms before the cage starts moving (0x400 when sent up, 0 otherwise); access width proven by the byte
    Vec3s previousPos;
    ScnObject *topSwitch;
    ScnObject *bottomSwitch;
    ScnObject *middleSwitch;
    CollBox carryBox;                              // built from the HIGHEST-index model box with flags&1 (the scan decrements), and Box.flags
    CollBox clearBox;                              // its top sits exactly 60 units below the cage's underside (clear.min.y = carry.max.y + 60 per 0x4cf73
    CollBox *lowerBox;                             // the model box whose centre is lowest (largest y: y points down); access width proven by the byte mat
    CollBox *upperBox;
    u16 soundHandle;                               // motor sound 0xb2, volume 0xff, SoundPlayFlags 3 (the 4th argument is flags, not a priority)
    s32 open;                                      // the reads are (msg 0x4300) and (msg 0x4301), and ONLY the SetState call is gated o
};

class Joystick : public InputDevice {
public:
#ifdef SDW_MEMBERS_Joystick
    SDW_MEMBERS_Joystick
#endif
    virtual ~Joystick();                              // Joystick_ScalarDeletingDtor
    virtual long SetAcquired(u8 acquire);          // Joystick_SetAcquired (override)
    virtual long Update();                         // Joystick_Update (override)
    u8 caps[44];                                   // DIDEVCAPS filled by GetCapabilities in Joystick_Construct; bytes here because the generat
};

class Key : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Key_Init (override)
    virtual void Update();                         // Key_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Key_HandleMessage (override)
    virtual void Reset();                          // Key_Reset (override)
    void SetState(u8 value);
    Vec3s homePos;                                 // Ground-snapped Init pos, or pos at msg 9 arg 1; Reset restores it.
    u8 state;                                      // KeyState: 0 on the ground (anim 0), 1 held.
    s32 jailDist;                                  // 3-axis Manhattan distance to the jail, computed by Key_Update; 410 when |dvert| >= 200. Use needs <
    ScnObject *jail;                               // Jail (class 0x7e) from FindByClass; receives msg 0x4300. Its +0xb4 (non-zero = already opened, infer
};

class Keyboard : public InputDevice {
public:
#ifdef SDW_MEMBERS_Keyboard
    SDW_MEMBERS_Keyboard
#endif
    virtual ~Keyboard();                              // Keyboard_ScalarDeletingDtor
    virtual long SetAcquired(u8 acquire);          // Keyboard_SetAcquired (override)
    virtual long Update();                         // Keyboard_Update (override)
    void SetAxisKeys(u16 xNeg, u16 xPos, u16 yNeg, u16 yPos, u16 zNeg, u16 zPos);
    u16 axisKeys[6];                               // the DIK scan codes that stand in for the three axes: X-, X+, Y-, Y+, Z-, Z+ (Keyboard_Construct 0x40
};

class Laser : public ScnBody {
public:
    virtual void PostLoadInit();                   // Laser_PostLoadInit (override)
    virtual void Update();                         // Laser_Update (override)
    virtual void Render(Camera *view);             // Laser_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Laser_HandleMessage (override)
    virtual void Reset();                          // Laser_Reset (override)
    void SetState(u8 value);
    u8 state;                                      // 0 fresh (Update picks 1 or 3 from the INACTIVE flag), 1 disabled (hidden, SCN_UPD_NEVER, sound stopp
    s32 activationStartMs;                         // g_gameTimeMs captured when state 2 begins; the activation delay is measured from
    u16 activationDelayMs;                         // PROPERTY_LASER_ACTIVATIONDELAY (prop 0) truncated to 16 bits: how long state 2 lasts before the beam
    u16 activationElapsedMs;                       // g_gameTimeMs - activationStartMs, stored each frame of state 2 as a 16-bit value, so it wraps every
    u32 flags;                                     // Bit values: 1 = the INACTIVE property was set, 2 = a MSG_SWITCH_OFF toggle has already been consumed
    u16 soundHandle;                               // Handle of the looping beam sound. The call is Sound_Play(soundId 0xff, this, volume 0x7f, flags 7, r
    u16 type;                                      // PROPERTY_LASER_TYPE (prop 12), 0..9; selects the beam animation through the permutation below rather
    u16 animId;
    CollBox *beamBox;
    s16 normX;
    s16 normZ;                                     // z of that normal ( -> ContactInfo.wallNormalMean.z)
    u8 beamOn : 1;
};

class Lava : public ScnBody {
public:
    virtual void PostLoadInit();                   // Lava_PostLoadInit (override)
    virtual void Update();                         // Lava_Update (override)
    s16 range;                                     // PROPERTY_LAVA_RANGE (prop 0, PROPSIZE_LAVA 4): half-width of the square XZ area around homePos in wh
    Vec3s homePos;                                 // The placed position after the conditional snap to ground; every eruption is re-seated at homePos plu
};

class LazerRobot : public ScnBody {
public:
#ifdef SDW_MEMBERS_LazerRobot
    SDW_MEMBERS_LazerRobot
#endif
    virtual void PostLoadInit();                   // LazerRobot_PostLoadInit (override)
    virtual void Update();                         // LazerRobot_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // LazerRobot_HandleMessage (override)
    virtual void Reset();                          // LazerRobot_Reset (override)
    void CheckWolfKill();
    void StepPath();
    void TurnToward(Vec3s *target);
    u8 active;                                     // 1 while the robot flies its patrol path (anim 1, hum 0xdb), 0 while it is parked (anim 0, hum 0xdc);
    u16 voice;
    u32 pulseLatch;                                // 1 between MSG_SWITCH_ON and MSG_SWITCH_OFF; the ON arm does nothing while it is set, so one held swi
    CollBox killBox;                               // world-space kill volume rebuilt every active frame around the robot: +-240 on x and z, 400 above pos
    Trajectory *traj;                              // PROPERTY_LAZERROBOT_TRAJ: the patrol trajectory resource ( call Scn_GetPropTrajectory / 0x4d
    TrajFollower follower;                         // the 0x20-byte trajectory follower, init'd with headingBias 0x800, continuousHeading 1, use3dDistance
    Vec3s vel;
    Vec3s step;
    s16 pathHeading;                               // TrajFollower_Step's outHeading slot; written every step and never read - the
    s16 heading;                                   // heading toward the next waypoint, atan2 of the horizontal delta plus half a turn, masked to 0..0xfff
    Vec3s homePos;                                 // the placed position, saved by PostLoadInit and restored
    ScnObject *activator;                          // the sender of the switch message that last toggled the robot
    s16 speed;
};

class Leaf : public ScnBody {
public:
    virtual void PostLoadInit();                   // Leaf_Init (override)
    virtual void Update();                         // Leaf_Update (override)
    s32 respawnTimerMs;                            // Wait after the fall animation ends: Rand_Range(750,2250) ms, decremented by g_dtMs.
    s16 range;                                     // RANGE property: random x/z respawn offset from homePos (900 in Lvl-04, 300 in Lvl-05).
    s32 animating;                                 // 1 while the fall animation should advance.
    Vec3s homePos;                                 // Placed (ground-snapped) position.
};

class LightSpot : public ScnLogic {
public:
    virtual void PostLoadInit();                   // LightSpot_PostLoadInit (override)
    virtual void Update();                         // LightSpot_Update (override)
    virtual void Render(Camera *view);             // LightSpot_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // LightSpot_HandleMessage (override)
    virtual void Reset();                          // LightSpot_Reset (override)
    s32 MsgStub(ScnObject *sender, u32 msgId, void *arg);        /* no callers */
    void Stub();                                                 /* no callers */
    u8 IsPointLit(Vec3s *p);
    u8 _pad040[0x850];
    u16 numSides;
    u16 nbHeightSegs;                              // PROPERTY_LIGHTSPOT_NBHEIGHTSEGS, truncated the same way; also n
    s32 radius;                                    // PROPERTY_LIGHTSPOT_RADIUS: the cone's radius at the floor. Render lays the first ring vertex at +rad
    u32 color;                                     // PROPERTY_LIGHTSPOT_COLOR; not read by any function of this class
    u8 _pad89c[0x960];
    u8 block11fc[16];                              // a 0x10-byte block nothing in this class reads or writes; unk1210 is set to point at it ( add
    u16 unk120c;                                   // set to 0 by PostLoadInit; no reader in this class
    u8 unk120e;                                    // set to 3 by PostLoadInit; no reader in this class
    u8 unk120f;                                    // set to 0 by PostLoadInit, before +0x120e; no reader in this class
    void *unk1210;                                 // set to this + 0x11fc, i.e. it points at the 0x10-byte block that precedes the three fields above (0x
    TrajFollower follower;                         // the 0x20-byte trajectory follower for the beam's patrol path, init'd with speed 200, headingBias 0,
    Vec3s aim;                                     // where the beam currently points on the floor: the patrol step moves it, the loc
    u8 locked;
    ScnObject *robot;                              // the level's single CLASSID_ROBOT (101) object, or NULL when there is not exactly one ( Scena
    ZoneList authBoxes;                            // PROPERTY_LIGHTSPOT_AUTHORIZEDBOX: the boxes the robot must stay inside while the spot follows it, as
};

struct ListNode {
public:
    void *data;                                    // the node's payload: the ScnObject of an object-grid cell list (ObjGrid_QueryGroundY), the o
    ListNode *prev;                                // previous node (List_Remove; List_PushFront zeroes it)
    ListNode *next;                                // next node (List_Remove; ObjGrid_QueryGroundY)
};

class MCardManager : public ScnBody {
public:
    virtual void PostLoadInit();                   // MCardManager_PostLoadInit (override)
    virtual void Update();                         // MCardManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // MCardManager_HandleMessage (override)
    virtual void Reset();                          // MCardManager_Reset (override)
    CamSetup *uiCamera;                            // PROPERTY_MCARDMANAGER_ID_CAMERA (prop 0) via Scn_GetPropCamera: a camera setup record {u16 focal; s1
    u8 state;                                      // 0 idle; 1 start autosave (camera shot mode 2, freeze Ralph, MCard_SetMode(3)); 2 autosave running; 3
    s8 keepCamera;                                 // Latched at the start of state 4 from g_camMode being 9, 0xa or 7 (note: 7 here, unlike the
    u8 exitAfterUi;                                // Written by the state-5 exit dispatch: 0 for Card_StateMachine results 0x2649/0x264a, 1 for 0x264b. I
};

struct MMCKINFO {
public:
    u32 ckid;                                      // Win32 SDK MMCKINFO (mmsystem.h). Chunk id (FOURCC): WaveFile_Open / ResetFile store 'data' 0x6174616
    u32 cksize;                                    // Win32 SDK MMCKINFO (mmsystem.h).
    u32 fccType;                                   // Win32 SDK MMCKINFO (mmsystem.h). Form type (FOURCC) of a RIFF/LIST chunk: 'WAVE' 0x45564157 checked
    u32 dwDataOffset;                              // Win32 SDK MMCKINFO (mmsystem.h). File offset of the chunk data; Open/ResetFile seek to ckRiff.dwData
    u32 dwFlags;                                   // Win32 SDK MMCKINFO (mmsystem.h). MMIO_DIRTY etc.; not read by the game.
};

class Magnet : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Magnet_Init (override)
    virtual void Update();                         // Magnet_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Magnet_HandleMessage (override)
    virtual void Reset();                          // Magnet_Reset (override)
    void PullMover(MoveModifyArg *arg, ScnObject *mover);
    void ReleaseAll();
    void ReleaseObject(ScnObject *obj);
    void UpdateCaughtList(ScnObject **objects, s32 *dist2s, s32 count);
    void UpdateCaughtObject(ScnObject *obj, s32 dist2);
    ScnObject *caught[4];                          // Objects the magnet holds as a mover (they accepted MSG_RIDER_ADD 0x18); the list is kept in step wit
    Vec3s homePos;                                 // Respawn position: pos at Init and at msg 9 arg 1; Reset moves there with SetPosition when in the wor
    u16 humSound;                                  // Looping hum channel handle: Sound_Play(0x92, this, 0xff, 0xb, 0x1000) while anything is caught; Soun
    Vec3s attractPos;                              // Centre of the 400-unit search and the Magnet_PullMover target: pos each frame while attached, plus 7
    u8 onRod : 1;                                  // (bits) bit 0 of the flag byte +0x9a, a one-bit field (byte loads/ANDs and, byte store
    u8 pad9b;                                      // tail padding to sizeof 0x9c
};

class MagnetRod : public CompositeRod {
public:
    virtual void PostLoadInit();                   // MagnetRod_Init (override)
};

struct MailboxFlagBits {
public:
    u8 generated : 1;                              // (bits) bit 0 (1-byte unsigned unit) | 4d3aba..4d3ac4 and 4d3c17..4d3c20 establish byte bitfields at +0x71.
    u8 opened : 1;                                 // (bits) bit 1 (1-byte unsigned unit)
    u8 committed : 1;                              // (bits) bit 2 (1-byte unsigned unit)
};

class Mailbox : public ScnBody {
public:
    virtual void PostLoadInit();                   // Mailbox_Init (override)
    virtual void Update();                         // Mailbox_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Mailbox_HandleMessage (override)
    virtual void Reset();                          // Mailbox_Reset (override)
    void Deliver();
    void Rollback();
    u8 idle;                                       // 1 when no delivery camera is running
    u32 hasRock;                                   // LVL03ROCK != 0 (only the Lvl-03 mailbox with designer boxes)
    ScnObject *rock;                               // Scenaric_FindByIdList(LVL03ROCK); sent msg 0x1B at delivery
    u8 deliveryPending;                            // set to request a delivery; consumed by Mailbox_Update when no cinematic is active
    MailboxFlagBits mbFlags;                       // MailboxFlags: 1 GENERATED_BOXES (no BOX1-4), 2 OPENED, 4 COMMITTED
    ScnObject *boxes[4];                           // container per item: the BOX1..4 designer objects, or the box/FloatingBox objects created at init fro
    u16 itemIds[4];                                // O1..O4 object-link ids (0 = unused slot)
    ScnObject *items[4];                           // the O1..O4 objects, removed from the world at init and handed to the boxes at delivery
    u8 boxRecords[4][24];                          // synthesised WAR records for box_Create, built by (rec, &itemSpawnPos[n], itemId) when !float
    Vec3s itemSpawnPos[4];                         // copy of each item's record+4 (designer position); where generated boxes are added to the world
    ScnObject *opener;                             // the sender of msg 1 (Wolf or Robot). Frozen with msg 0xE when the delivery camera starts, unfrozen w
    u32 floatingBoxes;                             // (u16)FLOATINGBOXES: generated containers are FloatingBox (class 143) instead of box (class 14). Set
    u8 floatingBoxRecords[4][24];                  // synthesised records for FloatingBox_Create, built by when floatingBoxes
    u32 cameraTimeMs;                              // PROPERTY_MAILBOX_TIMERCAMERA (ms)
    u32 cameraTimer;                               // accumulates g_dtMs while the delivery camera runs (double-truncated ms: runs ~4% slow at 60 FPS)
    CamSetup *camera;                              // IDCAMERA list[0] camera resource {u16 focal +0; s16 [3] +2..+6; data +8}, passed to Camera_StartScri
};

struct MapMarker {
public:
    UiQuad sprite;                                 // the IGLCERC_ marker quad; its colour field at +0x00 is the pulse tint and its flags at +0x08 take th
    s16 mapX;                                      // Mailbox property MAPPOSX (offset 28) multiplied by 2
    s16 mapY;                                      // Mailbox property MAPPOSY (offset 32)
    u16 acceptedClassIds[4];                       // class ids this mailbox accepts, resolved from Mailbox properties O1..O4
};

class UiIcon {
public:
#ifdef SDW_MEMBERS_UiIcon
    SDW_MEMBERS_UiIcon
#endif
    void SetColor(u32 rgb);
    void SetEnabled(s32 on);
    void SetHighlighted(s32 on);
    void UiIcon_SetFadeLevel(u8 level);
    void UiIcon_Draw(u16 layer);
    void UiIcon_Setup(const u16 *mainRes, const u16 *overlayRes, const u16 *backRes, s16 x, s16 y, u16 scaleX, u16 scaleY, s32 scaleExtras);
    u8 enabled;                                    // drawn at all: UiIcon_Draw requires +0 and +1.
    u8 highlighted;                                // second UiIcon_Draw gate; Map_ResetSelection clears it
    u8 hasExtraQuads;                              // set by UiIcon_Setup when either the back or overlay bitmap was supplied; UiIcon_Draw draws the overl
    u8 quantity;                                   // item count; UiIcon_Draw prints the "x%u" label only when it is greater than 1
    UiQuad mainQuad;                               // the item bitmap itself; always drawn
    UiQuad overlayQuad;                            // grey (0x808080) overlay drawn under the main quad when hasExtraQuads is set
    UiQuad backQuad;                               // grey background/slot bitmap
    u32 unk40;                                     // never read or written by UiIcon_Setup / UiIcon_Draw / UiIcon_SetFadeLevel or by the Map; present bec
};

class Map {
public:
#ifdef SDW_MEMBERS_Map
    SDW_MEMBERS_Map
#endif
    ScnObject *FindCombineTarget(u16 classA, u16 classB);
    ScnObject *GetSelectedObject();
    char *GetHelpText();
    s32 IsRowVisible(s16 row);
    s32 IsSelectedItemAssembled();
    u16 FindOrAddRow(u16 classId);
    u16 FindRow(u16 classId);
    void AcquireItem(ScnObject *obj);
    void ActionNext();
    void ActionPrev();
    void BeginModeTransition(u8 nextMode);
    void CombineItems();
    void CursorNext();
    void CursorPrev();
    void Draw();
    void DrawHelpTooltip();
    void HighlightLocationsForZone(u16 classId);
    void InitObject();
    void Open();
    void ReleaseItem(ScnObject *obj);
    void RemoveRow(u16 classId);
    void ResetSelection();
    void SelectMenu_RestoreFlags();
    void SetPanelRect(s16 *outRect, UiQuad *src, u16 h, u16 w);
    void SortRowsByOwned();
    void SplitItem();
    void UpdateState();
    char *helpText;                                // description string for the item under the cursor, refreshed by Map_GetHelpText
    ScrollText scrollText;                         // the help-text scroller of mode 4: Map_UpdateState calls ScrollText_Init / ScrollList_Update on this+
    u32 pulseColorFast;                            // cosine-modulated tint 0x808080<->0x3040E0 used for the mailbox markers
    u32 pulseColorSlow;                            // slower cosine tint passed to the MapLocation "you are here" marker draw
    u8 _pad020[0x4];
    s16 panelFrom[4];                              // start rect of the animated description panel as one contiguous {x,y,w,h}: lerps +0x24 and +
    s16 panelTo[4];                                // target rect of the animated description panel, one contiguous {x,y,w,h}; is called on +0x2c
    s16 pulsePhaseFast;                            // 12-bit angle advanced by g_dtRawMs*12 each frame
    s16 pulsePhaseSlow;                            // 12-bit angle advanced by g_dtRawMs*4 each frame
    s16 scrollTop;                                 // index of the first visible inventory row
    s16 cursorIndex;                               // selected inventory row (absolute, not windowed)
    u16 selectedClassId;                           // class id of the item under the cursor; fed to Map_GetHelpText and Map_HighlightLocationsForZone
    s16 prevCursorIndex;                           // cursor position before the current move, used to repaint the old row
    s16 actionIndex;                               // selected action button 0=HELP 1=USE 2=COMBINE/SPLIT 3=EXIT
    s16 prevActionIndex;                           // previous action button, repainted to 0x808080
    s16 combinePartnerIndex;                       // row latched as the first half of a combine while mode 3 is active
    s16 helpTimerMs;                               // tooltip countdown, armed to 3000 and decremented by g_dtRawMs
    s32 markerTimerMs;                             // mailbox-highlight countdown, armed to 6000 and decremented by g_dtRawMs
    s16 listBaseY;                                 // virtual-screen Y of the first item slot; starts 0x80 then shifted up by half the list height
    u16 autoRepeatMs;                              // auto-advance timer for the transient modes (bit 7 of mode set); any d-pad press forces it to 0x96
    u16 wipeHeight;                                // height of each curtain bar during the open/close wipe, stepped by +7 per FRAME
    u8 swapPending;                                // 1 while a combine partner has been picked and the second item is being chosen
    u8 _pad053[0x1];
    u8 mode;                                       // sub-mode inside state 3: 1 item list, 2 action row, 3 combine pick, 4 help scroll; bit 0x80 = transi
    u8 pendingMode;                                // mode committed when autoRepeatMs expires
    u8 itemCount;                                  // number of inventory classes present in this level
    u8 visibleCount;                               // rows shown at once, min(4, itemCount)
    u8 locationCount;                              // number of Mailbox markers found (max 5)
    u8 phaseCounter;                               // per-FRAME counter for the opening (15 down) and closing (15 up) holds; also drives the global fade l
    u8 _pad05a[0x1];
    s8 scrollDir;                                  // mode-4 help scroll direction for this frame: 0, -1 while UP is held, +1 while DOWN is held (Pad_Menu
    u8 closeCounter;                               // per-FRAME counter for state 5; at 5 it calls SelectMenu_RestoreFlags, at 10 it moves to state 6
    u8 state;                                      // select-menu state 0 closed, 1 opening wipe, 2 opening hold, 3 interactive, 4 closing hold, 5 closing
    u16 *resIcons[13];                             // resource id lists: [0..7] = four action-button icon pairs (HELPB/HELPA, USEDB/USEDA, COMBB/COMBA, EX
    char *actionText[4];                           // button labels, indexed by actionIndex: UI strings 0x24, 0x23, 0x25, 0x03
    char *actionTextSplit;                         // UI string 0x49, substituted for actionText[2] when the selected item is composite
    MapMarker markers[5];                          // cached Mailbox markers, 0x20 bytes each
    u16 inventoryClassIds[16];                     // class id of each inventory row, in scan order
    UiQuad backdrop[5];                            // the DAV_IDI_MAP bitmap laid out as a five-piece nine-slice: top bar h=0x30, left column 0x60x0xA1, r
    UiQuad scrollArrow[2];                         // up/down list arrows drawn from IGLFLEC_ at x=0x18, above and below the item list
    UiIcon actionSlots[4];                         // the four action buttons, 0x44 bytes each: +0x00 u8 enabled, +0x01 u8 highlighted, +0x04 u32 tint
    UiIcon itemSlots[4];                           // the four visible inventory slots, same 0x44 layout
};

struct MapLocSlot {
public:
    ZoneList zones;                                // zone list {boxes, count}: Scn_FindIdList(BOXnn): zone boxes tested XZ-only against the Wolf | count
    s16 mapX;                                      // BOXnnPOSX << 1: marker x in the 512-wide virtual HUD space
    s16 mapY;                                      // BOXnnPOSY (not scaled)
};

class MapLocation : public ScnLogic {
public:
    virtual void PostLoadInit();                   // MapLocation_Init (override)
    s32 DrawWolfMarker(u8 fade, u32 color);
    void AddSlot(s32 boxOffset, s32 xOffset, s32 yOffset);
    void *props;                                   // copy of record (+0x20)
    UiQuad marker;                                 // HUD quad for the Wolf icon (color +0x44, drawFlags +0x4c, x/y +0x50/+0x52); drawn at layer 0xb
    uptr *markerBitmap;                             // IdList_FindWithCount(0x8E = DAV_IDI_IGLCOYO1). Its first dword is the bitmap passed to UiQuad_SetFro
    MapLocSlot slots[15];                          // compacted BOXnn slots (12 bytes each)
    u16 slotCount;                                 // number of valid slots
};

struct MarvinFlagBits {
public:
    u8 solved : 1;                                 // (bits) bit view of Marvin.dialogueFlags (+0x7d): bit 0 (and cl,1 reads; and 0xfe clears)
    u8 wolfFrozen : 1;                             // (bits) bit 1 (shr 1; or 2 / and 0xfd)
    u8 interrupted : 1;                            // (bits) bit 2 (shr 2; or 4 / and 0xfb)
    u8 insideCineBox : 1;                          // (bits) bit 3 (shr 3; or 8 / and 0xf7)
    u8 reserved : 4;                               // (bits) remaining bits
};

class Marvin : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Marvin_PostLoadInit (override)
    virtual void Update();                         // Marvin_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Marvin_HandleMessage (override)
    virtual void Reset();                          // Marvin_Reset (override)
    void SetState(u8);
    u8 state;                                      // state byte
    MarvinFlagBits dialogueFlags;                  // flag byte; bit view MarvinFlagBits (bits 0-3 set with or, cleared with and, read by shr/and 1)
    u8 tauntIndex;
    char *mockeryLine[2];                          // MOCKERYID1/2 (props 0x10/0x14) via Text_GetClassString. DEFECT: the empty-string checks for the SUCC
    s32 mockeryVoice[2];                           // voices of the two mockery lines
    u8 successIndex;
    char *successLine[2];                          // the two success lines
    s32 successVoice[2];                           // voices of the success lines
    Box *cineBox;                                  // the cinematic trigger box
};

struct MenuState {
public:
    Menu *current;                                 // the open menu (g_curMenu)
    s16 itemCount;                                 // item count of the open menu (g_menuItemCount)
    s16 capacity;                                  // g_menuCapacity
    s16 cursor;                                    // g_menuCursor
    s16 lastCursor;                                // g_menuLastCursor
    s16 currentPage;                               // g_menuCurPage
    s16 previousPage;                              // g_menuPrevPage
    u8 capture;                                    // g_menuCapture
    u8 previousCapture;                            // g_menuCapturePrev
    u8 cursorRow;                                  // g_menuCursorRow
    u8 reserved13;
    s32 dirty : 1;                                 // (bits) bit 0 of the flags word, a signed one-bit field: the menu needs redrawing (Menu_IsDirty / Menu_SetDi
    s32 flagsRest : 31;                            // (bits) remaining bits of the flags word
    MenuPage *pages;                               // g_menuPages
    u32 reserved1c;                                // unknown final dword retained as storage
};

class MeshAnimFrame {
public:
#ifdef SDW_MEMBERS_MeshAnimFrame
    SDW_MEMBERS_MeshAnimFrame
#endif
    virtual ~MeshAnimFrame();                         // MeshAnimFrame_VectorDeletingDtor
    u16 halfDurationMs;                            // first u16 of the frame's track record (BsFile_ReadTrack12); AnimMesh doubles it for the fra
    MeshPartPose *poses;                           // array of the sequence's part poses, new[]'d by BsFile_ReadAnimNames, filled by BsFile_Rea
};

class MeshAnimSeq {
public:
#ifdef SDW_MEMBERS_MeshAnimSeq
    SDW_MEMBERS_MeshAnimSeq
#endif
    virtual ~MeshAnimSeq();                           // MeshAnimSeq_VectorDeletingDtor
    char name[8];                                  // 8-char animation name, or "No anim" for an empty slot (BsFile_ReadAnimNames)
    MeshAnimFrame *frames;                         // new MeshAnimFrame[frameCount] (BsFile_ReadAnimNames)
    u32 frameCount;                                // the u16 read after the name (BsFile_ReadAnimNames)
};

class MeshPart {
public:
#ifdef SDW_MEMBERS_MeshPart
    SDW_MEMBERS_MeshPart
#endif
    virtual ~MeshPart();                              // MeshPart_VectorDeletingDtor
    void BlendThenSetTarget(MeshPartPose *pose, float t);
    void SetPose(MeshPartPose *pose);
    void SetTargetPose(MeshPartPose *pose);
    void BuildMatrices(Mat44 *parentMat, Mat44 *parentScaleMat, float t);
    float WrapAnglePi(float a);
    float WrapAngle2Pi(float a);
    u32 parentIndex;                               // index of the parent part; 0xffff (none) from MeshPart_Ctor, the file's u16 from BsFile_Type
    u32 field_08;                                  // zeroed by MeshPart_Ctor; no reader in this range
    u32 vertexCount;                               // vertices of this part (u16 from the file); zeroed by MeshPart_Ctor
    Mat44 *matrix;                                 // the part's final matrix = *scaleMatrix * *localMatrix, written by MeshPart_BuildMatrices; t
    Mat44 *localMatrix;                            // rotation * translation * parent's localMatrix, written by MeshPart_BuildMatrices; children
    Mat44 *scaleMatrix;                            // SetScale(lerped scale); children receive it as parentScale (its diagonal scales their of
    float offset[3];                               // joint offset from the parent, from the file (s16 via Bs_S16ToFloat); scaled by the parent scale diag
    float rot[3];                                  // current rotation (radians), lerped toward targetRot through the wrap helpers by MeshPart_BlendThenSe
    float pos[3];                                  // current translation
    float scale[3];                                // current scale, 1.0f at construction
    float targetRot[3];                            // pose being blended toward (MeshPart_SetTargetPose)
    float targetPos[3];
    float targetScale[3];                          // 1.0f at construction
};

class MeshPartPose {
public:
#ifdef SDW_MEMBERS_MeshPartPose
    SDW_MEMBERS_MeshPartPose
#endif
    virtual ~MeshPartPose();                          // MeshPartPose_VectorDeletingDtor
    float rot[3];                                  // Euler angles in radians, read by BsFile_ReadJointTable; cleared to 0 fi
    float pos[3];                                  // translation, read by BsFile_ReadJointTable; cleared to 0 first
    float scale[3];                                // scale, 1.0 unless present (BsFile_ReadJointTable)
};

class MineDetector : public ScnMobile {
public:
    virtual void PostLoadInit();                   // MineDetector_Init (override)
    virtual void Update();                         // MineDetector_Update (override)
    virtual void Render(Camera *view);             // MineDetector_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // MineDetector_HandleMessage (override)
    virtual void Reset();                          // MineDetector_Reset (override)
    void DrawGauge();
    void InitCollBox();
    void SetState(u8 newState);
    ContactInfo contact;                           // Collide_ResolveMove output while falling (state 1); floorObj +0x7c, movableObj +0x84
    CollBox collBox;                               // private collision box for the fall: min (-10,-20,-10), max (10,0,10) (MineDetector_InitCollBox); fla
    Vec3s homePos;                                 // spawn position (Init after snap; msg 9 arg 1 = container release); Reset returns here
    u16 scanSound;                                 // handle of the scanning sound 0x102; stopped by every SetState
    s32 reading;                                   // distance to the selected mine in world units (<= 200 shows on the gauge); INT_MAX = nothing
    u8 state;                                      // MineDetectorState
    u8 regionCount;                                // number of registered region GroundMines (msg 0x2280, max 4); zeroed only in MineDetector_Create
    u8 scanning : 1;                               // (bits) bit 0 of the flag byte +0xb6, a one-bit field (byte bitfield operations): bit 0
    u8 padB7;                                      // padding
    ScnObject *regions[4];                         // region-mode GroundMines that registered with msg 0x2280; each is asked msg 0x5a (ray distance) every
};

class MirrorManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // MirrorManager_Init (override)
    virtual void Update();                         // MirrorManager_Update (override)
    virtual void Render(Camera *view);             // MirrorManager_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // MirrorManager_HandleMessage (override)
    ScnObject *objects[9];                         // reflected objects: [0] = g_pWolf (filled by the update once wolfLinked == 0), [1..] = the n
    ZoneList zones;                                // zone list {boxes, count}: IDBOX id-list (NULL when IDBOX = 0) | count (+4): count for boxes
    u32 mirrorColor;                               // MIRRORCOLOR
    s16 mirrorPlaneVert2x;                         // not groundAltitude2x: the mirror constant K passed to ScnBody_RenderTinted, which keeps it in g_mirr
    s16 colorIntensity;                            // COLORINTENSITY (level data 2048)
    s8 objectCount;                                // entries used in objects[] (starts at 1: slot 0 is the Wolf)
    u8 wolfLinked;                                 // 0 until the update stores g_pWolf in objects[0]
};

struct MltHeader {
public:
    char version[4];                               // 'v1.2' in the shipped Levels/*/*.MLT files; never checked by Load_MLT
    u16 blockCount;                                // number of language blocks (7 in the shipped files); Load_MLT warns when the UNMAPPED config language
    u16 listCount;                                 // string lists per language block (0xa9 in Lvl-14.MLT); copied to StringBank.listCount
};

struct Model {
public:
    u8 _pad000[0xc];
    SDW_WARPTR(ModelBoxList) boxes;  // counted box list {u32 count; CollBox[count]}, or NULL (read; ScnObject_GetFirs
    SDW_WARPTR(void) animTable;                               // {u32 directCount; ptr direct[directCount]; u32 mappedCount; ptr mapped[mappedCount]}: Anim_Start use
    SDW_WARPTR(ModelJoint) joints;                            // joint hierarchy, 10 bytes per joint
    u16 nbJoints;
};

struct ModelBoxList {
public:
    u32 count;                                     // number of boxes (compared unsigned: jbe and)
    CollBox boxes[1];                              // CollBox[count], object-local; 16-byte stride (add 0x10)
};

struct ModelJoint {
public:
    u16 parent;                                    // parent joint index; its matrix and pose scale are used for this joint
    Vec3s offset;                                  // rest offset from the parent in model units (multiplied by the parent's pose scale)
    u16 vertexCount;                               // vertices of this part; parts are drawn as consecutive ranges starting at part 1 with vertex 0; part
};

struct MonolitheRider {
public:
    Vec3s offset;                                  // rider offset from the Monolithe, read as signed shorts and written
    u16 pad;                                       // padding to align the pointer; never accessed
    ScnObject *object;                             // the riding object, stored at +0xc of the record
};

class Monolithe : public ScnBody {
public:
#ifdef SDW_MEMBERS_Monolithe
    SDW_MEMBERS_Monolithe
#endif
    virtual void PostLoadInit();                   // Monolithe_PostLoadInit (override)
    virtual void Update();                         // Monolithe_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Monolithe_HandleMessage (override)
    virtual void Reset();                          // Monolithe_Reset (override)
    void SetState(u8 newState);
    u8 monoState;                                  // name and access width as src/objects/monolithe.cpp uses the field
    u8 previousState;                              // name and access width as src/objects/monolithe.cpp uses the field
    u16 riderCount;                                // name and access width as src/objects/monolithe.cpp uses the field
    u16 spinTicks;                                 // elapsed game-clock ticks through the sixteen-reference-step turn
    u16 ghostCount;                                // name and access width as src/objects/monolithe.cpp uses the field
    u8 _pad06c[0x4];
    s32 freezeAccepted;                            // name and access width as src/objects/monolithe.cpp uses the field
    s32 freezeEnabled;                             // name and access width as src/objects/monolithe.cpp uses the field
    Vec3s homeRot;                                 // name and access width as src/objects/monolithe.cpp uses the field
    Vec3s spinRot;                                 // name and access width as src/objects/monolithe.cpp uses the field
    CamSetup *uiCamera;                            // name and access width as src/objects/monolithe.cpp uses the field
    ScnObject *ghosts[10];                         // name and access width as src/objects/monolithe.cpp uses the field
    ScnObject *freezeTarget;                       // who gets the UI freeze during the turn: g_pWolf (stored) or a Robot (classId 0x65) found in
    CollBox *sheepBox;                             // name and access width as src/objects/monolithe.cpp uses the field
    MonolitheRider riders[10];                     // 10 entries of 12 bytes {Vec3s offset; u16 pad; ScnObject *obj}; the acceptance test is Vec3s_DistXZ
    u16 spinSound;                                 // name and access width as src/objects/monolithe.cpp uses the field
};

class Mouse : public InputDevice {
public:
#ifdef SDW_MEMBERS_Mouse
    SDW_MEMBERS_Mouse
#endif
    virtual ~Mouse();                                 // Mouse_ScalarDeletingDtor
    virtual long SetAcquired(u8 acquire);          // Mouse_SetAcquired (override)
    virtual long Update();                         // Mouse_Update (override)
};

struct MoveModifyArg {
public:
    Vec3s delta;                                   // the mover's displacement for this step, which a registered rider may change (BlackHole_ApplyPull rea
    u16 flag0 : 1;                                 // (bits) bit 0 of the flags word; no reader identified
    u16 noPull : 1;
    u16 flag2 : 1;                                 // (bits) bit 2: ScnControllable::ApplyMoveModifiers sets it from its fifth argument (or 4 / and 0xfffb on the
    u16 otherFlags : 13;                           // (bits) remaining flag bits
    Vec3s velocity;                                // the mover's velocity; BlackHole_ApplyPull adds signed 16-bit components to it
};

struct MoveRecord {
public:
    s16 maxSpeed;                                  // target speed at full stick deflection, u/s (Mobile_Steer: maxSpeed * stickMag >> 8)
    s16 acceleration;                              // Math_ApproachLinear rate when speeding up
    s16 deceleration;                              // Math_ApproachLinear rate when slowing; Mobile_Steer brakes at (acceleration+deceleration)/2 when the
    s16 verticalRate;                              // not read by the steering functions; name
    s32 maxTravelTurnRate;                         // angular speed limit for the motion heading
    s32 maxFacingTurnRate;                         // angular speed limit for the facing
    s32 travelTurnAcceleration;                    // angular acceleration for the motion heading (also passed as the facing's deceleration)
    s32 facingTurnAcceleration;                    // angular acceleration for the facing
    s32 returnAngleThreshold;                      // a stick further than this from the motion heading makes Mobile_Steer return 1
};

struct SamEdgeNormal {
public:
    s16 x;                                         // x of an edge's 4.12 unit normal (NavNode.edgeNormal, Sam.followEdgeNormal); SamNav_DistToEdge reads
    s16 z;                                         // z of the normal
};

struct NavNode {
public:
    s16 x;                                         // node x; +2 z; +4 y
    s16 z;                                         // node Z
    s16 groundY;                                   // ground query (Coll_BoxGroundQuery) at build time, NOT the trajectory third value: a new nod
    NavNode *sons[8];                              // neighbour pointers
    union {
        s16 edgeNormal[8][2];                          // per-edge 4.12 unit normal (nx, nz)
        SamEdgeNormal edgeNormals[8];              // edgeNormal as SamEdgeNormal pairs (sam.cpp copies one into Sam.followEdgeNormal as a struct)
    };
    s16 bbox[4];                                   // minX, minZ, maxX, maxZ over node + neighbours, grown by 300
    u8 nbSons;                                     // neighbour count
    NavNode *parent;                               // A* parent
    u32 g;                                         // cost so far; +0x5c h (Manhattan); +0x60 f
    u32 h;                                         // Manhattan distance to the search goal (NavPath_BeginSearch)
    u32 f;                                         // g + h; the open list is kept sorted on this (NavPath_OpenListInsert compares +0x60)
    u8 listState;                                  // 0 none, 1 open, 2 closed; +0x68 next, +0x6c prev
    NavNode *next;                                 // next in the search list (walked by NavPath_ClearSearch)
    NavNode *prev;                                 // previous in the search list; completes the 0x70 stride that g_samNavNodes[120] uses
};

struct NavSearch {
public:
    NavNode *head;                                 // Sorted list of open and closed nodes
    u32 active;                                    // One after BeginSearch and zero after ClearSearch
    s16 goalX;                                     // Search goal X
    s16 goalZ;                                     // Search goal Z
};

struct ObjGridHeader {
public:
    u16 dimX;                                      // cells along X
    u16 dimZ;                                      // cells along Z
    s16 originX;                                   // grid origin X
    s16 originZ;                                   // grid origin Z
    u16 shift;                                     // log2 cell size
};

struct ObjMgrEntry {
public:
    ScnObject *obj;                                // managed object (Scenaric_FindByRecord)
    u16 savedUpdateBits;                           // obj.flags & 0x6000 saved when the camera left; restored on re-entry when keepUpdating == 0. Uninitia
    u16 pad6;                                      // padding (stride 8)
};

class ObjectManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // ObjectManager_Init (override)
    virtual void Update();                         // ObjectManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // ObjectManager_HandleMessage (override)
    void AddIdList(u16 id);
    void SetObjectsActive(Box *camBox);
    ZoneList manageBoxes;                          // zone list {boxes, count}: MANAGEINBOXES id-list (Scn_FindIdList). Tested against g_camPos (the camer
    ObjMgrEntry entries[50];                       // managed objects from OBJECT01..OBJECT20 id-lists; appended without a bounds check
    s16 entryCount;                                // number of valid entries
    u8 camZoneState;                               // 0 before the first update, 1 camera outside every box, 2 inside one (ObjMgrCamZone)
    u32 keepUpdating;                              // ACTION & 1. 0: outside the boxes objects are hidden (4) AND frozen (0x2000), and their 0x6000 bits a
};

class PackJpeg {
public:
#ifdef SDW_MEMBERS_PackJpeg
    SDW_MEMBERS_PackJpeg
#endif
    s32 Next();
    s32 Open(const char *);
    s32 Prev();
    s32 SelectAndFetch(const char *, void *);
    s32 SelectByName(const char *);
    s32 SetIndex(u32);
    u32 GetCount();
    void BeginImage();
    void Close();
    void DecodeToTextures(s32);
    void DrawImage();
    void ReleaseImage();
    u8 header[8];                                  // file header: Open reads 12 bytes straight over the object (header + count); no vptr
    u32 count;                                     // number of images in the pack (read)
    FILE *file;                                    // open file handle (the CRT FILE * of fopen; fread/fseek/fclose take it)
    u32 opaqueValue;                               // its address is passed; role not established
    PackJpegEntry *entries;                        // directory of the pack
    PackJpegImage *image;                          // the decoded current image
    u32 index;                                     // current image index (read)
};

struct PackJpegEntry {
public:
    u32 fileOffset;                                // offset of the image in the pack file
    u8 unknown04[5];                               // not established
    char name[15];                                 // image name, scanned by SelectByName; entry stride 24
};

struct PackJpegImage {
public:
    u8 unknown00[24];                              // not established
    s16 width;                                     // image width
    s16 height;                                    // image height
    s32 byteSize;                                  // pixel data size; halved with a signed divide (cdq; sub; sar)
    u8 unknown20[8];                               // not established; the record is allocated as 0x28 bytes (push 0x28)
};

struct PadTypeLen {
public:
    u8 len : 4;                                    // (bits) PadFrame.typeLen low nibble: data length in halfwords (PS1 libpad)
    u8 type : 4;                                   // (bits) high nibble: controller type (7 = analog)
};

struct PadFrame {
public:
    s8 status;                                     // PS1 receive-buffer byte 0: 0 = read OK, 1 = InputMgr_Poll failed.
    PadTypeLen typeLen;                            // PS1 byte 1 as a bitfield {len:4, type:4} (PadTypeLen): type = controller id (4 digital, 7 analog), r
    u16 buttons;                                   // active-low PS1-layout button word
    u8 rightX;                                     // right stick X 0..255 (0x80 centre), from InputMgr::rightX
    u8 rightY;                                     // right stick Y 0..255 (0x80 centre)
    u8 leftX;                                      // left stick X (0x80 centre)
    u8 leftY;                                      // left stick Y (0x80 centre)
};

struct PadRepeat {
public:
    s16 timerMs;                                   // ms the value has been held; signed
    u16 lastValue;                                 // value seen last call
    u16 output;                                    // the value on the first frame and on each repeat, 0xffff (nothing) in between; repeats OR in 0xff06
};

class Pad {
public:
#ifdef SDW_MEMBERS_Pad
    SDW_MEMBERS_Pad
#endif
    s32 JustConnected();
    s32 TypeChanged();
    u16 GetChangedPress(u16 ignoreMask);
    void AnalogToDpadBits();
    void AutoRepeat(PadRepeat *rep, u16 value);
    void Latch();
    void Latch_Dup();
    void OnConnected_stub();
    void Open(int port);
    void ResetState();
    void SetActuator(int on);
    void Stub_55f078(u32 arg);
    void Stub_55f09d();
    s32 IsConnected();                                           /* Pad_IsConnected */
    void Rumble_stub(s32 durationMs, const u8 *pattern, s16 strength); /* empty on PC */
    PadFrame raw;                                  // PS1-style receive buffer (34 bytes on the PS1: prev starts at 0x22); only the first 8 used. Filled b
    u8 _pad008[0x1a];
    PadFrame prev;                                 // previous latched frame; struct-copied from cur by Pad_Latch
    PadFrame cur;                                  // current latched frame; struct-copied from raw by Pad_Latch
    u16 menuCur;                                   // = g_padMenuCur: raw.buttons with the stick folded in as d-pad bits (Pad_AnalogToDpadBits 0x
    u16 menuPrev;                                  // = g_padMenuPrev: previous menuCur
    PadRepeat btnRepeat;                           // Pad_AutoRepeat state for cur.buttons
    PadRepeat menuRepeat;                          // Pad_AutoRepeat state for menuCur; output +0x40 = g_padMenuRepeat
    u32 state44;                                   // cleared by Pad_ResetState and on every analog Pad_DetectType; no reader found
    u32 state48;                                   // cleared with +0x44; no reader found
    u32 state4c;                                   // cleared with +0x44; no reader found
    u16 state50;                                   // set to 0x1000 (1.0 in 4.12) by Pad_ResetState
    u8 _pad052[0x2];
    u8 state54;                                    // cleared by Pad_ResetState / analog detect
    u8 _pad055[0x2];
    u8 padType;                                    // controller type id: 4 digital (keyboard/digital device), 7 analog; source of the raw type nibble
    u8 padTypeReq;                                 // written alongside +0x57 (4/7); Input_Init tests == 7 to set +0x5b
    u8 state59;                                    // cleared by Pad_ResetState and analog detect; no reader found
    u8 state5a;                                    // cleared by Pad_ResetState and analog detect; no reader found
    u8 actuatorEnable;                             // set 1 by Input_Init when +0x58 == 7, cleared on pause open and on every analog Pad_DetectType, resto
    u8 state5c;                                    // cleared by Pad_ResetState; no reader found
    u8 state5d;                                    // cleared by Pad_ResetState; no reader found
    u32 state60;                                   // cleared by Pad_ResetState / analog detect
};

struct PadRecHeader {
public:
    u8 version;                                    // set 3 by PadRec_Save
    u8 padType;                                    // set 7 by PadRec_Save
    u16 remap[6];                                  // button map snapshot, slots 0xe 0xf 0xc 0xd 0xb 0xa (Input_SetMode)
    u32 dataLen;                                   // recorded data length (PadRec_Save)
    u32 gameTimeMs;                                // g_gameTimeMs stamped by PadRec_Save
    u8 reserved[16];                               // rest of the 0x28-byte header (PadRec_Save adds 0x28)
};

class PathFollower {
public:
    s32 GetSegmentLength();
    void BeginSegment();
    void Start(s32 speed);
    u8 Step();
    u8 StepSmooth();
    Trajectory *path;                              // the Trajectory it follows ({u16 count; Vec3s pts[count]}): PathFollower_Start reads pts[0]
    s32 speed;                                     // units per second: PathFollower_Start's argument; turns segment length * 1000 / s
    Vec3s pos;                                     // the current position: Vec3s_LerpToTarget(&pos, points[fromIdx], t) every step; bipbip mov
    Vec3s segEnd;                                  // the point the current segment runs to: points[toIdx], or with smooth set the halfway point towards p
    Vec3s segStart;                                // points[fromIdx], copied by Start and by Step when it moves to the next segment (0
    u16 heading;                                   // 12-bit heading of the current segment, from atan2 in (& 0xfff); bipbip faces it
    u16 smooth;                                    // 1 = cut corners through the halfway point; zeroed by Start and on every new s
    u16 fromIdx;                                   // index of the segment's start point: 0 from Start, +1 per segment
    u16 toIdx;                                     // index of the segment's end point: 1 from Start, +1 per segment; Step returns 2 instead of advancing
    u16 rate;                                      // t per 1024 ms: / (segment ms); t += rate * g_dtMs / 1024 per step ( -0
    s16 t;                                         // progress along the segment, 0..0x1000
};

struct PerfumeFlagBits {
public:
    u8 blown : 1;                                  // (bits) bit 0 (1-byte unsigned unit) | 4da418..4da439 prove two byte bitfields in the generated +32d storage
    u8 registered : 1;                             // (bits) bit 1 (1-byte unsigned unit)
};

struct PerfumeScentReport {
public:
    u16 classId;                                   // class id of the reporting object
    u16 proximity;                                 // its proximity value
};

class Perfume : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Perfume_Init (override)
    virtual void Update();                         // Perfume_Update (override)
    virtual void Render(Camera *view);             // Perfume_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Perfume_HandleMessage (override)
    virtual void Reset();                          // Perfume_Reset (override)
    InlineEmitter32 scentFx;                       // 32-slot perfume-particle emitter with inline storage: the factory sets slotPool = +0xa0, particles =
    Vec3s emitPos;                                 // particle source, rewritten every Update: pos with y - 40 (40 units up). The drift band reference hei
    Vec3s homePos;                                 // spawn position (Init; msg 9 arg 1); Reset returns here unless the item is KEPT
    u8 state;                                      // PerfumeState: 0 inactive (delivered, held or reset), 1 emitting (after a placed drop, msg 5 flag bit
    PerfumeFlagBits perfumeFlags;                  // PerfumeFlags: bit 0 blown this frame (msg 0x14; consumed by Update), bit 1 scent source registered w
    u16 pad32e;                                    // padding, never accessed
    s32 blowTime;                                  // accumulated blowing in 1/4096 s: += g_dt while blown, -= g_dt otherwise, clamped 0..0x471c. Scent ra
    s16 blowHeading;                               // facing of the last Fan holder that blew it (msg 0x14 arg); the drift direction of the particles and
    PerfumeScentReport lastReport;                 // PerfumeScentReport: classId = class of the nearest object that reported smelling the perfume (msg 0x
    u16 pad33a;                                    // padding, never accessed
    u32 reportTime;                                // g_gameTime of the last stored report (the relay is sent only while it is <= 0x800 old); dword-zeroed
};

class Pipe : public ScnBody {
public:
    virtual void PostLoadInit();                   // Pipe_PostLoadInit (override)
    virtual void Update();                         // Pipe_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Pipe_HandleMessage (override)
    virtual void Reset();                          // Pipe_Reset (override)
    void SetState(u8 newState);
    CollBox *mouths[2];                            // name and access width as src/objects/pipe.cpp uses the field
    ZoneList movementZones;                        // name and access width as src/objects/pipe.cpp uses the field
    CollBox workBox;                               // 16-byte scratch Box, loaded THREE times per state-4 pass: the scanned mouth, then pos+myBox (which f
    u8 _pad084[0x10];
    CollBox *modelBox;                             // name and access width as src/objects/pipe.cpp uses the field
    ScnObject *ball;                               // name and access width as src/objects/pipe.cpp uses the field
    Vec3s entryPos;                                // name and access width as src/objects/pipe.cpp uses the field
    Vec3s launchDir;                               // name and access width as src/objects/pipe.cpp uses the field
    Vec3s exitPos;                                 // name and access width as src/objects/pipe.cpp uses the field
    Vec3s centerPos;                               // name and access width as src/objects/pipe.cpp uses the field
    s32 forbidX;                                   // name and access width as src/objects/pipe.cpp uses the field
    s32 forbidZ;                                   // name and access width as src/objects/pipe.cpp uses the field
    s32 pushedThisFrame;                           // set 1 by every accepted push but cleared ONLY while state == 1 â€” jne skips the 0
    u8 occupied;                                   // name and access width as src/objects/pipe.cpp uses the field
    u8 armed;                                      // name and access width as src/objects/pipe.cpp uses the field
    u8 mouthIndex;                                 // name and access width as src/objects/pipe.cpp uses the field
    u8 objectIndex;                                // name and access width as src/objects/pipe.cpp uses the field
    u8 pipeState;                                  // name and access width as src/objects/pipe.cpp uses the field
    u8 exitAxis;                                   // name and access width as src/objects/pipe.cpp uses the field
    s32 exitBoxSpan;                               // Vec3s_DistXZ of the EXIT activation box's min/max, not of the pipe's own collision box - workBox is
    u32 launchDelay;                               // name and access width as src/objects/pipe.cpp uses the field
    u32 launchStart;                               // name and access width as src/objects/pipe.cpp uses the field
    u16 soundHandle;                               // handle of the push rumble (Sound_Play 0x106, vol 0xff, flags 2 â€” the 4th argument is SoundPlayFlags,
};

class Pipe2 : public ScnBody {
public:
    virtual void PostLoadInit();                   // Pipe2_PostLoadInit (override)
    virtual void Update();                         // Pipe2_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Pipe2_HandleMessage (override)
    virtual void Reset();                          // Pipe2_Reset (override)
    void MoveBoxWithSpin(CollBox *box, Vec3s offset, Vec3s pivot, s16 angle);
    void SetState(u8 newState);
    CollBox *mouths[2];                            // name and access width as src/objects/pipe2.cpp uses the field
    CollBox queryBox;                              // 16-byte scratch Box: the mouth being scanned, then the model box, then the exit mouth; handed to Obj
    CollBox savedMouth;                            // name and access width as src/objects/pipe2.cpp uses the field
    CollBox *modelBox;                             // name and access width as src/objects/pipe2.cpp uses the field
    u8 _pad090[0x20];
    CollBox *originalMouths[2];                    // name and access width as src/objects/pipe2.cpp uses the field
    ScnObject *ball;                               // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s entryPos;                                // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s launchDir;                               // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s exitPos;                                 // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s centerPos;                               // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s homePos;                                 // name and access width as src/objects/pipe2.cpp uses the field
    u8 _pad0da[0xc];
    Vec3s mouthCenter1;                            // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s mouthCenter2;                            // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s turnOffset1;                             // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s turnOffset2;                             // name and access width as src/objects/pipe2.cpp uses the field
    Vec3s turnPivot;                               // name and access width as src/objects/pipe2.cpp uses the field
    u8 occupied;                                   // name and access width as src/objects/pipe2.cpp uses the field
    s32 armed;                                     // gate on the whole state machine ( cmp/je at the top of Update). Set by msg 0x66 from CanonSi
    s32 wolfFrozen;                                // name and access width as src/objects/pipe2.cpp uses the field
    u8 mouthIndex;                                 // name and access width as src/objects/pipe2.cpp uses the field
    u8 objectIndex;                                // name and access width as src/objects/pipe2.cpp uses the field
    u8 pipeState;                                  // name and access width as src/objects/pipe2.cpp uses the field
    u8 exitAxis;                                   // name and access width as src/objects/pipe2.cpp uses the field
    s16 spinAngle;                                 // visual turn accumulator, +0x20 per frame, ending on `spinAngle == 0x400` ( jne â€” exact equal
    s16 unk116;                                    // zeroed beside spinAngle by SetState case 4 and read nowhere in
    s32 exitBoxSpan;                               // name and access width as src/objects/pipe2.cpp uses the field
    u32 launchDelay;                               // name and access width as src/objects/pipe2.cpp uses the field
    u32 launchStart;                               // name and access width as src/objects/pipe2.cpp uses the field
    s32 spinRemainder = 0;
};

struct Trajectory {
public:
    u16 count;                                     // number of points
    Vec3s pts[1];                                  // the points, 6 bytes each (Scn_GetPropTrajectory resources: FrozenRiver); indexed
};

class Piranhas : public ScnBody {
public:
    virtual void PostLoadInit();                   // Piranhas_Init (override)
    virtual void Update();                         // Piranhas_Update (override)
    virtual void Render(Camera *view);             // Piranhas_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Piranhas_HandleMessage (override)
    virtual void Reset();                          // Piranhas_Reset (override)
    void PickSwimTarget(Vec3s *out);
    void SetState(u8 newState);
    void UpdateSharedFx();
    u8 state;                                      // PiranhasState (1 bite, 2 attack wait, 3 swim, 4 leap)
    u8 attackOrder;                                // g_piranhaAttackCount after this piranha's increment; only 1 sends the kill
    u8 _pad066[0x6];
    s32 attackTimerMs;                             // 1000 ms between freezing Ralph and the bite
    u8 _pad070[0x4];
    Box *waterBox;                                 // water zone containing the spawn point; min.y = surface; the attack and target area
    ScnObject *inflatableSheep;                    // first InflatableSheep (class 0x84) in the level, or NULL; sent msg 0 arg 9 while inside waterBox
    u32 submerged;                                 // 1 = draw with the water tint (tintAmount 0x800 of g_waterColor); 0 while leaping out or attacking
    Trajectory path;                               // the inline trajectory the follower runs on: count (2 for swim, 4 for leap), then the points (start,
    Vec3s pathMorePts[3];                          // path.pts[1..3]: where the inline trajectory's points continue past its one declared point; written a
    TrajFollower follower;                         // 0x20-byte follower over path (speed 500, arrive radius 50, 3D); +0xa0 pointIndex drives the leap ani
};

class PolyBatcher {
public:
#ifdef SDW_MEMBERS_PolyBatcher
    SDW_MEMBERS_PolyBatcher
#endif
    virtual ~PolyBatcher();                           // PolyBatcher_ScalarDeletingDtor
    u32 GetTypeStateFlags(s32 polyType);
    void SetTypeStateFlags(s32 polyType, u32 flags);
    void ClearTypeStateFlags(s32 polyType, u32 flags);
    u32 GetDefaultStateFlags();
    void SetDefaultStateFlags(u32 flags);                        /* PolyBatcher_SetDefaultStateFlags */
    void ClearDefaultStateFlags(u32 flags);
    u32 GetClearColor();
    void SetClearColor(u32 argb);
    void SetComputeSortZ(u8 enable);
    s32 CreateTexture(const char *fileName, u32 index, u32 width, u32 height, u32 format);
    s32 LoadTexturePages(const char *path);
    long Render_RestoreLostSurfaces();
    bool IsImmediateTexture(u32 textureIndex);
    void Render_BeginFrame();
    void Render_EndFrame(u8 flush);
    void Render_Present(u32 maxFps);
    void Flush();
    D3DApp *renderer;                              // the renderer/device wrapper whose IDirect3DDevice7* lives at +0x28; the batcher reaches D3D only thr
    u32 defaultStateFlags;                         // RenderStateFlags applied by Render_BeginFrame at the start of every frame; initialised to 0x120 = RS
    u32 clearColor;                                // colour passed to IDirect3DDevice7::Clear each frame; 0 at construction, set per level by Load_DAVnWA
    u32 batchCapacity;                             // triangles per batch before flush
    Texture **textures;                            // malloc'd array of textureCount+4 Texture object pointers, one per VDX7 page
    u32 texturePageCount;                          // total texture-page count. PolyBatcher_LoadTexturePages writes it as header_word & 0xffff an
    u32 immediateTexCount;                         // count of the IMMEDIATE texture prefix, a subset of texturePageCount at +0x18 (not a total).
    u32 lastTextureIndex;                          // index of the texture currently bound to stage 0, used to skip redundant SetTexture calls
    u8 textureDirty;                               // forces the next textured draw to rebind even if the index matches; set by Render_BeginFrame, by the
    u32 *texStateFlags;                            // per-page RenderStateFlags: 0xc020 for immediate pages (TEXTURED|FILTER_LINEAR|DITHER), 0xc002 for al
    void *flatBatchVerts;                          // untextured triangle batch, capacity * 0x48 bytes (3 vertices of 0x18, FVF 0xc4).
    u32 flatBatchCount;                            // triangles in the untextured batch
    u32 stateFlags;                                // the RenderStateFlags word applied when the untextured batch is flushed
    void **texBatchVerts;                          // per-texture batch buffers (0x60 per triangle, FVF 0x1c4); counts at +0x3C[], render flags at +0x28[]
    u32 *texBatchCounts;                           // per-texture triangle counts, one per immediate page, parallel to texBatchVerts (+0x38) and texStateF
    u8 computeSortZ;                               // 1 makes PolyBatcher_SubmitPoly compute a deferred poly's sortZ as the sum of its three vertex z (rea
    RenderPoly *sortedPolys;                       // pool of 6000 RenderPoly of 0x20 bytes; the allocation is 0x2ee04 bytes = a leading count word (6000)
    RenderPoly **sortedList;                       // malloc(24000) = 6000 pointers into the RenderPoly pool; this is the array qsort reorders
    u32 sortedCount;                               // number of RenderPoly already pushed into sortedList this frame; the index RenderPoly_Assign writes a
    u32 blendStateFlags;                           // RenderStateFlags for RenderPoly type 2, initialised to 2 = RSF_BLEND_ALPHA (SRCBLEND=INVSRCALPHA, DE
    u32 addStateFlags;                             // RenderStateFlags for RenderPoly type 3, initialised to 4 = RSF_BLEND_ADD (SRCBLEND=INVSRCALPHA, DEST
    void *lineBatchVerts;                          // line batch: a fixed 96000-byte buffer = 4000 vertices of 0x18 bytes = 2000 lines, drawn as D3DPT_LIN
    u32 lineBatchCount;                            // lines queued in the line batch; zeroed by the flush and by the discard path of Render_EndFrame
    u32 lineStateFlags;                            // RenderStateFlags for the line batch, 0x121 = RSF_ANTIALIAS | RSF_DITHER | RSF_COLORVERTEX
};

class PorkyLevel01 : public ScnMobile {
public:
    virtual void PostLoadInit();                   // PorkyLevel01_PostLoadInit (override)
    virtual void Update();                         // PorkyLevel01_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // PorkyLevel01_HandleMessage (override)
    virtual void Reset();                          // PorkyLevel01_Reset (override)
    void ShowSalad(s8 index);
    void HideSalads();
    void SetState(u8 newState, char *text, u32 voice);
    void FaceWolf();
    s32 burnt;                                     // MSG_KILL (0) with argument 0 (an explosion): Porky turns black (tint 0x1000, INST_F_TINT) and his ne
    s32 burntLineSaid;                             // the burnt line (class string 9, voice 0x41) has been said; it is said once
    Box *activationBox;                            // PROPERTY ACTIVATIONBOX (record +0x00; an id list that must hold exactly one box, else state 5): Ralp
    Box *cinBox;                                   // PROPERTY CINBOX (+0x10; 0 unless exactly one box): Cine_Start's box
    Box *sheepBox;                                 // PROPERTY SHEEPBOX (+0x28; exactly one box, else state 5): Cine_Start's sheep box
    u8 _pad090[0x4];
    u16 cinWithSheep;                              // PROPERTY CINWITHSHEEP (+0x1c) as a word: the cinematic played when Ralph arrives with a s
    u16 cinWithoutSheep;                           // PROPERTY CINWITHOUTSHEEP (+0x14) as a word: the cinematic played when he arrives without
    u8 state;                                      // PorkyLevel01 state (SetState): 0 waiting for Ralph in ACTIVATIONBOX, 1 lesson done, 2 with-
    s32 cinWithSheepPlayed;                        // the with-sheep cinematic has started; afterwards neither cinematic plays again
    s32 cinWithoutSheepPlayed;                     // the without-sheep cinematic has started; it plays once
    char *cinWithSheepText;                        // Text_GetClassString(CINWITHSHEEPTEXT +0x20): the with-sheep cinematic's text
    char *cinWithoutSheepText;                     // Text_GetClassString(CINWITHOUTSHEEPTEXT +0x18)
    char *withSheepText2;                          // Text_GetClassString(WITHSHEEPTEXT2 +0x30); not read in this file
    char *withoutSheepText2;                       // Text_GetClassString(WITHOUTSHEEPTEXT2 +0x2c); not read in this file
    s32 saladDelayMs;                              // PROPERTY DISPLAYSALADMILLIS (+0x24) at load: the delay before the first lettuce appears during the w
    ScnObject *sheep;                              // the sheep (class 11 within 200 of Ralph) that started the with-sheep cinematic: sent msg 0x12 then,
    s32 menuJustOpened : 1;                        // (bits) bit 0: set by the talk message, cleared at the end of every Update; blocks a second talk and the con
    s32 wolfFrozen : 1;                            // (bits) bit 1: Ralph was frozen with msg 0xE when the talk started; unfrozen with msg 0xF when the line or t
    s32 unusedBits : 30;
    MenuPage choicePages[5];                       // the question box's node array (root + 3 choices + 1), built by Menu_BuildList
    Menu choiceMenu;                               // Menu record written by Menu_BuildList; its count (+0xec) sizes the box
    u32 curVoice;                                  // voice of the line being said (SetState argument 3; Dialogue_Say)
    Salad *salads[20];                             // the level's lettuces (Scenaric_FindByClass(3.., 20)), taken out of the world at load; a s
    Vec3s saladPos[20];                            // each lettuce's position when it was taken out; ShowSalad puts it back there
    u32 saladCount;                                // number of lettuces found (compared unsigned with jae; Update reads its low byte as a signed
    s32 saladShowActive;                           // 1 from the start of the with-sheep cinematic until every lettuce has been shown one by one
    s8 saladShowIndex;                             // index of the last lettuce shown; -1 at the start
    char *curText;                                 // line being said in state 4 (SetState argument 2)
    MenuBox questionBox;                           // text = ASKTEXT, pages = &choicePages, list = &choiceMenu, rect sized to the longest line and centred
    char *choiceTexts[3];                          // Text_GetClassString(CHOICESTEXTS + i), printed by the three choice handlers
};

class PrayingGhost : public ScnBody {
public:
    virtual void PostLoadInit();                   // PrayingGhost_PostLoadInit (override)
    virtual void Update();                         // PrayingGhost_Update (override)
    virtual void Render(Camera *view);             // PrayingGhost_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // PrayingGhost_HandleMessage (override)
    virtual void Reset();                          // PrayingGhost_Reset (override)
    void SetState(u8 newState);
    s32 MoveToward(Vec3s target, s32 speed, s32 turn);
    s16 TurnToHeading(s16 want);
    void FaceTowards(Vec3s target);
    void DrawAlertIcon();
    ScnObject *sheep;                              // designer property SHEEP (Scn_GetPropObject prop 0x1c); the sheep this ghost prays to and c
    ScnObject *jail;                               // first CLASSID_JAIL 126 object (Scenaric_FindByClass 0x7e)
    ScnObject *inflatableSheep;                    // first CLASSID_INFLATABLESHEEP 132 object, cleared when the search does not return 1
    ScnObject *ghostCostume;                       // first CLASSID_GHOSTCOSTUME 125 object, cleared when the search does not return 1
    ScnObject *checkpoint;                         // designer property CHECKPOINT (prop 0x0c); gets message 0x1080 from state 14
    u8 _pad078[0x4];
    ScnObject *camRestrict1;                       // designer property CAMERARESTRICTION1 (prop 0x00) - gets message 0xd80 with 0/1
    ScnObject *camRestrict2;                       // designer property CAMERARESTRICTION2 (prop 0x04)
    ScnObject *camRestrict3;                       // designer property CAMERARESTRICTION3 (prop 0x08)
    ScnObject *fallingGate;                        // designer property FALLINGGATE (prop 0x14) - gets message 0x1f from state 5
    ScnObject *fallingGateDetec;                   // designer property FALLINGGATEDETEC (prop 0x18) - gets message 0x20 when the trajectory ends
    ScnObject *noticed;                            // the object handed with message 0x5e; classId 0x65 makes it the chase target
    ScnObject *target;                             // who this ghost chases in states 12/13: `noticed` when its classId is 0x65, else g_pWolf ( -0x
    ZoneList monoHideBoxes;                        // designer property WOLFNOTVISIBLEMONO (prop 0x34) resolved through Scn_FindIdList
    ZoneList wolfHideBoxes;                        // designer property WOLFINVISIBLE1 (prop 0x30) resolved through Scn_FindIdList
    TrajFollower followSelf;                       // TrajFollower_Init(this..., speed 300, bias 0x800, arrive 50) over the TRAJECTORY property (0x45ae8
    TrajFollower followWolf;                       // second follower over the same trajectory - stepped with g_pWolf as the mover
    TrajFollower followSheep;                      // third follower over the same trajectory - stepped with g_prayingGhosts[2] as the mover
    Trajectory *trajectory;                        // designer property TRAJECTORY (Scn_GetPropTrajectory prop 0x2c) - u16 count then Vec3s points from +2
    s16 turnHeading;                               // heading turned a quarter of the way per call by PrayingGhost_TurnToHeading
    Box *startBox;                                 // designer property STARTBOX (Scn_GetPropBox prop 0x24)
    Box *sheepBox;                                 // designer property SHEEPBOX (Scn_GetPropBox prop 0x20)
    u16 trajCount;                                 // trajectory[0] - the number of trajectory points
    Vec3s trajFirst;                               // first trajectory point
    Vec3s trajLast;                                // last trajectory point: trajectory + 2 + (count-1)*6
    ScnBody halo;                                  // a second animated body built from exported resource 0xbf; its vtables are constructed by the factory
    u16 haloRecord[10];                            // the synthetic WAR record Scn_BuildRecordFromExport(0xbf...) writes for the halo body
    s32 haloValid;                                 // 1 when the halo record was built and the halo body initialised
    s32 haloVisible;                               // 1 while the halo body is drawn (set in state 13; tested by Render)
    Vec3s chaseDelta;                              // target minus mover; the working vector of every chase step
    Vec3s chaseVel;                                // chaseDelta scaled to speed 200 on x and z, 0 vertical
    Vec3s chaseStep;                               // Vec3s_ScaleByDt(chaseVel) - the per-frame translation
    Vec3s homePos;                                 // the object's position at PostLoadInit; the respawn point
    Vec3s postPos;                                 // the ghost's post on the trajectory: trajFirst offset by 0x1f4 / 0 / 0xf5 / 0x15e on x by index (0x45
    Vec3s sheepHome;                               // centre of SHEEPBOX in x/z, ground height there from the sheep's own collision box
    Vec3s wolfCapturePos;                          // g_pWolf->pos snapshotted when the capture starts (states 4 and 8)
    Vec3s sheepCapturePos;                         // sheep->pos snapshotted when state 6 starts
    u8 state;                                      // PrayingGhost_SetState's argument; 0..14, the Update switch
    u8 index;                                      // this ghost's slot: the argument of message 0x4505, also its index into g_prayingGhosts and g_ghostAt
    s32 wolfLift;                                  // vertical offset added to wolfCapturePos.y while the Wolf is carried; steps by -10 until |wolfLift| >
    s32 sheepLift;                                 // same for the sheep in state 6; steps by -10 while 300 - |sheepCapturePos.y| > |sheepLift|
    u32 prayDurMs;                                 // Anim_GetDurationMs(anim 4) - the length of the praying animation
    u32 grabDurMs;                                 // Anim_GetDurationMs(anim 10) - the length of the grab animation
    s32 alertDelayMs;                              // designer property TIMEORANGE (prop 0x28) - how long the ghost may see the Wolf before alertLevel goe
    s32 timer;                                     // countdown in g_dtMs units: the praying cycle in state 10 and the grab in state 13
    s32 sheepGraceMs;                              // 2000 ms grace after the sheep enters SHEEPBOX before the ghosts are told (message 0x4501)
    s16 trajHeading;                               // heading written by TrajFollower_Step
    u8 _pad1fa[0x6];
    s32 registered;                                // 1 once message 0x4505 has placed this ghost in g_prayingGhosts
    s32 isLeader;                                  // set only on the ghost with index 1; that one drives the cinematic capture
    s32 praying;                                   // 1 in state 10 (the praying loop) and 0 in states 1 and 12
    s32 wolfHidden;                                // 1 when the Wolf's position is inside one of monoHideBoxes or wolfHideBoxes
    s32 sheepBoxOccupied;                          // 1 when ObjGrid_QueryBoxOverlap on SHEEPBOX returned something that is not a sheep
    s32 sheepDecoySeen;                            // 1 when an InflatableSheep is in SHEEPBOX
    s32 decoyAccepted;                             // 1 when the InflatableSheep accepted message 0x4601
    s32 graceArmed;                                // 1 while the sheep-gone grace period is counting
    s32 trajDone;                                  // 1 when the cinematic trajectory has been walked to its end
    s32 watching;                                  // 1 in state 10 (the ghost is at its post and watching) and 0 in state 1
    s32 sheepStolen;                               // 1 once the sheep has been seen leaving STARTBOX; cleared when the capture reaches the Wol
    u8 iconBufToggle;                              // alternates the two icon primitive buffers handed to ScnBody_RenderEx
    u8 _pad22d[0x3];
    u8 iconPrimBuf[2][400];                        // two 0x190-byte primitive blocks for the head-icon draw (same shape as Watch::hudPrimBuf)
    u32 alertLevel;                                // index into g_samZoneColors for the head icon: 0 white / 1 orange
    ZoneList alertBoxes;                           // id list 0x45 - while the Wolf stands in one of these boxes the ghost does not notice him
};

struct ProgressOptionBits {
public:
    u8 setting : 2;                                // (bits) Progress.optionFlags bits 0-1 -> g_optLanguageOrDifficulty
    u8 gate : 1;                                   // (bits) bit 2 ->
    u8 soundMode : 2;                              // (bits) bits 3-4 ->, the sound option (cycled % 3 by the options menu); Sound_Play plays only in mo
};

struct ProgressRuntimeFlagBits {
public:
    u8 fieldAcFlag : 1;                            // (bits) Progress.runtimeFlags (+0xb4) bit 0, a bitfield VIEW of that u8 (runtimeFlags stays u8; the union me
    u8 secondDemoNext : 1;                         // (bits) bit 1. Level_FinishScene sets it when attract demo scene -6 ends and clears it when -7 en
    u8 timeKeeperUnsaved : 1;                      // (bits) bit 2. Set only by TimeKeeper_HandleMessage msg 1 right after Progress_AwardTimeKeeper. R
    u8 cardOpen : 1;                               // (bits) bit 3. Set by MCard_Init after Reg_OpenProgressKey succeeded, cleared by MCard_Shutdown (
    u8 saveSlotValid : 1;                          // (bits) bit 4. Set when a card session ends in a known slot: MCS_FINISH_SAVED, MCS_FINISH_LOADED
    u8 autoSaveOn : 1;                             // (bits) bit 5, the 'automatic save' setting. Set only by MCS_FINISH_LOADED, toggled by the pause
    u8 unused : 2;                                 // (bits) bits 6-7: never read or written anywhere in the image
};

class Progress {
public:
#ifdef SDW_MEMBERS_Progress
    SDW_MEMBERS_Progress
#endif
    s32 FieldAcFlagClear();
    s8 CurrentLevel();
    s8 GetLevel();
    s8 GetLevelIndexA();
    u8 GetLanguage();
    void SetLevelDone(s8 level);
    void SetSceneExitTarget(s8 level);
    void SetSecondDemoNext(s32 on);
    void Reset();                                                /* Progress_Reset */
    void AwardTimeKeeper();
    u32 IsTimeKeeperDone(s8 level);
    void SetBonusPoints(u16 points);
    void LoadRecord(const void *src);
    void CopyRecord(void *dest);
    u8 IsLevelDone(s8 level);
    void Level_FinishScene(s32 success);
    void StartAttractDemo();
    s32 GotoScene(s8 scene);
    char scenePath[SDW_PATH_MAX];                           // path prefix of the scene to load, built by Progress_BuildScenePath; g_pProgress itself is the path a
    u32 field80;                                   // set to 1 by during start-up; just outside the saved record
    u8 timeKeeperBits[4];                          // SAVED. per-level (0..31) TimeKeeper reward bitset: set by Progress_AwardTimeKeeper, tested by Progre
    u8 levelDoneBits[4];                           // SAVED. per-level completion bitset: tested by Progress_IsLevelDone; set by SceneSheepPanel
    u32 bonusFlags;                                // SAVED. 32-bit collected/completed bit set. Consumers index it by a per-object bit number held at obj
    u16 bonusPoints;                               // SAVED. spendable balance: +1 per TimeKeeper reward (+2 for level 16), lowered by BonusManager purcha
    s8 sheepToCatch;                               // SAVED (record +0xe). MCard_SlotChooser prints it with string 0x30 '%d sheep to catch' in the save-sl
    s8 bonusCount;                                 // SAVED (record +0xf). Printed as '%d' followed by string 0x31 'bonus' in the save-slot info.
    s8 slotIcon;                                   // SAVED (record +0x10). Save-slot thumbnail index 0..18; 19 is the empty-slot icon. It is mapped throu
    u8 streamVolumeA;                              // SAVED. CfgGame[0], default 0xc0; volume/255 applied by when g_pStreamPlayer mode is 1 (musi
    u8 streamVolumeB;                              // SAVED. CfgGame[1], default 0xff; applied by for stream modes 3/6
    u8 sfxVolume;                                  // SAVED. SFX volume 0..255. and push byte [+0x97] into, which sto
    union {
        u8 optionFlags;                                // SAVED. packed options byte, unpacked into three globals: bits 0-1 -> (a four-choice setting
        ProgressOptionBits optionBits;             // optionFlags as bitfields (setting, gate, soundMode): the sites that compile bitfield shifts, e.g. So
    };
    ControlConfig controls;                        // SAVED. the control settings as the ControlConfig Input_ApplyControlConfig takes (&controls,
    u16 field_aa;                                  // SAVED (record +0x26). Progress_Reset stores 0 (word store); meaning not traced
    u16 field_ac;                                  // SAVED (record +0x28). Progress_Reset stores runtimeFlags bit 0 ? 0x10 : 0 (word store, c
    u8 viewDistanceSetting;                        // SAVED. CfgGame[6], default 200; view distance = 12000*t + 4000*(1-t) with t = (255-v)/255 (v=0 far 1
    u32 saveTimestamp;                             // NOT saved (one past the record end). Card_CommitBlock stamps g_rawTime here and is compared
    union {
        u8 runtimeFlags;                               // NOT saved. bit flags, read-modify-written all over the game - 52 refs, the most-referenced field on
        ProgressRuntimeFlagBits runtimeBits;       // runtimeFlags as bitfields: the sites that compile bitfield shifts
    };
    u8 saveSlot;                                   // NOT saved. the block index handed to Card_CommitBlock
    s8 currentLevel;                               // NOT saved. current level, as a SIGNED byte: Wolf_Init tests == -1, other code tests -6 and -7 (non-l
    s8 sceneExitTarget;                            // NOT saved. destination when the Scene hub (-3) finishes with success; set to -3 by every GotoScene
    s8 levelIndexA;                                // NOT saved. bounds-checked to 0..0x1f before use as an index; compared against levelIndexB. 13 refs
    s8 levelIndexB;                                // NOT saved. same 0..0x1f bounds check; branches on levelIndexB == levelIndexA. 9 refs
    u8 modeBits;                                   // NOT saved. low bits switched on (& 3) and tested (& 1); also used as % 3
    u8 language;                                   // Config language index: 0 English, 1 Spanish, 2 Italian, 3 Portuguese. MCard_GetString indexes the 4-
};

struct RCarpetFlagBits {
public:
    u8 collide : 1;                                // (bits) bit 0 (1-byte unsigned unit): collide with the world | RCarpetMobile +0xa8, read and written as r8
    u8 reversed : 1;                               // (bits) bit 1 (1-byte unsigned unit): travelling the waypoints in reverse
    u8 clampEnds : 1;                              // (bits) bit 2 (1-byte unsigned unit): clamp at the ends (one-way)
    u8 driven : 1;                                 // (bits) bit 3 (1-byte unsigned unit): driven this frame
    u8 snap : 1;                                   // (bits) bit 4 (1-byte unsigned unit): snapped to a waypoint
    u8 arrived : 1;                                // (bits) bit 5 (1-byte unsigned unit): scratch: within the arrival radius
};

class RCarpetMobile : public ScnLogic {
public:
    virtual void PostLoadInit();                   // RCarpetMobile_Init (override)
    virtual void Update();                         // RCarpetMobile_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // RCarpetMobile_HandleMessage (override)
    virtual void Reset();                          // RCarpetMobile_Reset (override)
    s32 GroundQuery(struct GroundQuery *query);
    s32 Move(s32 reverse, s32 clampEnds, s32 returnHome);
    s32 TrajStep(TrajFollower *f, Vec3s *outVelocity, s16 *outHeading, s32 reverse, s32 clampEnds, s32 returnHome);
    void UpdateRideBox();
    u16 senderClass;                               // classId of the last message sender (written for every message with a sender); Init sets 0x73. Tested
    ContactInfo contact;                           // Collide_ResolveMove contact output for the carpet's own sweep.
    TrajFollower follower;                         // Trajectory follower (arrive radius 30, use3d 1, speed 0). pointIndex +0x64 is moved both ways by Han
    Trajectory *traj;                              // TRAJECTORY property resource {u16 count; Vec3s pts[]}; the carpet starts on pts[0].
    CollBox rideBox;                               // Model box 0 + pos with the top raised by 30. Objects overlapping it are carried and Sheep in it are
    s32 speed;                                     // Signed drive speed from the last message (0x47 arg, Â±400 for 0x1F/0x20, 400 when returning), multipl
    s32 speedRatio;                                // SPEEDRATIO property, a percentage (50/70 in Lvl-13, 100 in Lvl-05).
    Vec3s homePos;                                 // Position after Init (trajectory point 0); the return target in mode 4 and the Reset position.
    Vec3s snapPoint;                               // The waypoint consumed in this TrajStep; Move snaps onto it that frame.
    RCarpetFlagBits carpetFlags;                   // RCarpetMobileFlags: 1 collide, 2 reversed, 4 clamp at ends (one-way), 8 driven this frame, 0x10 snap
    u16 motorSound;                                // Channel of motor sound 0x5E.
    s32 mode;                                      // RCarpetMobileMode: 1 loop (Create default), 2 one-way, 4 one-way and return home when not driven. Se
};

class RabbitCostume : public ScnBody {
public:
    virtual void PostLoadInit();                   // RabbitCostume_Init (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // RabbitCostume_HandleMessage (override)
    Vec3s homePos;                                 // Same as SheepCostume.homePos (identical code).
};

struct RaftFlagBits {
public:
    u8 fan : 1;                                    // (bits) bit 0 (1-byte unsigned unit): blown by a Fan this frame | Raft +0x13e, accessed as r8
    u8 reset : 1;                                  // (bits) bit 1 (1-byte unsigned unit): the RESET property
    u8 afloat : 1;                                 // (bits) bit 2 (1-byte unsigned unit): afloat
    u8 hit : 1;                                    // (bits) bit 3 (1-byte unsigned unit): hit by a CannonBall/WaterMine this frame
};

class Raft : public ScnBody {
public:
    virtual void PostLoadInit();                   // Raft_Init (override)
    virtual void Update();                         // Raft_Update (override)
    virtual void Render(Camera *view);             // Raft_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Raft_HandleMessage (override)
    virtual void Reset();                          // Raft_Reset (override)
    InlineEmitter8 wakeEmitter;                    // Embedded ParticleEmitter with inline storage, set up in Raft_Create. The header is 0x24 bytes at +0x
    s32 riderCount;                                // Number of non-excluded objects on the raft last frame; a change restarts the bob animation 2. Zeroed
    s32 fanSpeed;                                  // Fan propulsion speed in u/s, approaching 350 (accel/decel 200) while blown with the Wolf aboard. It
    s32 flowSpeed;                                 // Water-current speed in u/s, approaching the zone speed (100/200/400) at accel 50, decel 100, max 200
    Vec3s startPos;                                // Placed position; Reset returns the raft here only when RESET is set.
    s16 fanHeading;                                // Arg of the last msg 0x14 (Fan blow), which is the fan holder's facing. Also picks the wake spawn cor
    s16 flowHeading;                               // Heading of the water current from Zone_GetFlowHeading. It snaps while flowSpeed == 0, otherwise turn
    RaftFlagBits raftFlags;                        // RaftFlags: 1 fan-blown this frame, 2 RESET property, 4 afloat, 8 hit by CannonBall/WaterMine this fr
};

class RemoteControl : public ScnMobile {
public:
    virtual void PostLoadInit();                   // RemoteControl_Init (override)
    virtual void Update();                         // RemoteControl_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // RemoteControl_HandleMessage (override)
    virtual void Reset();                          // RemoteControl_Reset (override)
    Vec3s homePos;                                 // ground-snapped position at Init, updated by msg 9 arg 1; Reset returns here when in world
    u8 _pad082[0xa];
    ScnObject *target;                             // RemoteControlProps.TARGET via Scn_GetPropObject(record, 4): the robot (or other object) receiving ms
    u32 switchOnOff;                               // raw RemoteControlProps.SWITCHONOFF (record+0x14). Nonzero â†’ msg 0x17 answers 0x10 (HELD_REMOTE), zer
};

class Resizer : public ScnBody {
public:
    virtual void PostLoadInit();                   // Resizer_PostLoadInit (override)
    virtual void Update();                         // Resizer_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Resizer_HandleMessage (override)
    virtual void Reset();                          // Resizer_Reset (override)
    void PlaceAtBoxCentre(ScnObject *object, Box *box);
    void SetState(u8 state);
    u32 flags;                                     // bit 0x1 = someone else already froze the Wolf, do not freeze (Update tests !(flags&1) be
    u8 state;
    AltModel model;                                // AltModel: outMain of ScnObject_InitWithAltModels
    AltModel altMaximize;                          // alt-model entry for id-list 0xbe
    AltModel altExit;                              // outAlts[1], id-list 0xb7
    s32 startMs;                                   // g_gameTimeMs when the target is frozen in state 1 (Update); the resize waits 500 ms from it
    u32 elapsedMs;                                 // g_gameTimeMs - startMs (Update state 1)
    u16 camFocal;                                  // the scripted camera's focal, 0x180 (PostLoadInit)
    Vec3s camRot;                                  // the scripted camera's rotation: (0x100, -rot.y & 0xfff, 0) (PostLoadInit)
    Vec3s camEye;                                  // the scripted camera's eye: pos moved 300 back along the heading and 250 up (PostLoadInit)
    CollBox *modelBox;                             // model box 0, made non-solid while idle and solid while capturing (SetState)
    Box *activationBox;                            // PROPERTY_RESIZER_ACTIVATIONBOX (prop 8): the zone Update searches for a target; not read with RESIZE
    s16 height;                                    // modelBox->min.y - 5: the y at which a captured or ejected object is placed
    u16 targetClass;                               // the target's ScenaricClassId (CLASSID_WOLF or CLASSID_INSTANTMARTIAN)
    ScnObject *target;                             // the object being resized: found in activationBox, or handed over by MSG_RESIZER_RECEIVE
    ScnObject *exitObject;                         // PROPERTY_RESIZER_EXIT (prop 12): the exit Resizer that receives the resized target (MSG_RESIZER_RECE
};

struct Vec3i {
public:
    s32 x;
    s32 y;                                         // vertical points down
    s32 z;
};

struct ResolveScratch {
public:
    Vec3i projected;                               // Collide_ResolveMove: the remaining displacement << 10 while it is projected off the contact planes (
    Vec3i normalSum;                               // sum of the normals projected off in one pass; 3/8 of their mean is added back (dword zeroing at 0x50
    Vec3s step;                                    // the part of the displacement travelled up to the first contact (word store); reused as b
    u8 contactClass[16];                           // per contact of the last sweep: 1 floor, 2 wall, 4 floor-plane contact the box corner is below (byte
};

class ScnControllable : public ScnMobile {
public:
    virtual s16 GetStickHeading(s16 fallback);     // ScnControllable_GetStickHeading
    s32 IsStickTowardSide(ScnObject *);
    void NextIdleAnim(const IdleAnimEntry *, const IdleAnimEntry *, u32);
    s32 AddMoveModifier(ScnObject *);
    void RemoveMoveModifier(ScnObject *);
    s32 ApplyMoveModifiers(Vec3s *, Vec3s *, s32, s32, s32);
    void ReadPad(Pad *, s32, s32);
    s32 ScanInteractables(ActionHit *primary, ActionHit *secondary, s16 yRange, s16 boxDist, s16 pointRadius, s16 maxAngle, s32 skipPrimary);
    s32 Mobile_Steer(Vec3s *outVel, Vec3s *rot, const MoveRecord *rec, s32 suppressFacing);
    void SteerRun(Vec3s *outVel, Vec3s *rot, const MoveRecord *rec, s32 runFlag);
    void SteerToHeading(Vec3s *outVel, Vec3s *rot, const MoveRecord *rec, s16 heading);
    s32 Wolf_SurfaceVelocity(Vec3s *vel, s32 doubleSlopeRate, const s16 *surfaceTuning);
    u32 Wolf_MoveResolve(Vec3s *delta, ContactInfo *info, u16 normalCutoff, u16 mask, s32 horizLimit, s32 vertLimit, s16 stepAllowance);
    void StopMotion();
    void Hud_ResetActionPrompt();
    char actionPromptText[64];                     // cached HUD action prompt string
    u32 actionPromptTimeMs;                        // g_gameTimeMs when the prompt text was last rebuilt (500 ms refresh)
    s32 airTime;                                   // ticks since ground contact, cap 0x1E000
    s32 slopeTime;                                 // ticks on steep slope/slide zone
    s32 stickX;                                    // filtered stick X
    s32 stickY;                                    // filtered stick Y
    s32 stickMag;                                  // stick magnitude (0 = neutral)
    u32 padBits;                                   // 0x1 jump, 0x8 run held, 0x10 sneak, 0x20 jump edge, 0x40 action, 0x80, 0x2 (swim)
    Vec3s velocity;                                // units/s (x,+0xDA y,+0xDC z)
    s16 moveDir;                                   // heading of motion, 12-bit
    Vec3s bounceNormal;                            // last wall/floor contact normal used for rocket bounce
    s16 wantedDir;                                 // desired heading
    s32 facingAngVel;                              // angular velocity state for facing (+0x16)
    s32 moveDirAngVel;                             // angular velocity state for moveDir
    s16 speed;                                     // horizontal speed u/s
    s16 bounceSpeed;                               // extra speed along bounceNormal, decays with rec accel; zeroed by all ground steps
    Vec3s groundNormal;                            // 4.12 normal, flat = (0,-4096,0)
    ScnObject *riders[8];                          // objects attached to/riding the controllable; receive msg 0x6A
    u8 riderCount;                                 // count for riders[]; also gates platform-motion query
    u8 idleLoopCount;                              // loops left before next idle variant (RNG)
};

class Robot : public ScnControllable {
public:
#ifdef SDW_MEMBERS_Robot
    SDW_MEMBERS_Robot
#endif
    virtual void PostLoadInit();                   // Robot_Init (override)
    virtual void Update();                         // Robot_Update (override)
    virtual void Render(Camera *view);             // Robot_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Robot_HandleMessage (override)
    virtual void Reset();                          // Robot_Reset (override)
    virtual s16 GetStickHeading(s16 fallback);     // Robot_GetStickHeading (override)
    s32 UpdateCamera(Pad *pad);
    s32 GetDropPoint(Vec3s *out);
    s32 IsFalling();
    s32 CanJump();
    void ScanContextActions();
    s32 IsContextActionAllowed(s32 onGround);
    s32 DoContextAction();
    s32 TryContextAction(s32 onGround);
    u32 Move(Vec3s *delta, Vec3s *vel, const Vec3s *newRot, u16 mask);
    u32 WalkStep();
    u32 DashStep();
    u32 TurnToHeadingStep(s16 heading);
    u32 FallStep(u16 mask);
    u32 JumpRiseStep();
    s32 EjectFlightStep();
    s32 GroundControlStep();
    s32 StandStep();
    s32 FaceTargetStep();
    s32 IsControllable();
    s32 TakeControl();
    void ReleaseControl();
    void EnterIdle();
    void NextIdleAnim();
    void EnterState(u8 state);
    void SetState(u8 state);
    void SetStateAnim(u8 state, u16 animId, s32 loop);
    void SetStateKeepAnim(u8 state);
    void ClearState();
    TrailEmitter smokeEmitter;                     // Embedded dash-smoke ParticleEmitter with inline storage, the same 0x164-byte layout as Wolf trailA.
    ScnObject *heldObject;                         // Object in the robot's claw. Set when the pickup target accepts msg 4 (joint 0x13) at the end of stat
    ScnObject *suspender;                          // Third party that froze the robot (msg 0xE from a non-Wolf sender while CONTROLLED): control is suspe
    ScnObject *remote;                             // RemoteControl that activated the robot (sender of msg 0x3F arg 1). Gets msg 0 arg 8 at the end of th
    ScnObject *trap;                               // WolfTrap the robot sprang (end of state 0x16, flag 8). Sam's msg 0 sends it 0x2C80 (reset); msg 0x33
    CamSetup *ejectCam;                            // RobotProps.EJECTCAM via Scn_GetPropCamera(record, 0): camera setup {u16 focal; s16 rot[3]; Vec3s pos
    Trajectory *ejectTraj;                         // RobotProps.EJECTTRAJ via Scn_GetPropTrajectory(record, 4): {u16 count; Vec3s pts[]}. pts[0].y (+4) i
    ActionHit ctxAction;                           // ActionHit {action, target}: action = WolfContextAction from ScnControllable_ScanInteractables (msg 2
    s32 heldActionType;                            // WolfHeldAction reply of heldObject to msg 0x17 (2 = throwable, which allows put-down)
    s32 ctxPromptExtra;                            // 4th word of the context block. The scan sets it to 1 every frame (the Wolf leaves its copy at 0). Wo
    u16 itemPromptId;                              // Item prompt block passed to heldObject msg 6. Preset to 0x65 (the Robot class id) as a 'none' sentin
    u16 itemPromptArg;                             // second half of the msg 6 block (Salad writes a distance here for the Wolf); zeroed by ClearState
    s32 stateTime;                                 // ticks since state entry (+= g_dt, cap 0x1E000); zeroed by Robot_EnterState
    s32 smokeFlickerTimer;                         // ticks until the smoke phase bit 0x80 toggles; reloaded with Rand_Range(0x100,0x400), halved when the
    Vec3s spawnPos;                                // ground-snapped position at Init; Reset returns here
    s16 spawnFacing;                               // rot.y at Init; restored by Reset
    Vec3s savedCamRot;                             // robot camera {pitch, yaw, roll} saved every frame in camera mode 6 and restored into g_camReqRot whe
    s16 savedCamDist;                              // saved g_camDist for mode 6; ClearState default g_camModeParams[6].dist
    Vec3s dropOffset;                              // offset from pos for the put-down point; Init sets (0,-2,0)
    u16 stateSoundHandle;                          // looping state sound (hum 0x119, motor 0x97, dash 0x98); stopped by Robot_EnterState
    Vec3s ejectStartPos;                           // position when Sam's kick arrived (msg 0); start of the eject parabola
    s16 ejectFacing;                               // HeadingTo(kicker) stored by msg 0; kicked state 0x11 turns toward it
    u8 state;                                      // RobotState, the index into g_robotStateTable
    u32 robotFlags;                                // RobotFlags (see enum)
};

struct RobotStateDesc {
public:
    u16 animId;
    u16 flags;
};

struct RockFlagBits {
public:
    u8 pushed : 1;                                 // (bits) bit 0 (1-byte unsigned unit) | Rock +0x185: seven unsigned one-bit fields, accessed through r8
    u8 moving : 1;                                 // (bits) bit 1 (1-byte unsigned unit)
    u8 allowZ : 1;                                 // (bits) bit 2 (1-byte unsigned unit)
    u8 allowX : 1;                                 // (bits) bit 3 (1-byte unsigned unit)
    u8 reset : 1;                                  // (bits) bit 4 (1-byte unsigned unit)
    u8 fallsAtInit : 1;                            // (bits) bit 5 (1-byte unsigned unit)
    u8 debrisValid : 1;                            // (bits) bit 6 (1-byte unsigned unit)
};

class ScnLogicShadowed : public ScnLogic {
public:
    virtual void Render(Camera *view);             // ScnLogicShadowed_Render (override)
    virtual void RenderScaled(Camera *view, Vec3s *scale); // ScnLogicShadowed_RenderScaled (override)
    virtual void SetPosition(Vec3s *pos);          // ScnLogicShadowed_SetPosition (override)
    virtual ScnObject* Init(void *record);         // ScnLogicShadowed_Init (override)
    Shadow shadow;                                 // 0x18-byte Shadow for the ScnLogic+shadow base (flags at +0x54)
};

class Rock : public ScnLogicShadowed {
public:
#ifdef SDW_MEMBERS_Rock
    SDW_MEMBERS_Rock
#endif
    virtual void PostLoadInit();                   // Rock_Init (override)
    virtual void Update();                         // Rock_Update (override)
    virtual void Render(Camera *view);             // Rock_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Rock_HandleMessage (override)
    virtual void Reset();                          // Rock_Reset (override)
    s32 CrushObjectsBelow(ScnObject *exclude);
    u16 Move(Vec3s *delta, u16 flags, s32 pushing);
    void ApplyGravity(Vec3s *delta);
    void PlayRollSound();
    void ReturnHome();
    void SetState(u8 newState);
    void StartFall();
    void StartLvl03Cinematic();
    void StopRollSound();
    u32 lvl03Enabled;                              // 1 when LVL03WITHBOX or LVL03WITHOUTBOX is nonzero; enables the LVL03BOX cinematic trigger
    u32 lvl03BoxDelivered;                         // set by msg 0x1B (sent by Mailbox_Deliver); picks the WITHBOX cinematic and is cleared when
    u16 lvl03CineWithBox;                          // PROPERTY_ROCK_LVL03WITHBOX: cinematic id passed to Cine_Start (Lvl-03: 1)
    u16 lvl03CineWithoutBox;                       // PROPERTY_ROCK_LVL03WITHOUTBOX: cinematic id (Lvl-03: 4)
    Box *lvl03Box;                                 // Scn_GetPropBox(LVL03BOX); XZ trigger that starts the Lvl-03 cinematic in states 2/3
    Box *lvl03CameraBox;                           // Scn_GetPropBox(LVL03IDCAMERABOX); in state 7 the cinematic camera is released once the Wolf leaves i
    ScnObject *pusher;                             // sender of the last msg 0xA; exempt from wall-crush in Rock_Move; cleared on settle/seesaw/Reset
    s32 fallTime;                                  // ticks of free fall (g_dt); gravity speed = clamp(fallTime*4000>>12,200,3000); reset on floor contact
    Mat34s rollMatrix;                             // rolling orientation (Matrix_RollByDisplacement), passed to Instance_DrawRigid instead of rot
    Box *collBox;                                  // model box 0, or box 1 when box 0 lacks flag 4 and there are >1 boxes; used for pushes, dome ground q
    LaunchArc launch;                              // copy of the msg 0xC launch record; t at +0xbc, camera +0xc0, obj +0xc4, camParam +0xc8, flags +0xcc
    CamShot camShot;                               // skippable camera shot started at launch (CamShot_Update every frame)
    u32 smashTime;                                 // g_gameTime when a Train smashed the rock; state 6 lasts 0x5000 ticks
    ScnRecordSynth debrisRecord;                   // synthetic record from Scn_BuildRecordFromExport(0xDE WAR_IDO_AROCHE03)
    ScnBody debrisBody;                            // embedded ScnBody (ctor stores ScnObject then ScnBody vtable at +0xfc); inst +0x100, pos +0x108, anim
    ZoneList pushBoxes;                            // zone list {boxes, count}: Scn_FindIdList(ALLOWPUSHBOXES): a pushed position must overlap one of thes
    u32 stateTime;                                 // g_gameTime at the last Rock_SetState
    Vec3s homePos;                                 // position after Init (ground-snapped unless FALLSATINIT); target of Rock_ReturnHome
    Vec3s launchStartPos;                          // pos at msg 0xC; msg 0xB snaps back to it when |x|,|z| are within 50 of it (compared by absolute valu
    Vec3s seesawRestPos;                           // scratch copy of pos taken by msg 0xB for the snap-back test
    s16 settleMs;                                  // ms countdown (g_dtMs, only while no movable-object contact) before a landed rock goes back to state
    u16 rollSound;                                 // channel handle of the rolling loop sfx 0x106
    s16 pushHeading;                               // 12-bit atan2 heading of the last push; state 3 rolls this way at 200 u/s
    u8 state;                                      // RockState
    RockFlagBits rockFlags;                        // RockFlags
};

class Rocket : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Rocket_Init (override)
    virtual void Update();                         // Rocket_Update (override)
    virtual void Render(Camera *view);             // Rocket_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Rocket_HandleMessage (override)
    virtual void Reset();                          // Rocket_Reset (override)
    void DrawGauges();
    void SetState(u8 state);
    Vec3s homePos;                                 // Init pos or pos at msg 9 arg 1 (the sender's pos); Reset and Update state 2 (crash respawn) return t
    u8 state;                                      // RocketState 0..7 (Rocket_SetState, table). Rocket_Render: 4 draws the respawn wobble, 7 dra
    u8 active : 1;                                 // (bits) bit 0 of the flag byte +0x83, a one-bit field: Bit 0 is ACTIVE: set by msg 0x15 (start using), clear
    s32 fuel;                                      // remaining fuel in 1/4096 s ticks; msg 0x900 subtracts its arg (g_dt, or g_dt*2 while boosting) and r
    s32 fuelMax;                                   // PROPERTY_ROCKET_FUEL << 12
    s32 hits;                                      // remaining hit points; msg 0x901 subtracts its arg, returns 0 when destroyed; the write is at 0x4e519
    s32 hitsMax;                                   // (PROPERTY_ROCKET_HITS << 13) / 100
    u32 stateTime;                                 // g_gameTime stamp: set on entering states 3 and 4 (and 0 in Init). State 3 waits 0x3000 (3 s) hidden;
    u32 fuelColor;                                 // PROPERTY_ROCKET_FUELCOLOR
    u32 hitsColor;                                 // PROPERTY_ROCKET_HITSCOLOR
    s16 engineSoundHandle;                         // looping sound handle; non-zero while the engine sound plays
    s32 enginePitch;                               // 4.12 pitch/speed factor 0x1000..0x2000, moves by g_dt/2 per frame toward boost / cruise (2.0 s full
};

class Rocks : public ScnBody {
public:
    virtual void PostLoadInit();                   // Rocks_Init (override)
    virtual void Update();                         // Rocks_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Rocks_HandleMessage (override)
    u8 state;                                      // RocksState (0 intact, 1 hit, 2 crumbling)
};

struct RollingCarpetFlagBits {
public:
    u8 camera : 1;                                 // (bits) bit 0 (1-byte unsigned unit): scripted camera active | RollingCarpet +0xb6, accessed as r8
    u8 registered : 1;                             // (bits) bit 1 (1-byte unsigned unit): rider registered (MSG_RIDER_ADD sent)
};

class RollingCarpet : public ScnBody {
public:
    virtual void PostLoadInit();                   // RollingCarpet_Init (override)
    virtual void Update();                         // RollingCarpet_Update (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // RollingCarpet_CustomCollide (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // RollingCarpet_HandleMessage (override)
    virtual void Reset();                          // RollingCarpet_Reset (override)
    ScnObject *FindRider();
    void FaceRider();
    void InitBeltScroll(s8 step);
    void ScrollBelt(s16 rows);
    void SetState(u8 newState);
    s32 riderDelta;                                // The rider's displacement this frame along the carpet axis (MSG_MODIFY_MOVE delta.x or delta.z); its
    s32 timerMs;                                   // Cooldown after release (0x400) or the fast-roll limit (0x800); decremented by g_dtMs in states 3 and
    s32 ratio;                                     // RATIO*4096/10 (RATIO=500 in Lvl-13). Written by Init, never read by the class code.
    CamSetup *camera;                              // CAMERA property resource {u16 a; s16 b,c,d; pos at +8}, used by Camera_StartScripted while a non-fro
    u8 _pad074[0x4];
    CollBox topBox;                                // Model box 0 + pos, flattened to the top face and extended 50 upward, inset 50 on x and 150 on z. Use
    CollBox *collBox;                              // Model box used for CustomCollide and the ground query: the last box whose flags lack 0x40000000 (axi
    ScnObject *rider;                              // The object riding the belt (FindRider); receives 0x18/0x19/0x44/0x4C.
    ScnObject *targets[5];                         // TARGET..TARGET5 objects (RCarpetMobiles); msg 0x68 MOVING at Init, msg 0x47 speed while rolling.
    Vec3s center;                                  // Belt centre: x and z are the topBox midpoint; y is never written. The rider is pulled here.
    u16 motorSound;                                // Channel of belt sound 0x8E (rate 0x1000, or 0x2000 in the fast state).
    Vec3s rollRot;                                 // Rotation copied to rot when the rider's facing flips: rot at Init, with y (+0xae) rewritten by FaceR
    s16 riderSpeed;                                // The rider's velocity along the axis (MSG_MODIFY_MOVE vel.x or vel.z in u/s), 0 when its delta is 0.
    u8 state;                                      // RollingCarpetState 0..5.
    u8 axis;                                       // 0 = the belt runs along x (facing about 0x400/0xC00), 1 = along z (facing about 0/0x800).
    RollingCarpetFlagBits rcFlags;                 // RollingCarpetFlags: 1 scripted camera active, 2 rider registered (MSG_RIDER_ADD sent).
};

class Rook : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Rook_Init (override)
    virtual void Update();                         // Rook_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Rook_HandleMessage (override)
    virtual void Reset();                          // Rook_Reset (override)
    Vec3s *AimCannonAt(Vec3s *outError, const Vec3s *target);
    void SetState(u8 state);
    u8 state;                                      // 0 riding the trajectory and watching, 1 aiming, 3 recoil/fire delay, 4 ball launched (no Update case
    s32 fireDelayMs;                               // Recoil countdown in ms: 0x15e (350) on entering state 3, drained by g_dtMs until the signed test at
    CollBox *detectBox;                            // PROPERTY_ROOK_BOXDETECT (0). Translated in place every frame so it travels with the Rook; only the x
    CollBox *bodyBox;                              // The Rook's own first solid model box (ScnObject_GetFirstSolidBox), used as the carry vol
    ScnObject *cannon;                             // PROPERTY_ROOK_CANNON (4): the CanonDummy barrel, kept 60 units above the Rook; its rot is what Rook_
    ScnObject *cannonBall;                         // PROPERTY_ROOK_CANNONBALL (8): receives 0x3382 MSG_CB_PLACE while patrolling, 0x3387 with 0 on recoil
    TrajFollower traj;                             // 0x20-byte follower for PROPERTY_ROOK_TRAJECTORY (20). Init hardcodes speed 200, headingBias 0x800, a
    s32 aimDeltaX;                                 // target.x - pos.x, truncated to 16 bits then sign-extended; recomputed by every aim step.
    s32 aimDeltaY;                                 // target.vertical - pos.vertical (down-positive); drives the pitch through -atan2(dy, horizontal dista
    s32 aimDeltaZ;                                 // target.z - pos.z; with aimDeltaX it gives the yaw through atan2(dx, dz).
    s32 aimVelRoll;                                // Angular-velocity slot for the unused roll axis: cleared on entering state 1 and never read, because
    s32 aimVelYaw;                                 // Math_ApproachAngle state for the cannon's facing (cannon+0x16); slew limit 0x1f40, accel/decel 0x3e8
    s32 aimVelPitch;                               // Math_ApproachAngle state for the cannon's pitch (cannon+0x18), same limits as the yaw.
    Vec3s lastTargetPos;                           // The Wolf's position as of the previous aiming frame. The Rook fires only while this still equals his
    Vec3s cannonRestRot;                           // The cannon's rotation captured at init and restored verbatim by Rook_Reset. Last field: 0xd2 + 6 = o
};

class Sail : public ScnBody {
public:
    virtual void PostLoadInit();                   // Sail_Init (override)
    virtual void Update();                         // Sail_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Sail_HandleMessage (override)
};

class Salad : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Salad_Init (override)
    virtual void Update();                         // Salad_Update (override)
    virtual void Render(Camera *view);             // Salad_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Salad_HandleMessage (override)
    virtual void Reset();                          // Salad_Reset (override)
    void Consume();
    void SetState(u8 state);
    u32 stateTime;                                 // g_gameTime at hide/respawn transitions (10 s wait, 1 s wobble). Collides with ScnMobile.csv's 0x7c/0
    s32 eatTime;                                   // g_dt accumulated while a sheep sends msg 1; >=0x3000 (3 s) -> eaten. Cumulative across sheep and vis
    s32 liftBlockMs;                               // ms since msg 0x79 (Wooden Lift); liftBlocked cleared after 1000
    Vec3s homePos;                                 // respawn/restart position; = pos at Init and at checkpoint msg 9 arg 1
    u8 state;                                      // SaladState
    u8 squashed : 1;                               // (bits) bit 0 of the flag byte +0x8f, a one-bit field: squashed model (ALAITU02) currently swapped in
    AltModel mainModel;                            // base model entry filled by ScnObject_InitWithAltModels in Salad_Create
    AltModel squashedModel;                        // alternate model WAR_IDO_ALAITU02 (id list 0x77) shown in state 3
    u32 liftBlocked;                               // set by msg 0x79 from the Wooden Lift; while set, sheep get 0 from msg 2 (not attracted)
};

class SaladRod : public CompositeRod {
public:
    virtual void PostLoadInit();                   // SaladRod_Init (override)
};

struct SamBeachBits {
public:
    u8 onSide2 : 1;                                // (bits) Sam.beachFlags bit 0: the beach variant's second ('2') resource set is current (see Sam.csv beachFla
};

struct SamCarryGoal {
public:
    ScnObject *object1;                            // first object to carry
    ScnObject *object2;                            // second object to carry
    Vec3s destination;                             // where to take them
};

struct SamChaseBits {
public:
    s32 chaseAfterDrop : 1;                        // (bits) Sam.chaseFlags bit 0: drop what you carry, then chase the Wolf (see Sam.csv chaseFlags)
};

struct SamChaseSoundBits {
public:
    u16 volume : 15;                               // (bits) fade-in volume of the run sound 0x137, 1..0xff
    u16 unused : 1;                                // (bits) bit 15: never read or set; every store keeps it only because it is the rest of the bitfield
};

struct SamFetchBits {
public:
    s32 picked1 : 1;                               // (bits) Preserved matched Sam bitfield access
    s32 picked2 : 1;                               // (bits) Preserved matched Sam bitfield access
};

struct SamFetchFlags {
public:
    union {
        u32 all;                                       // Sam.fetchGoal.flags as the plain word: set and cleared with | 1 / & ~1 ( family)
        SamFetchBits bits;                         // the same word as the SamFetchBits view: picked1 / picked2 read as signed 1-bit fields
    };
};

struct SamFetchGoal {
public:
    ScnObject *object1;                            // first object to fetch
    ScnObject *object2;                            // second object to fetch (0 = only one)
    SamFetchFlags flags;                           // bit 0 object1 picked up, bit 1 object2 picked up
};

struct SamFollowBits {
public:
    s32 midRoute : 1;                              // (bits) Sam.followFlags bit 0: after reaching a waypoint Sam sets it to waypointCount > 1, i.e. the new targ
    s32 dropping : 1;                              // (bits) bit 1: set while midRoute when the ground under the next step is more than 100 lower, or
    s32 dropToNode : 1;                            // (bits) bit 2: set while midRoute and not dropping when the current waypoint is more than 40 belo
};

struct SamPathHist {
public:
    u8 type;                                       // the entry kind, SAM_HIST_* (0 = empty; Sam_AddPathHistory stores its kind argument)
    u8 pad;                                        // padding
    s16 x;                                         // Sam's x when the entry was added
    s16 z;                                         // Sam's z when the entry was added
    s16 a;                                         // first argument of Sam_AddPathHistory (the search heading for kind 3)
    s16 b;                                         // second argument of Sam_AddPathHistory (the heading step for kind 3; the search reads it back negated
};

struct SamSheepBits {
public:
    s32 heardCall : 1;                             // (bits) Sam.sheepFlags bit 0: Sam heard the sheep-costume bleat (msg 0x69) inside his orange zone (see Sam.c
};

struct SamTracked {
public:
    ScnObject *object;                             // the tracked object (entry 0 = the Wolf, the rest sheep; filled by Sam_SnapshotSheepPositions 0x464cc
    s16 x;                                         // the object's x when last checked
    s16 z;                                         // the object's z when last checked
};

class Sam : public ScnMobile {
public:
#ifdef SDW_MEMBERS_Sam
    SDW_MEMBERS_Sam
#endif
    virtual void PostLoadInit();                   // Sam_Init (override)
    virtual void Update();                         // Sam_Update (override)
    virtual void Render(Camera *view);             // Sam_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Sam_HandleMessage (override)
    virtual void Reset();                          // Sam_Reset (override)
    Vec3s ClampPointToBoxXZ(CollBox *box, Vec3s *point);
    Vec3s NearestPointInBoxesXZ(CollBox **boxes, short count, Vec3s *point, short margin);
    int CollectMovedSheep(ScnObject **out);
    int FindPathHistory(unsigned char kind, short a);
    int FindPathHistoryExact(unsigned char kind, short a, short b);
    int IsDistBelowHistoryThreshold(int distance);
    int IsTargetNearFinalPathEdge();
    int IsWolfBehindHidingBox();
    int IsWolfOutsideHitZone(int ignoreTimer, int requireGoalChase);
    int KickRobot(int force);
    int NoticesWolfWhileBusy(int kind);
    int PointInBoxListXYZ(Vec3s *point, CollBox **boxes, unsigned short count);
    int PointInBoxListXZ(Vec3s *point, CollBox **boxes, unsigned short count);
    int TryCatchWolf();
    unsigned char CheckStepUpAhead(Vec3s *end, short firstHeight);
    unsigned short GetAnimId(unsigned short index);
    unsigned short ResolveMove(Vec3s *delta, SamContactInfo *info, unsigned short normalCutoff, unsigned short mask, Vec3s *startPos, CollBox *box);
    void AddCarried(ScnObject *object);
    void AddPathHistory(unsigned char kind, short a, short b);
    void ClearPathHistory();
    void DetectWolf(Vec3s *wolfPos, short halfAngle);
    void DetectWolfPatrol(Vec3s *wolfPos, short halfAngle);
    void DrawZoneIndicator();
    void DropAllCarried();
    void EnterCin01State();
    void EnterDefaultState();
    void EnterFollowNodeState(Vec3s *firstPos, NavNode *pathHead, int resetCooldown);
    void EnterFollowTrajectoryFastState();
    void EnterFollowTrajectoryState();
    void EnterInvestigateState();
    void EnterLookAfterState();
    void EnterLookAfterTimedState(short durationMs);
    void EnterReachGoalState();
    void EnterReachNodeState(NavNode *node, unsigned char neighborIndex, int fromSearch);
    void EnterSearchNearestNodeState(int dx, int dz);
    void FreezePickupTarget(ScnObject *object);
    void InitBeachVariant();
    void InitHeadRecord(unsigned short *record);
    void KnockOut(unsigned short fallAnim, unsigned short lyingAnim, unsigned short getUpAnim);
    void OnGoalReached(int reached, int interrupted);
    void PlayAnim(unsigned short index, int loop, int flag2, short speed);
    void PutDownCarried(unsigned short keepPosition);
    void RemoveCarried(ScnObject *object);
    void ResetStallAnchor();
    void SetGoalCarry(SamCarryGoal *goal);
    void SetGoalFetch(SamFetchGoal *goal);
    void SetGoalHome();
    void SetGoalWolf();
    void SetState(unsigned char state);
    void SetupJump(Vec3s *point);
    void SnapshotSheepPositions();
    void StartReactionCamera();
    void StartSearchTowardGoal(int *dxOut, int *dzOut, int *distanceOut);
    void Stub_46eb4c(short yaw, int unused);
    void SwapBeachPositions();
    void TurnYawToward(short targetYaw);
    void UnfreezePickupTarget(ScnObject *object);
    void UpdateGoalPos();
    void UpdateJump();
    void UpdateStallTracker();
    void UpdateZoneState();
    Vec3s FindFreeDropSpot(Vec3s point, CollBox *box);           /* Its field load overwrites the * scratch value before use; no uninitialized pointer is dereferenced. */
    s16 *animTable;                                // logical->model anim table: normal, carrying one, carrying two
    u8 state;                                      // SamState
    u16 waypointCount;                             // number of entries in waypoints[]
    Vec3s waypoints[128];                          // node path as 6-byte s16 triples (x, z-or-y order per copy: node[0], node[2], node[1]); extent up to
    NavSearch navSearch;                           // path search context: +0 open list head (node*), +4 active count, +8 s16 goalX, +0xA s16 goalZ
    Vec3s lastNodeTarget;                          // last node targeted by state 4 (x at +0x390, z at +0x394); init 0x8000
    s16 searchHeading;                             // state 3 heading, 45-degree steps
    s16 searchTurnCounter;                         // state 3 reroute counter mod 8
    s16 searchHeadingStep;                         // signed turn step used when blocked in state 3
    s32 renavCooldownMs;                           // 500 ms cooldown between nav re-plans in state 1
    Vec3s stallAnchor;                             // position at last stall reset
    s32 stallBestDist;                             // max Manhattan displacement from the anchor
    s32 stateTimerMs;                              // ms since Sam_SetState (also jump interpolation clock)
    s32 searchBlockedMs;                           // only ever zeroed (dead 5 s branch)
    u16 stallClockMs;                              // ms since stall anchor reset
    SamPathHist pathHistory[16];                   // 10-byte entries {u8 type; u8 pad; s16 x; s16 z; s16 a; s16 b}
    u16 pathHistoryIdx;                            // ring write index (newest)
    s16 speed;                                     // move speed units/s: 0x44c run, 200 walk
    SamEdgeNormal followEdgeNormal;                // dword taken from pathHead node +0x28+4*i (edge normal) for the first edge
    s16 spawnYaw;                                  // yaw at Init, restored on patrol/look-after entry
    u16 headRecord[10];                            // fake level record for the head sub-object (a WAR record, u16 words; Sam_InitHeadRecord fill
    ScnBody head;                                  // embedded ScnBody (HUD head icon); head+0x16 = +0x48a yaw, head fade +0x48e/+0x490, head flags +0x478
    u8 headRenderScratch[800];                     // two 400-byte primitive buffers after the embedded head
    s16 yawToWolf;                                 // absolute bearing to the Wolf (atan2 + 0x800)
    u8 frameToggle;                                // alternates 0/1 each update
    s16 headSweepPhase;                            // 12-bit phase of the look-after head sweep
    s16 headYaw;                                   // head yaw offset = sin(phase)*maxAngle/360 (applied to bone 9)
    s16 maxAngle;                                  // PROPERTY_SAM_MAXANGLE
    s16 headSpeed;                                 // PROPERTY_SAM_HEADSPEED
    s16 relAngleToWolf;                            // signed angle to Wolf relative to body+head yaw
    Vec3s homePos;                                 // home / look-after position
    u16 greenZoneCount;                            // count for GREENZONEBOX list
    u16 orangeZoneCount;                           // ORANGEZONEBOX count
    u16 authorizedZoneCount;                       // AUTHORIZEDZONE count
    u16 wolfCanBeHitZoneCount;                     // WOLFCANBEHITZONE count
    CollBox **greenZoneBoxes;                      // PROPERTY_SAM_GREENZONEBOX
    CollBox **orangeZoneBoxes;                     // PROPERTY_SAM_ORANGEZONEBOX
    CollBox **authorizedZoneBoxes;                 // PROPERTY_SAM_AUTHORIZEDZONE
    CollBox **wolfCanBeHitZoneBoxes;               // PROPERTY_SAM_WOLFCANBEHITZONE
    CollBox *catchableSheepBox;                    // PROPERTY_SAM_CATCHABLESHEEPBOX (single box)
    CollBox *dontTryToCatchSheepBox;               // PROPERTY_SAM_DNTTRYTOCATCHSHPBOX
    s32 alertLevel;                                // SamAlert: -1 none, 0 green, 1 orange, 2 red
    s32 wolfInGreenZone;                           // green-box flag
    Vec3s putSheepPos;                             // centre of PROPERTY_SAM_PUTSHEEPBOX (or spawn pos)
    s8 jumpPhase;                                  // 0 none, 1..7 jump-over-step sequence
    TrajFollower trajFollower;                     // 0x20-byte trajectory follower state
    Trajectory *trajectory;                        // PROPERTY_SAM_TRAJECTORY, a Trajectory {u16 count; Vec3s pts[]} (the first point at +2 is Sam's home
    u32 patrolFlags;                               // bit0 = sidestep MODE: while set, patrol drives with patrolSidestepDir instead of its own velocity (c
    union {
        u16 chaseSoundVolume;                          // low 15 bits: fade-in volume of Sam's run sound 0x137 (SGORUN.WAV), started at 1 and ramped to 0xff;
        SamChaseSoundBits chaseSoundBits;          // chaseSoundVolume as the SamChaseSoundBits view: the 15-bit volume is read and written as a bitfield
    };
    u16 chaseSoundHandle;                          // handle of sound 0x137 played while anim id 0x31 or 7
    Vec3s patrolSidestepDir;                       // perpendicular sidestep VELOCITY used while patrolFlags bit0: set to (vel.z, -vel.x) when patrol stal
    s16 lookAfterTimeoutMs;                        // state 7 countdown
    s32 distCatchPatrol;                           // PROPERTY_SAM_DISTCATCH_PATROL
    s32 forcedAlert;                               // set by msg 0x33 / 0x1D
    Vec3s investigatePos;                          // sender position from msg 0x481
    s16 investigateTimerMs;                        // -1 idle, 1000 on msg 0x481
    CollBox **hypnotizedZoneBoxes;                 // PROPERTY_SAM_HYPNOTIZEDZONE list (count at +0x884)
    u16 hypnotizedZoneCount;
    s16 investigateStopDelayMs;                    // state 10: 200 ms grace before switching to idle anim
    Vec3s jumpLandPos;                             // jump landing target
    Vec3s jumpStartPos;                            // position at jump take-off
    Vec3s jumpPrevPos;                             // previous interpolated jump position (for delta move)
    s32 jumpDurationMs;                            // duration of anim index 1
    s32 actionTimerMs;                             // knock-out (DEATHDURATIONMS) / robot watch+hit (WATCHROBOTTIMEMS) countdown
    u16 koGetUpAnim;
    u16 koLyingAnim;
    u16 koFallAnim;
    s32 lifeTimerMs;                               // free-running ms clock (run bob phase in Sam_Render)
    s8 frameCounter;                               // per-update counter mod 8 (noise poll on 0, give-up poll on 1, step check on 4)
    u32 mode;                                      // PROPERTY_SAM_MODE (SamModeFlags); modified at runtime (low 10 bits)
    s32 goalDist;                                  // Euclidean XZ distance to goalPos
    s32 goalDx;
    s32 goalDz;
    s32 restartStatePending;                       // set by state 0xB after launching CIN01; when DAT_0071c450 clears, re-runs Sam_SetState(current)
    u8 _pad8c8[0x4];
    ScnObject *frozenPickupObj;                    // pick-up target Sam has frozen while approaching: Sam_FreezePickupTarget sends msg 0xE (clas
    s16 freezeTimerMs;                             // update early-out while > 0
    s16 carriedCount;                              // number of objects in carried[] (Sam_AddCarried/RemoveCarried; zeroed in Sam_Init)
    ScnObject *carried[2];                         // objects Sam is holding after a pick-up (fetchObj1/2 appended in state 0xD). Sam_DropAllCarried copie
    CollBox **hidingBoxes;                         // PROPERTY_SAM_HIDINGBOXES (count u16 at +0x8e0)
    u16 hidingBoxCount;
    u32 cin01;                                     // PROPERTY_SAM_CIN01
    u32 cin01Flags;                                // PROPERTY_SAM_CIN01FLAGS
    CollBox *cin01Box;                             // PROPERTY_SAM_CIN01BOX
    CollBox *cin01SheepBox;                        // PROPERTY_SAM_CIN01SHEEPBOX
    void *cin01TextHandle;                         // result of (this, CIN01FLAGS, (u8)PROPERTY_SAM_CIN01TEXT)
    Vec3s goalPos;                                 // aim point (with lead)
    Vec3s targetPos;                               // un-led target position (catch distance uses this)
    s8 goalKind;                                   // SamGoalKind
    SamCarryGoal carryGoal;                        // SamCarryGoal: object1 = carry goal: first carried object; object2 = second carried object; destinati
    SamFetchGoal fetchGoal;                        // fetch goal: object1, object2 and the picked-up flags, read by sam.cpp as one struct
    union {
        u32 followFlags;                               // route bits (SamFollowBits): bit0 midRoute = the current target is an intermediate path node, set on
        SamFollowBits followBits;                  // followFlags as the SamFollowBits view: bit 0 is written as a bitfield, and the reads comp
    };
    SamTracked trackedObjs[25];                    // 8-byte entries {ScnObject *obj; s16 x; s16 z}; entry 0 = Wolf, rest sheep
    s16 trackedCount;                              // sheep count + 1
    u32 sheepMovedAccum;                           // raw g_dt accumulator in the sheep-guard logic (trigger 0x3000); zeroed by SetState
    s16 sheepGuardCooldownMs;                      // 20000 on look-after entry, min 2000 later
    CollBox *beachCsBoxCur;                        // MODE 0x8000: id-list 0xAF (sheep pen box used for drop-spot search)
    CollBox *beachCsBoxOther;                      // id-list 0xB0
    Vec3s altHomePos;                              // home used for goal kind 3 when MODE 0x8000
    Vec3s beachHomePosOther;                       // centre of id-list 0xB1 box
    union {
        u8 beachFlags;                                 // bit0 onSide2: the beach variant's second resource set (BEACHCSBOX2 176, BEACHSAMIPOS2 177, BEACHORAN
        SamBeachBits beachBits;                    // beachFlags as the SamBeachBits view: bit 0 read, set and flipped as a bitfield
    };
    union {
        u32 chaseFlags;                                // bit0 chaseAfterDrop: set in Sam_OnGoalReached when Sam, carrying a sheep or the first fetch object,
        SamChaseBits chaseBits;                    // chaseFlags as the SamChaseBits view: bit 0 read and cleared as a signed bitfield
    };
    CollBox **beachOrangeZoneOther;                // id-list 0xC3 (count u16 +0xa22)
    CollBox **beachGreenZoneOther;                 // id-list 0xC4 (count u16 +0xa20)
    u16 beachGreenZoneOtherCount;                  // count for beachGreenZoneOther (+0xa1c, id-list 0xC4 BEACHGREENZONE2); swapped with greenZoneCount +0
    u16 beachOrangeZoneOtherCount;                 // count for beachOrangeZoneOther (+0xa18, id-list 0xC3 BEACHORANGEZONE2); swapped with orangeZoneCount
    union {
        u32 sheepFlags;                                // bit0 heardCall: set by Sam_HandleMessage on msg 0x69 (the Wolf's sheep-costume bleat, MSG_SHEEP_CALL
        SamSheepBits sheepBits;                    // sheepFlags as the SamSheepBits view: bit 0 read and cleared as a signed bitfield
    };
    Vec3s firstSheepSpawnPos;                      // position of the first class-0xB object at Init
    ScnObject *catapultLoadedObj;                  // param of msg 0x482 while fetching the Wolf (compared with g_pWolf in pre-pass -> state 0x14)
    ScnObject *robot;                              // first class 0x65 (Robot) object or NULL
    Vec3s robotWatchPos;                           // robot position when state 0x16 began (moved > 10 -> hit at once)
    ScnObject *target;                             // chase target: g_pWolf or robot
};

struct SamContactInfo {
public:
    ScnObject *floorObj;                           // object of a floor-class contact (Sam_ResolveMove's private contact layout; zeroed at start)
    ScnObject *wallObj;                            // object of a wall-class contact
    ScnObject *carrierObj;                         // contact object whose class flags have 4 (SCN_CF_CARRIER)
    Vec3s floorNormal;                             // init (0,-0x1000,0); last floor normal
    s16 minContactY;                               // init box bottom; min of sweep outMaxY
    s16 boxTopY;                                   // translated box min.y (top), written once at start
    s16 minStaticY;                                // init box bottom; min of Collide_SweepBox_Sam outStaticY
    Vec3s wallNormalMean;                          // mean wall normal (written only if any wall contact); init 0
};

class Sam_Pirate : public ScnMobile {
public:
#ifdef SDW_EXTRA_Sam_Pirate
    SDW_EXTRA_Sam_Pirate
#endif
    virtual void PostLoadInit();                   // Sam_Pirate_Init (override)
    virtual void Update();                         // Sam_Pirate_Update (override)
    virtual void Render(Camera *view);             // Sam_Pirate_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Sam_Pirate_HandleMessage (override)
    virtual void Reset();                          // Sam_Pirate_Reset (override)
    Vec3s PickTrajPoint(Vec3s *points, u16 count, Vec3s exclude, s32 nearest);
    s16 CircleStep(Vec3s offset, Vec3s pivot, s16 angle, s16 step);
    s32 MoveToward(Vec3s target, s32 speed, s16 fallSpeed, Vec3s post, Vec3s bridgeEnd, s32 aboard);
    s32 Vec3sEqual(Vec3s a, Vec3s b);
    void FaceToward(Vec3s target);
    void SetState(u8 state);
    ScnObject *jailGate;                           // first FallingGate2 (class 0x8C) in the level; msg 0x72 asks whether it is open (returns CLOSED==0)
    Box *jailBox;                                  // PROPERTY JAIL_BOX (Lvl-15 id 617)
    Box *nearJailBox;                              // PROPERTY BOX_NEAR_JAIL (618)
    Box *bridgeBox;                                // PROPERTY BRIDGE_BOX (619): the boat bridge; on-boat test and talk trigger
    ZoneList forbiddenBoxes;                       // zone list {boxes, count}: PROPERTY BOX_FORBIDDEN id list (669, 2 boxes): a target coin inside one ab
    u8 state;                                      // Sam_PirateState (0..0x11), written by Sam_Pirate_SetState
    u8 pocketStep;                                 // state 0xD coin-pocketing step (0: lift coin 50 + scale 0x2aa, 1: scale 0x155)
    u8 idleAnimIdx;                                // current idle index 0..3 into g_samPirateBoatIdleAnims / JailIdleAnims (never repeats twice)
    u8 idleAnimPick;                               // scratch for the Rand_Bounded(4) re-roll
    s32 coinScale;                                 // render scale sent to the target coin with msg 0x4A82 (0x400, then 0x2aa, then 0x155)
    u8 _pad09c[0x4];
    s32 wolfDist;                                  // state 4: Vec3s_DistXZ(Wolf, Sam); < 401 draws
    s32 timerMs;                                   // g_dtMs countdown: idle length (2x or 4x anim), shoot length (2 x anim 1), 700/100 ms coin steps, 100
    Vec3s spawnPos;                                // pos after SnapToGround in Init; Reset and idle 0 teleport here; state 2 walks here
    Vec3s jailSpot;                                // pos on reaching nearJailPos (state 0xF end); jail idle 0 teleports here; state 3 walks back here
    u8 _pad0b4[0xc];
    Vec3s circleOffset;                            // pos - circlePivot at idle-0 start (= (Â±150,0,0)); rotated each frame by CircleStep
    Vec3s bridgeEndPos;                            // farthest TRAJ_BOAT point from spawn (Lvl-15: 2898,-24,6277); route point for leaving/boarding the bo
    Vec3s guardPost;                               // nearest TRAJ_BOAT point (Lvl-15: 1294,-200,6275); home for states 1/0xE/0x11 and the talk-trigger ce
    Vec3s trajExclude;                             // zeroed in Init and passed as the excluded point to Sam_Pirate_PickTrajPoint; no other use
    u8 _pad0d8[0x6];
    Vec3s nearJailPos;                             // centre of BOX_NEAR_JAIL (XZ) at ground height; jail walk target and jail talk-trigger centre
    u32 jailedOnce;                                // set on the first entry to state 0x10 (forces idle 3); gates the top-of-frame jail check; never reset
    u32 coinFound;                                 // state 9 scratch: a reachable free coin was found this frame
    u32 coinPickedBefore;                          // set by SetState(0xC); cleared by SetState(4). When set, a reached coin goes straight to state 0xD (s
    ScnObject *coins[15];                          // GoldenCoins (class 0x8D) found by Scenaric_FindByClass, limited to NBER_OF_COIN. Capacity up to +0x1
    ScnObject *targetCoin;                         // coin being hunted or pocketed
    Vec3s coinSearchOrigin;                        // reference for the 601-unit coin radius: Sam's pos (set when zero or unless keepSearchOrigin), or the
    u32 coinCount;                                 // number of coins actually found (written, not read in this class)
    u32 nbCoins;                                   // PROPERTY NBER_OF_COIN (Lvl-15: 10); loop bound for coins[]
    s32 nearestCoinDist;                           // best coin distance in state 9 (reset to 600)
    u32 coinHuntActive;                            // set in states 2/9; cleared in state 4 and the non-jail Reset. While 0, the top-of-frame coin scan ru
    u32 keepSearchOrigin;                          // set by state 0xD so the next state-9 search is centred on the pocketed coin
    Vec3s circlePivot;                             // spawnPos.x-150 (boat) or jailSpot.x+150 (jail)
    s16 circleAngle;                               // accumulated angle (starts -30, -30 per frame). Idle ends when |angle| > 0x2ffd (3 laps, boat) or > 0
    u32 onBoat;                                    // 1 if Sam's XZ is in bridgeBox, or |pos.y| > |guardPost.y| - 10 (on deck); drives routing, coin filte
    TrajFollower trajFollower;                     // initialised on TRAJ_BOAT (speed 250, radius 50) but never stepped (dead)
    Trajectory *trajBoat;                          // PROPERTY TRAJ_BOAT trajectory {u16 count; Vec3s pts[]}
    u16 trajPointCount;                            // copied from trajBoat[0] (Lvl-15: 2)
    Vec3s trajPoints[10];                          // copy of the TRAJ_BOAT points. Capacity 10 is inferred from the next field (+0x1bc); there is no boun
    EmitterDriftParams dustParams;                 // EmitterDriftParams: hSpeed = Emitter_UpdateDrift params p[0] = 100: horizontal drift units/s; vSpeed
    InlineEmitter16 dustEmitter;                   // running dust (spawns in states 9/0xE), drawn by Sam_Pirate_Render; inline storage 0x1d4..0x338: fact
    u8 _pad338[0x6];
    u16 shotSoundHandle;                           // handle of sound 0xF4 started in SetState(7); stopped when the state-8 anim ends
    char *textInBoat1;                             // string of PROPERTY TEXT_IN_BOAT1 (voice 0x38)
    char *textInBoat2;                             // TEXT_IN_BOAT2 (voice 0x39)
    char *textInJail;                              // TEXT_IN_JAIL (voice 0x3A)
    u32 talkedFlag;                                // 1 after a dialogue. Blocks re-talk until the Wolf is > 600 from guardPost (boat) or > 400 from nearJ
    u32 boatTextToggle;                            // alternates TEXT_IN_BOAT1/2 between boat talks
    u32 jailWarningGiven;                          // set while TEXT_IN_JAIL is spoken. Afterwards the Wolf entering jailBox makes jailed Sam draw (state
    s32 circleRemainder = 0;
};

class SceneSheepPanel : public ScnBody {
public:
    virtual void PostLoadInit();                   // SceneSheepPanel_PostLoadInit (override)
    virtual void Update();                         // SceneSheepPanel_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SceneSheepPanel_HandleMessage (override)
    virtual void Reset();                          // SceneSheepPanel_Reset (override)
    CamSetup *camShot;                             // PROPERTY_SCENESHEEPPANEL_ID_CAMERA (prop 0) via Scn_GetPropCamera: the camera setup record {u16 foca
    Box *sheepBox;                                 // PROPERTY_SCENESHEEPPANEL_ID_SHEEPBOX (prop 8); the panel fires when the sheep's position is inside i
    ScnObject *sheep;                              // The level's Sheep (class 11), found once in state 0 by Scenaric_FindByClass. Reset stores the sentin
    u16 timerMs;                                   // Milliseconds since the panel animation stepped down, accumulated from g_dtMs truncated to 16 bits; b
    u16 cineId;                                    // PROPERTY_SCENESHEEPPANEL_ID_CINE, read as the dword at record+4+0x14 and stored as a word; played wi
    s8 doorWorldReply;                             // Reply from Scenaric_SendToClass(DoorWorld 169, msg 0x5880): -1 = not asked yet, 0 = no door sequence
    u8 doorSequenceDone;                           // Set by SceneSheepPanel_HandleMessage on msg 0x4e00, which DoorWorld_Update sends when its d
    u8 state;                                      // 0 find the sheep and play the return cinematic, 1 wait for the sheep in ID_SHEEPBOX (with an extra b
};

class Scene_Wheel : public ScnBody {
public:
    virtual void PostLoadInit();                   // Scene_Wheel_PostLoadInit (override)
    virtual void Update();                         // Scene_Wheel_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Scene_Wheel_HandleMessage (override)
    virtual void Reset();                          // Scene_Wheel_Reset (override)
    s8 index;                                      // It seeds the target angle (index *
    u8 state;                                      // 0 freeze the Wolf and start the intro anim, 1 wait for it, 2 idle/done, 3 (re)arm, 4 interactive spi
    s8 snapping;                                   // Set to 1 whenever the player steps the wheel; while set the wheel integrates angVel toward targetAng
    s8 wolfFrozen;                                 // Non-zero once the Wolf has accepted SCN_MSG_UI_FREEZE (0x0E); on confirm the wheel sends 0x0F and cl
    s8 exitLevelOnConfirm;                         // Branch selector on confirm: non-zero sets g_levelExitFlags bit 1 and calls Fade_StartLevelExit(0x100
    u8 pad069;                                     // Never referenced; alignment between the s8 at 0x68 and the u16 at 0x6a.
    u16 unk6a;                                     // Zeroed by Scene_Wheel_Reset and never read or written anywhere else in the class (exactl
    u16 idleMs;                                    // Milliseconds (g_dtRawMs) since the last wheel step; reset on every step and by Reset. Accumulated ev
    u16 pad06e;                                    // Never referenced; alignment between the u16 at 0x6c and the float at 0x70.
    float angVel;                                  // Angular velocity in rad/s, recomputed each frame as (targetAngle - currentAngle) * 2 (fadd st,st) an
    u32 unk074;                                    // A real 4-byte slot between the two floats that NOTHING in references (per the offs
    float targetAngle;                             // Target wheel angle = index * PI/2 (1.5707964); stepped by +-PI/2 per press and re-derive
};

struct ScnClassRegEntry {
public:
    ScnFactoryFn factory;                          // class factory
    void *iconA;                                   // inventory icon image resource (first u16 = image index)
    void *iconB;                                   // alternate icon
    u32 classFlags;
};

class Screen {
public:
#ifdef SDW_MEMBERS_Screen
    SDW_MEMBERS_Screen
#endif
    virtual ~Screen();                                // Screen_ScalarDeletingDtor
    u32 *Layers4(u16 index);
    u32 *Layers60(u16 index);
    s32 Init(s32 unused);                                        /* (ret 4; the argument is not read) */
    float ScaleX(s32 x);                                         /* Screen_ScaleX */
    float ScaleY(s32 y);                                         /* Screen_ScaleY */
    u16 LayerIndex(u32 *layer);
    float Draw2D_LayerToZ(u32 *layer);
    void Clear(u32 color);
    void SetProjection(s32 dist);
    s32 mirrorRenderFlag;                          // absolute address (g_screen + 4)
    u16 mirrorRenderOn;                            // absolute address (g_screen + 8)
    s16 mirrorPlaneVert2x;                         // absolute address (g_screen + 0xa)
    u16 virtWidth;                                 // virtual screen width, 0x200 (512) written by Screen_Init
    u16 virtHeight;                                // virtual screen height, 0xf0 (240) written by Screen_Init
    s16 viewportX;                                 // viewport origin x, 0 (Screen_Init)
    s16 viewportY;                                 // viewport origin y, 0 (Screen_Init)
    s16 viewportWidth;                             // _ftol(g_pFrustrum->viewportWidth) (Screen_Init)
    s16 viewportHeight;                            // _ftol(g_pFrustrum->viewportHeight) (Screen_Init)
    u8 _pad018[0x68];
    void *scratch4;                                // malloc(4) by Screen_Init
    void *scratch60;                               // malloc(0x60) by Screen_Init
    s32 projDist;                                  // projection distance; Screen_SetProjection stores its argument (Screen_Init passes 0x180)
};

struct ScrollTextFlagBits {
public:
    s32 open : 1;                                  // (bits) g_scrollTextFlags bit 0: the player took over the paging with the action button (ScrollText
    s32 closed : 1;                                // (bits) bit 1: the player closed the text (or 2); cleared with and -3
    s32 hasPrev : 1;                               // (bits) bit 2: a previous page exists (Text_PageStep, and -5 / shl 2); read shl 0x1d / sar 0x1f
    s32 hasNext : 1;                               // (bits) bit 3: a next page exists (Text_PageStep, and -9 / shl 3); read shl 0x1c / sar 0x1f (0x5
    s32 pageTurned : 1;                            // (bits) bit 4: a page was turned this call (ScrollText_Run and -0x11 / shl 4; cleared at 0x53299
};

class Seaweed : public ScnBody {
public:
    virtual void PostLoadInit();                   // Seaweed_PostLoadInit (override)
    virtual void Update();                         // Seaweed_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Seaweed_HandleMessage (override)
    virtual void Reset();                          // Seaweed_Reset (override)
};

class SecretDoor : public ScnLogic {
public:
    virtual void PostLoadInit();                   // SecretDoor_PostLoadInit (override)
    virtual void Update();                         // SecretDoor_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SecretDoor_HandleMessage (override)
};

class Seed : public ScnBody {
public:
    virtual void PostLoadInit();                   // Seed_Init (override)
    virtual void Update();                         // Seed_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Seed_HandleMessage (override)
    virtual void Reset();                          // Seed_Reset (override)
    void SetState(u8 value);
    u8 state;                                      // SeedState 0..4 (Seed_SetState).
    Vec3s homePos;                                 // Init pos (when in the world) or pos at msg 9 arg 1; restored by msg 0x55.
    Vec3s *treePosReply;                           // Scratch: the msg 0x3280 reply pointer (Tree.seedPos), filled with the tree position.
    TimeTravelArg *arrivalQuery;                   // Scratch: the msg 0x3281 argument (sphere's wolf/gossamer arrival query); .pos is written with a box
    ScnObject *sphere;                             // the TimeMachineSphere: FindByClass(0x56) in Init, or NULL; no reader in the Seed code.
    Tree *tree;                                    // TREE property (Scn_GetPropObject offset 4); receives msg 0x3300 (visible = state 4) on every msg 0x3
    Sheep *sheepScratch;                           // Scratch copy of g_pSheepOutOfZone during msg 0x3c; the sheep that gets moved out of a tre
    ScnObject *gossamer;                           // Gossamer_Lev08 (class 0x57) from FindByClass, or NULL. Moved to a box corner by msg 0x3c.
    ZoneList zones;                                // zone list {boxes, count}: BOXES id list (past-era planting boxes, 16-byte Box). Used for the plant t
    Box presentBoxes[10];                          // Copies of boxes[i] with min and max shifted by g_timeMachineOffsetToPresent; used only by msg 0x3c (
};

struct SeesawBodyEntry {
public:
    s32 landingSpeed;                              // the rider's landing speed as seesaw_AddBody records it (0 when resting, at least 512 when landing, 4
    s32 canLaunch;                                 // 0 when the rider stood in an ejection box's side (the landing arms that box instead); a later, faste
    ScnObject *obj;                                // the rider ( reads it at +8)
};

struct SeesawPendingBits {
public:
    s32 negative : 1;                              // (bits) bit 0 (4-byte signed unit): launch pending on the negative side | seesaw +0xa3c: signed one-bit read
    s32 positive : 1;                              // (bits) bit 1 (4-byte signed unit): launch pending on the positive side
    s32 rock : 1;                                  // (bits) bit 2 (4-byte signed unit): set by a falling/rolling Rock landing (msg 0x5C == 1)
};

struct SensibleButtonFlagBits {
public:
    u8 pressed : 1;                                // (bits) bit 0 (1-byte unsigned unit) | SensibleButton +0x72: and access bits 1/0
    u8 enabled : 1;                                // (bits) bit 1 (1-byte unsigned unit)
};

class SensibleButton : public ScnBody {
public:
    virtual void PostLoadInit();                   // SensibleButton_Init (override)
    virtual void Update();                         // SensibleButton_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SensibleButton_HandleMessage (override)
    virtual void Reset();                          // SensibleButton_Reset (override)
    u8 locked;                                     // 1 = no detection (no targets, or msg 0x4D arg 1 which also shows the pressed anim)
    u8 type;                                       // PROPERTY_SENSIBLEBUTTON_TYPE: 10/14 ground-snap ignoring objects, 12 starts hidden and disabled, 14
    CollBox *pressBox;                             // model box 0; translated to pos for the occupancy grid query
    Vec3s homePos;                                 // pos after the Init ground snap; restored by Reset (unless a Train exists) and msg 0x1000
    SensibleButtonFlagBits buttonFlags;            // SensibleButtonFlags
    ScnObject *occupants[4];                       // objects on the plate this frame (zeroed each Update); passed as arg of MSG_SWITCH_ON
    u16 clickSound;                                // channel of sfx 0x36 (press and release click)
    ScnObject *targets[8];                         // objects from the TARGET id list
    ScnObject *targets2[8];                        // objects from the TARGET2 id list
    u16 targetCount;                               // entries in targets (Scn_FindIdList count; not clamped to 8)
    u16 target2Count;                              // entries in targets2 (not clamped to 8)
};

class SfxCineManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // SfxCineManager_PostLoadInit (override)
    virtual void Update();                         // SfxCineManager_Update (override)
    virtual void Render(Camera *view);             // SfxCineManager_Render (override)
    u8 typeSfx;                                    // PROPERTY_SFXCINEMANAGER_TYPESFX (prop 0x24) truncated to a byte. 1 = drifting particles (SFXCINE_TYP
    s32 spawnPending;                              // 1 = emit one particle on this Update. Raised when the spawn timer wraps, passed as the spawn argumen
    s32 spawnTimer;                                // Countdown to the next particle, decremented by g_dtMs (1000 per second) and reloaded from s
    s32 spawnPeriod;                               // (LIFETIME << 12) / NBSFX, computed with the RAW NBSFX rather than the copy
    Vec3s targetOffset;                            // POSX/POSY/POSZ (props 0x10/0x14/0x18) truncated to s16 each; added to the target object's position t
    ParticleEmitter emitter;                       // The inline emitter (0x24-byte header, so 0x58..0x7b); its slot and particle buffers are allocated by
    EmitterFadeParams fadeParams;                  // Parameters for typeSfx 2: life = LIFETIME<<12, fadeStart 0, spawnInterval 0x400, sizeStart = SIZE, s
    EmitterDriftParams driftParams;                // Parameters for typeSfx 1: hSpeed = (s16)MOVESPEED, vSpeed -60 (upward), life = LIFETIME<<12, spawnIn
    EmitterRiseParams riseParams;                  // Parameters for typeSfx 4: riseSpeed -120 (upward), life = LIFETIME<<12, spawnInterval 0x400, size =
    u8 _pad0bc[0x18];
    ScnObject *target;                             // PROPERTY_SFXCINEMANAGER_TARGET (prop 0x20) via Scn_GetPropObject. When set, particles spawn at targe
};

struct ShadowScratch {
public:
    Vec3i tangent;                                 // (n.y, -n.x, 0) scaled to the shadow radius by Vec3i_SetLength (Shadow_Update)
    u8 _pad00c[0x4];
    Vec3i bitangent;                               // normal x tangent (Vec3i_Cross out at +0x10:)
    u8 _pad01c[0x4];
    Vec3i normal;                                  // the ground normal widened to s32
};

class Shark : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Shark_PostLoadInit (override)
    virtual void Update();                         // Shark_Update (override)
    virtual void Render(Camera *view);             // Shark_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Shark_HandleMessage (override)
    virtual void Reset();                          // Shark_Reset (override)
    void UpdateWolfChecks();
    void UpdateAi();
    void RunState();
    void ForceState(u8 newState);
    void SetState(u8 newState);
    void AimRotation(Vec3s *from, Vec3s *to, Vec3s *out, s32 pitch);
    void FaceWolf(s32 pitch);
    void AimBiteRot(s32 pitch);
    void ApplyBiteRot();
    void FollowPath();
    u16 HeadingXZ(Vec3s *from, Vec3s *to);
    u16 CornerHeading();
    void UpdateBreathFx();
    void UpdateChargeFx();
    void UpdateRippleFx();
    u8 state;                                      // 1 patrol, 3 aim at Ralph, 4 hold and face him, 5 lunge blocked, 6 back to patrol, 7 wind-up, 8 lunge
    s32 detectRange;                               // PROPERTY_SHARK_DETECTRANGE (prop +0xc); 640 in Lvl-04
    s32 detectHeight;                              // PROPERTY_SHARK_DETECTHEIGHT (prop +8); the vertical window in which Ralph wakes t
    s32 detectRangeCopy;                           // second copy of detectRange; never read again in the class
    s32 watchOverCave;                             // PROPERTY_SHARK_BOOLWATCHOVERCAVE != 0 (prop +0); 0 on both Lvl-04 sharks. Gates s
    s32 biteAccepted;                              // Ralph's reply to MSG_KILL with cause 5 (arg 5, msgId 0) when the bite starts; cl
    Box *zone;                                     // PROPERTY_SHARK_BOX (Scn_GetPropBox prop +4): the water volume the shark patrols;
    TrajFollower path;                             // the patrol path: TrajFollower_Init(PROPERTY_SHARK_TRAJ, speed 350, bias 0x800, arrive radius 50) (0x
    s16 homeVert;                                  // pos.y saved by PostLoadInit; never read
    s32 unusedBc;                                  // cleared by PostLoadInit and never read
    u16 unusedC0;                                  // cleared by PostLoadInit and never read
    s32 cornering;                                 // 1 while the patrol is rounding a waypoint: set when the distance to the waypoint drops below 80 (0x4
    u16 cornerHeadingIn;                           // heading of the leg entering the corner
    u16 cornerHeadingOut;                          // heading of the leg leaving it
    s32 cornerDist;                                // distance travelled since the corner began; the blend parameter of Shark_CornerHeading
    s32 wolfDist;                                  // Vec3s_Dist(pos, Ralph's pos), refreshed at the top of Update
    s32 aimTimer;                                  // 2000 ms on entering state 3, counted down by g_dtMs; 0 starts the wind-up
    Vec3s lungeFrom;                               // the shark's position when the lunge is aimed
    Vec3s lungeTo;                                 // Ralph's position when the lunge is aimed
    Vec3s lungeVel;                                // lunge velocity: the aim direction scaled to 900 units/s, reversed for the swim b
    s32 lungeLeft;                                 // distance still to travel; set to the aim distance, reduced by each step, 0 end
    s32 lungeLen;                                  // the full lunge distance, kept for the swim back (compared)
    Vec3s biteRot;                                 // rotation that points from Ralph back to the shark (Shark_AimBiteRot); copied into rot at th
    s16 idleTimer;                                 // 3000 ms between idle animations, counted down by g_dtMs in state 1
    s32 idlePending;                               // 1 while an idle animation is queued (set when Ralph passes close by, cleared when it pla
    u16 idleAnim;                                  // the animation state 1 plays: 6 by default, 5 for the look-at-Ralph idle
    InlineEmitter8 breathFx;                       // 8 bubbles rising from 90 units in front of the shark whenever it is 150+ below the water surface (Sh
    EmitterRiseParams breathParams;                // rise -120/s, life 0x2000, spawn every 0x400, size 10, sheet 2; size and cap are
    TrailEmitter chargeFx;                         // 16 bubbles behind the shark during the wind-up and the lunge (Shark_UpdateChargeFx); TrailE
    EmitterRiseParams chargeParams;                // rise -120/s, life 0x2000, spawn 0x80, size 10, sheet 2; the lunge (state 8) swap
    InlineEmitter8 rippleFx;                       // 8 flat surface rings drawn on the water while the shark swims within 150 of the surface (Shark_Updat
    EmitterFadeParams rippleParams;                // life 0x2000, fade from 0, spawn 0x400, size 50 -> 200, sheet 1
    Box *waterBox;                                 // the water zone box the shark is in, refreshed by each of the three effect updates (0x47683
    s32 wolfInZone;                                // Ralph's position is inside zone
    s32 wolfInWater;                               // Ralph is inside one of the global water zones
    s32 wolfInReach;                               // wolfDist <= 160, the bite distance
};

struct Vec4s {
public:
    s16 x;
    s16 y;                                         // vertical points down
    s16 z;
    s16 pad;                                       // never written by Mat34s_TransformVec3s, copied out uninitialised
};

struct SharkFxScratch {
public:
    Mat34s m;                                      // Shark_UpdateBreathFx's view of g_collScratchA: Mat34s_FromEulerScaled(&rot, &m, 0) (0x4766a
    Vec4s out;                                     // Mat34s_TransformVec3s output ( pushes scratch+0x20 as the out argument)
    Vec3s offset;                                  // the local-space spawn offset written just before the transform: (0,0,-90) for the breath bubbles (0x
};

struct WallAvoidFlagBits {
public:
    u16 active : 1;                                // (bits) bit view of WallAvoid.flags (+6): the last move hit a wall. Word-wide one-bit reads (mov ax,[+6]; an
    u16 turnSide : 1;                              // (bits) turn side: +0x300 instead of -0x300 (shr dx; and dx,1; cleared with and 0xfffd
    u16 reserved : 14;                             // (bits) remaining bits
};

class WallAvoid {
public:
#ifdef SDW_MEMBERS_WallAvoid
    SDW_MEMBERS_WallAvoid
#endif
    void Reset();
    void ComputeSteer(const Vec3s *from, u16 fallbackHeading, const Vec3s *target, s16 keepDist, u16 *outSteerHeading, u16 *outTargetHeading, s16 *outDistError);
    void UpdateFromContact(const ContactInfo *contact, u16 moveResult);
    Vec3s wallNormal;                              // mean wall normal copied from ContactInfo.wallNormalMean on a wall hit
    WallAvoidFlagBits flags;                       // WallAvoidFlags as u16 one-bit fields (WallAvoidFlagBits): 1 active (last move hit a wall), 2 turn si
};

class Sheep : public ScnMobile {
public:
#ifdef SDW_MEMBERS_Sheep
    SDW_MEMBERS_Sheep
#endif
#ifdef SDW_EXTRA_Sheep
    SDW_EXTRA_Sheep
#endif
    virtual void PostLoadInit();                   // Sheep_PostLoadInit (override)
    virtual void Update();                         // Sheep_Update (override)
    virtual void Render(Camera *view);             // Sheep_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Sheep_HandleMessage (override)
    virtual void Reset();                          // Sheep_Reset (override)
    ScnObject *FindNearestAttractor(s32);
    ScnObject *FindScentSourceInRange();
    s32 CanSmellObject(ScnObject *);
    s32 FallStep(s32, s32, s32);
    s32 IsAtTarget(ScnObject *, s32);
    s32 IsAvailable();
    s32 IsTargetSameLevel(ScnObject *);
    s32 IsTargetValidWithin(ScnObject *, s32);
    s32 IsTargetWithinRange(ScnObject *, s32);
    s32 MoveToward(Vec3s *, s32);
    s32 ShouldEnterFall();
    s32 ShouldIgnoreLures();
    s32 TestScentSource(FlockScentSource *);
    s32 Think_FindAttraction();
    s32 TurnToHeading(s32);
    u32 TryReserveScentSource();
    u32 TryReserveTarget(s32, u32);
    void AttachIceBlock();
    void DetachIceBlock();
    void EnterIdleState();
    void FrozenFloatStep();
    void Move(Vec3s *, Vec3s *, s32);
    void NextIdleAnim();
    void ReleaseTarget();
    void ResetVars();
    void SetState(u8, u16, s32, s32);
    void SetStateId(u8);
    void SetStateQuiet(u8);
    void StartApproach(s32);
    void StartSplashFx();
    void StopSplashFx();
    void TurnToTarget();
    void UpdateZoneMask();
    s16 slopeLimit;                                // 4.12 min ground-normal passed to collision sweep; default 0xB54, 0x1000 after seesaw launch
    s16 slopeOverrideMs;                           // ms countdown (g_dtMs) after which slopeLimit returns to 0xB54; set 500 by msg 0x27 (moved from ScnMo
    ScnObject *pTarget;                            // current attractor / scent source / leader; reserved in g_flockReservedTargets when flag 2
    ScnObject *pPlatform;                          // object that claimed the sheep via msg 0x18 (carpet, train, elastic, magnet, GossamerOnde...); gets m
    ScnObject *pScriptHolder;                      // sender of msg 0x23 (Sam/Monolithe/PrayingGhost); only it may grab while flag 0x20
    s32 fallTime;                                  // time falling in 1/4096 s; fall speed = fallTime*1500>>12 capped 1400; tested by to enter FA
    s32 stateTime;                                 // time in current state, 1/4096 s; zeroed by Sheep_SetState
    s32 gravityTimer;                              // while >0 the update tail keeps applying Sheep_FallStep; reloaded to 0x5000 by movers
    Vec3s lastFoodPos;                             // position of the salad being eaten; next-food search origin and WALK_TO_LAST_FOOD destination
    s16 carryHeadingOffset;                        // heading relative to carrier at pickup; restored on drop
    Vec3s homePos;                                 // home: set to spawn position (ground-snapped) by Sheep_PostLoadInit; idle in-zone sheep walk back to
    s16 homeHeading;                               // heading applied with homePos by Sheep_Reset
    Vec3s respawnPos;                              // checkpoint respawn position, used ONLY while this sheep is g_pCheckpointSheep: written by msg 0x980
    s16 respawnHeading;                            // heading applied with respawnPos by Sheep_Reset for the checkpoint sheep
    Vec3s walkDest;                                // destination for WALK_TO_POINT 0x19 (msg 0x981), y re-queried from ground
    s16 fallLimitY;                                // Y cap for death fall state 0x14 (from death zone +0xC); default 32000
    Vec3s anchorPos;                               // leash centre set by msg 0x2E (platforms, raft, lift...)
    s32 anchorRadiusSq;                            // leash radius squared; beyond it -> state 0x1C
    WallAvoid wallAvoid;                           // {Vec3s wallNormal; u16 flags (bit0 active, bit1 turn side)} steering helper shared with Wolf (+0x2B0
    AltModel models[3];                            // 0x10-byte model slots: [0] main, [1] WAR 0x52 AMOUTO03 (sleeping), [2] WAR 0xE4 AMOUTO04 (variant, f
    Box *pRepelZone;                               // SHEEPREPELBOX (WAR id 0x35) box containing the sheep; its flag bits 0x40000000/0x20000000/0x10000000
    Box *pWaterZone;                               // water box (list) containing pos.y-60; flags 0x1000000 = freezing water, 0x2000000 = no-floa
    u16 sfxHandle;                                 // handle of looping/voice sfx (0x142 bleat, 0x112 snore, 0x3D eat, 0x28 shiver); stopped on state chan
    u8 state;                                      // enum SheepState
    u8 modelIdx;                                   // currently loaded entry of models[]
    s8 index;                                      // index in g_sheepTable, bit number in the flock masks and think frame slot
    s8 idleLoops;                                  // remaining idle animation loops before switching variant
    u32 flags;                                     // enum SheepFlags
};

class SheepCostume : public ScnBody {
public:
    virtual void PostLoadInit();                   // SheepCostume_Init (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SheepCostume_HandleMessage (override)
    Vec3s homePos;                                 // Position saved by Init (after the ground snap) and by msg 9 arg 1; restored by msg 0x55.
};

class SignPost : public ScnLogic {
public:
    virtual void PostLoadInit();                   // SignPost_Init (override)
    virtual void Update();                         // SignPost_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SignPost_HandleMessage (override)
    virtual void Reset();                          // SignPost_Reset (override)
    u8 state;                                      // SignPostState
    Vec3s savedPos;                                // restored by Reset
    u16 distance;                                  // PROPERTY_SIGNPOST_DISTANCE
    u32 textNum;                                   // PROPERTY_SIGNPOST_TEXTNUM: base string index
    char *text;                                    // current string
    s8 textVariant;                                // -1 until first read, then the global read counter at that time; variant 1 shows the question box
    ScnObject *anvil;                              // Scenaric_FindByIdList(ID_ANVIL); gets msg 0x1280; its +0x6c is polled in state 2
    CamSetup *camera;                              // Scn_FindIdList(ID_CAMERA)[0]: camera setup used while the anvil drops
    u8 _pad060[0x4];
    u32 wolfFrozen;                                // reply of Wolf msg 0xE
};

class SignPostAnimated : public ScnBody {
public:
    virtual void PostLoadInit();                   // SignPostAnimated_Init (override)
    virtual void Update();                         // SignPostAnimated_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SignPostAnimated_HandleMessage (override)
    u8 state;                                      // SignPostAnimatedState
    u16 distance;                                  // PROPERTY_SIGNPOSTANIMATED_DISTANCE
    u32 textIndex;                                 // PROPERTY_SIGNPOSTANIMATED_INDEXTEXT
    char *text;                                    // Text_GetClassString((u8)textIndex)
    u32 hidden;                                    // PROPERTY_SIGNPOSTANIMATED_HIDDEN; cleared by msg 0xE81
    u32 wolfFrozen;                                // reply of Wolf msg 0xE
};

class SignPostSimple : public ScnLogic {
public:
    virtual void PostLoadInit();                   // SignPostSimple_Init (override)
    virtual void Update();                         // SignPostSimple_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SignPostSimple_HandleMessage (override)
    virtual void Reset();                          // SignPostSimple_Reset (override)
    u8 state;                                      // SignPostSimpleState
    Vec3s savedPos;                                // restored by Reset; updated by msg 9 arg 1 and msg 0x13
    u16 distance;                                  // PROPERTY_SIGNPOSTSIMPLE_DISTANCE: Manhattan XZ read radius (all data 200)
    u32 textIndex;                                 // PROPERTY_SIGNPOSTSIMPLE_INDEXTEXT
    char *text;                                    // Text_GetClassString(this, (u8)textIndex)
    u32 wolfFrozen;                                // reply of Wolf msg 0xE; msg 0xF sent on close when set
};

class SignTips : public ScnBody {
public:
    virtual void PostLoadInit();                   // SignTips_PostLoadInit (override)
    virtual void Update();                         // SignTips_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SignTips_HandleMessage (override)
    virtual void Reset();                          // SignTips_Reset (override)
    void SetState(u8 value);
    u8 state;                                      // 1 wait for GF_FADE_IN to clear, 2 appearing with the hint camera running, 3 wait for the pad release
    s32 wolfFrozen;                                // Non-zero while this sign holds the Wolf frozen: set from the reply of msg 0xe, and recomputed in sta
    CamSetup *camRecord;                           // PROPERTY_SIGNTIPS_CAMERA (prop 0) via Scn_GetPropCamera: the camera setup record {u16 focal
    u8 rebirthCount;                               // Number of respawns so far with Ralph inside checkpointBox; incremented once per Reset.
    u8 rebirthsNeeded;                             // PROPERTY_SIGNTIPS_NBREBIRTH (prop 0x10), narrowed to a byte; the tip fires once rebirthCount reaches
    Box *checkpointBox;                            // PROPERTY_SIGNTIPS_CHECKPOINT (prop 4); Reset only counts a death when Ralph is inside it, and the te
    s32 boxVisited[10];                            // One flag per TRAJBOXES entry, set to 1 when Ralph enters that box; state 9 begins once all trajBoxCo
    ZoneList trajBoxes;                            // zone list {boxes, count}: Pointer array returned by Scn_FindIdList for PROPERTY_SIGNTIPS_TR
    u32 triggerDist;                               // PROPERTY_SIGNTIPS_DIST (prop 8): in state 9 the hint camera only starts once Wolf msg 0x41a returns
    char *tipText;                                 // Localised tip string from Text_GetClassString with PROPERTY_SIGNTIPS_INDEXTEXT (prop 0xc) n
    s16 textRect[4];                               // Screen rect handed to Ui_DrawSubtitleBox: left 0x32, top 0x32, right 0x200-0x64 = 0x19c, bottom 0xf0
};

struct SlidingIceCubeFlagBits {
public:
    u8 allowX : 1;                                 // (bits) bit 0 (1-byte unsigned unit): ALLOWPUSHONXAXIS. +0x83 is byte bit storage: /0x4f356
    u8 allowY : 1;                                 // (bits) bit 1 (1-byte unsigned unit): ALLOWPUSHONYAXIS (z)
    u8 bounced : 1;                                // (bits) bit 2 (1-byte unsigned unit): already bounced
};

class SlidingIceCube : public ScnBody {
public:
    virtual void PostLoadInit();                   // SlidingIceCube_Init (override)
    virtual void Update();                         // SlidingIceCube_Update (override)
    virtual void Render(Camera *view);             // SlidingIceCube_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SlidingIceCube_HandleMessage (override)
    virtual void Reset();                          // SlidingIceCube_Reset (override)
    s32 QueryRiders(ScnObject **out);
    u16 CrushMove(Vec3s *delta, u16 resolveFlags);
    u16 SlideMove(Vec3s *delta, u16 resolveFlags);
    void ApplyGravity(Vec3s *velocity);
    void HideSplash();
    void PlaySlideSound();
    void SetState(u8 value);
    void ShowSplash(Box *water);
    ZoneList pushBoxes;                            // zone list {boxes, count}: ALLOWPUSHBOXES id list: the cube may only slide while its next position is
    s32 slideSpeed;                                // 800 on a push; after the bounce it decelerates at 4000 u/s^2. In state 3 it accelerates to 800 at 40
    s32 fallTime;                                  // Airborne time in g_dt ticks, reset on floor contact. Gravity = fallTime*4000>>12 clamped to [200,150
    Vec3s homePos;                                 // Position after the ground snap in Init; restored by Reset.
    s16 slideDir;                                  // 12-bit slide heading (away from the pusher, snapped to 90 degrees); reversed once on a wall.
    ScnObject *frozenRiver;                        // The level's FrozenRiver (class 0x53) if any. Non-NULL disables kicking and the kick prompt; it recei
    u16 slideSound;                                // Channel of the scrape sound 0x7C.
    u8 state;                                      // SlidingIceCubeState 0..3.
    SlidingIceCubeFlagBits cubeFlags;              // SlidingIceCubeFlags: 1 ALLOWPUSHONXAXIS, 2 ALLOWPUSHONYAXIS (z), 4 already bounced.
    u8 splashVisible;                              // Draws and animates the shared splash body.
    u8 splashReady;                                // 1 when the splash record (export 0x5A) was built in Init.
};

struct SmallRockFlagBits {
public:
    s32 thrown : 1;                                // (bits) bit 0 (4-byte signed unit) | SmallRock +0x174: signed one-bit extraction
    s32 floating : 1;                              // (bits) bit 1 (4-byte signed unit)
    s32 hasIce : 1;                                // (bits) bit 2 (4-byte signed unit)
    s32 inWater : 1;                               // (bits) bit 3 (4-byte signed unit)
    s32 initAtReset : 1;                           // (bits) bit 4 (4-byte signed unit)
};

struct SmallRockMoveBits {
public:
    u8 pushed : 1;                                 // (bits) bit 0 (1-byte unsigned unit) | SmallRock +0x171: unsigned byte extraction
    u8 movableContact : 1;                         // (bits) bit 1 (1-byte unsigned unit)
};

class SmallRock : public ScnMobile {
public:
    virtual void PostLoadInit();                   // SmallRock_Init (override)
    virtual void Update();                         // SmallRock_Update (override)
    virtual void Render(Camera *view);             // SmallRock_Render (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // SmallRock_CustomCollide (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SmallRock_HandleMessage (override)
    virtual void Reset();                          // SmallRock_Reset (override)
    u16 Move(Vec3s *delta, u16 resolveFlags);
    void ApplyGravity(Vec3s *velocity);
    void ReturnHome();
    void SetState(u8 value);
    void StartFall();
    s32 fallTime;                                  // ticks of fall (g_dt); gravity speed = fallTime*1500>>12 capped at 1000; zeroed on floor contact
    u32 sliding;                                   // pos is inside slidingBox (x/z): slides at 400 u/s toward -z
    CollBox *collBox;                              // model box 0, or box 1 when box 0 lacks flag 4 and there is more than 1 box; lift-height test and pus
    Box *slidingBox;                               // SLIDINGBOX, only if its id list holds exactly one box
    LaunchArc launch;                              // copy of the msg 0xC LaunchArc; t at +0xb0 (advanced by g_dtMs>>2), camera +0xb4, obj +0xb8, camParam
    CamShot camShot;                               // launch camera shot
    s16 carryYaw;                                  // yaw relative to the carrier while attached (msg 4), restored on drop
    ZoneList deathBoxes;                           // zone list {boxes, count}: Scn_FindIdList(10 WAR_IDO_DEATHBOX) | count (+4): count for deathBoxes
    Vec3s homePos;                                 // respawn position (ReturnHome)
    AltModel baseModel;                            // saved intact model, restored when leaving state 4
    AltModel brokenModel;                          // WAR_IDO_AROCHE5B (id list 0x58), swapped in by SetState(4)
    ScnBody iceBody;                               // embedded ScnBody from WAR_IDO_AGLACON1 (only Lvl-06 exports it); drawn and collided while FLOATING;
    u8 state;                                      // SmallRockState
    SmallRockMoveBits moveFlags;                   // SmallRockMoveFlags
    SmallRockFlagBits rockFlags;                   // SmallRockFlags
};

struct SndBankEntry {
public:
    u32 soundId;                                   // Sample id from the .SND entry header, compared (as u32) against Sound_Play's soundId. Array g_sndBan
    u32 dataSize;
    u32 loop;                                      // == 1 forces StaticSound.looping on in Sound_Play, even without playFlags bit 1.
};

class Snowball : public ScnLogicShadowed {
public:
    virtual void PostLoadInit();                   // Snowball_PostLoadInit (override)
    virtual void Update();                         // Snowball_Update (override)
    virtual void Render(Camera *view);             // Snowball_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Snowball_HandleMessage (override)
    virtual void Reset();                          // Snowball_Reset (override)
    s32 TryResize(s16 newScale, Vec3s *delta);
    u16 Move(Vec3s *delta, u16 resolveFlags);
    void ApplyGravity(Vec3s *vel);
    s32 fallTime;                                  // 4.12 s airborne: += g_dt in state 2 and on each push, capped at 0x5000 by ApplyGravity; z
    s32 rollTime;                                  // 4.12 s of growing rolls (+= g_dt in TryResize): the scale target is rollTime*0x233/0x14000
    s16 baseRadius;                                // half the x extent of model box 0 at scale 1.0
    u8 state;                                      // SnowballState: 0 resting, 1 being pushed, 2 falling
    u8 pushed : 1;                                 // (bits) bit 0: set by each push (msg 0xA), cleared by the next Update; when a frame passes without
    ZoneList zone;                                 // PROPERTY ZONE id list (Scn_FindIdList): the ball only rolls and grows while its next pos
    s16 radius;                                    // current radius = baseRadius*scale >> 10 (set by TryResize); rolls the matrix
    s16 scale;                                     // 6.10 scale (0x400 = 1.0): 0x400 at load, 0x100 after the first TryResize; drawn with Mat34s_ApplySca
    Mat34s rollMatrix;                             // orientation, Mat34s_FromEulerScaled(&rot) at load, rolled by Matrix_RollByDisplacement on
    u16 rollSound;                                 // handle of the rolling sound 0x7b (Sound_Play), stopped when the ball stops being pushed
};

class SnowyGround : public ScnBody {
public:
    virtual void PostLoadInit();                   // SnowyGround_PostLoadInit (override)
    virtual void Update();                         // SnowyGround_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SnowyGround_HandleMessage (override)
    u8 state;                                      // SnowyGroundState: 1 intact (anim 1 loop), 2 collapsing (anim 0 once), 0 collapsed
    u8 collapsed : 1;                              // (bits) bit 0: set when an object other than itself and the Wolf lands on it; the ground query (m
};

class Sound {
public:
#ifdef SDW_MEMBERS_Sound
    SDW_MEMBERS_Sound
#endif
    virtual ~Sound();                                 // Sound_ScalarDeletingDtor
    virtual s32 CreateFromWave(SoundDevice *device, WaveFile *wave) SDW_PURE; // _purecall
    virtual s32 CreateFromFile(SoundDevice *device, char *path) SDW_PURE; // _purecall
    virtual s32 Free() SDW_PURE;                   // _purecall
    virtual s32 RestoreBuffer() SDW_PURE;          // _purecall
    virtual s32 Play() SDW_PURE;                   // _purecall
    virtual void Stop() SDW_PURE;                  // _purecall
    virtual void SetPaused(u8 pause) SDW_PURE;     // _purecall
    virtual void RewindBuffer() SDW_PURE;          // _purecall
    virtual s32 SetBufferPosition(float frac) SDW_PURE; // _purecall
    virtual u8 IsPlaying() SDW_PURE;               // _purecall
    virtual s32 Sound_SetBufferVolume(float volume) SDW_PURE; // _purecall
    virtual s32 SetBufferPan(float pan) SDW_PURE;  // _purecall
    virtual s32 Sound_SetBufferFrequency(u32 hz) SDW_PURE; // _purecall
    virtual s32 FillBuffer() SDW_PURE;             // _purecall
    u8 looping;                                    // 1 = pass DSBPLAY_LOOPING to IDirectSoundBuffer::Play. Also switches Sound_SetBufferPosition to fract
    WaveFile *wave;                                // The source .WAV reader. Deleted through its own vtable slot 0 when waveIsExternal == 0.
    u8 waveIsExternal;                             // 1 = the WaveFile belongs to somebody else, do not delete it. Set to 1 by both ctors and by CreateFro
    IDirectSoundBuffer *buffer;                    // The secondary buffer. Released (vtbl+8) by every teardown path; NULL means the sound is not created.
    u32 bufferBytes;                               // DSBUFFERDESC.dwBufferBytes. For StaticSound it is the whole data chunk; for StreamSound it is notify
    s32 volumeCb;                                  // Current attenuation in centibels, -6000..0. Initialised to -3000 by both derived ctors.
    s32 panCb;                                     // Current pan in centibels, -10000..+10000 (DSBPAN_LEFT..DSBPAN_RIGHT).
    u32 baseFrequency;                             // Sample rate. Filled by IDirectSoundBuffer::GetFrequency right after creation (StaticSound only), and
};

class StaticSound : public Sound {
public:
#ifdef SDW_MEMBERS_StaticSound
    SDW_MEMBERS_StaticSound
#endif
    virtual ~StaticSound();                           // StaticSound_ScalarDeletingDtor
    virtual s32 CreateFromWave(SoundDevice *device, WaveFile *wave); // StaticSound_CreateFromWave (override)
    virtual s32 CreateFromFile(SoundDevice *device, char *path); // StaticSound_CreateFromFile (override)
    virtual s32 Free();                            // StaticSound_Free (override)
    virtual s32 RestoreBuffer();                   // StaticSound_RestoreBuffer (override)
    virtual s32 Play();                            // StaticSound_Play (override)
    virtual void Stop();                           // StaticSound_Stop (override)
    virtual void SetPaused(u8 pause);              // StaticSound_SetPaused (override)
    virtual void RewindBuffer();                   // Sound_RewindBuffer (override)
    virtual s32 SetBufferPosition(float frac);     // Sound_SetBufferPosition (override)
    virtual u8 IsPlaying();                        // StaticSound_IsPlaying (override)
    virtual s32 Sound_SetBufferVolume(float volume); // Sound_SetBufferVolume (override)
    virtual s32 SetBufferPan(float pan);           // Sound_SetBufferPan (override)
    virtual s32 Sound_SetBufferFrequency(u32 hz);  // Sound_SetBufferFrequency (override)
    virtual s32 FillBuffer();                      // StaticSound_FillBuffer (override)
};

struct SoundChannel {
public:
    u8 paused;                                     // not flag0. Set to 1 by Sound_PauseAll for active channels (with StaticSound_SetPaused(1));
    u8 active;                                     // 1 while the channel is playing. Sound_IsPlaying (41 callers) is exactly a read of this byte
    u8 owned;                                      // set to 1 on allocation; Sound_Stop returns early unless this is non-zero, then clears it. Acts as th
    u8 keepBuffer;                                 // Written by Sound_AllocChannel: 1 when the chosen channel already holds this sample (same so
    void *owner;                                   // the object that started the sound. Sound_Play uses (owner, soundId) as a duplicate key and Sound_Sto
    u16 soundId;                                   // sample id passed to Sound_Play; zeroed by Sound_Stop
    u16 sampleSlot;                                // index into g_sndBankEntries/g_sndBankWaves found by Sound_Play's linear id search. Sound_MixerTick p
    u8 playFlags;                                  // not priorityOrFlags: Sound_Play's 4th argument, a SoundPlayFlags bit set. 0x01 loop, 0x02 positional
    float volume;                                  // playback volume as a 0..1 float. Written by Sound_Play from its 3rd argument and by Sound_SetVolume
    u32 baseSampleRate;                            // copied from the embedded StaticSound baseFrequency (+0x40) right after CreateFromWave. The DirectSou
    float mixedGain;                               // Sound_UpdateChannelGain writes (g_sfxVolume/255) * distanceGain * volume(+0x10); Sound_Mixe
    float rate;                                    // playback-rate multiplier: Sound_MixerTick multiplies baseSampleRate by it to get the buffer frequenc
    StaticSound sound;                             // EMBEDDED StaticSound (not a pointer), spanning 0x20..0x43.
};

struct SoundDeviceEntry {
public:
    char description[40];                          // the DirectSound device description DS_EnumCallback strcpy's here (unbounded); SoundDevice_G
    GUID guid;                                     // the device GUID DS_EnumCallback copies here (left zero for the primary device, whose GUID pointer is
};

class SoundDevice {
public:
#ifdef SDW_MEMBERS_SoundDevice
    SDW_MEMBERS_SoundDevice
#endif
    virtual ~SoundDevice();                           // SoundDevice_ScalarDeletingDtor
    u8 StepDevice(u8);                                           /* (launcher) */
    u8 SetDeviceIndex(u8);
    void GetDeviceName(char *);
    s32 Stub0();
    s32 Stub1();
    HRESULT Init(HWND hWnd, u32 sampleRate, u8 bitsPerSample);
    IDirectSound *GetDSound();
    u32 GetBufferLocFlags();
    u32 GetVoiceAmplitude(s32 refresh);
    u32 GetVoiceAmplitude_Unused(s32 refresh);
    u8 PollStreamEvents();
    HRESULT RegisterStreamEvent(void *stream, HANDLE *outEvent);
    u32 UnregisterStreamEvent(void *stream);
    u8 initialized;                                // Set to 1 once SoundDevice_Init has created the primary buffer.
    IDirectSound *pDS;                             // The IDirectSound object, handed to the sound layer by SoundDevice_GetDSound.
    IDirectSoundBuffer *pPrimaryBuffer;            // Primary (mixer) buffer; SetFormat is applied to it and it is left playing looped for the whole sessi
    u32 sampleRate;                                // Primary-buffer sample rate; Game_Main passes 22050.
    u8 bitsPerSample;                              // Primary-buffer sample width; Game_Main passes 16.
    u8 hasHwStreamingMixer;                        // DSCAPS.dwMaxHwMixingStreamingBuffers != 0. Decides whether secondary buffers are created LOCDEFER or
    SoundDeviceEntry devices[20];                  // Enumerated DirectSound devices (SoundDeviceEntry, 0x38 each: char description[0x28] then GUID guid a
    u32 deviceCount;                               // Number of enumerated DirectSound devices (capped at 20 by DS_EnumCallback).
    u32 deviceIndex;                               // Device chosen in the launcher.
    StreamSound *streamOwners[16];                 // Owner object of each of the 16 stream notification slots; NULL means free. Only StreamSound register
    void *streamEvents[16];                        // (HANDLE[16]; typed void *[16] by run3/snd_device so the source can index it: CreateEventA result sto
    s32 streamCount;                               // Number of registered streams. Incremented on register, decremented on unregister; SoundDevice_GetVoi
};

class StreamSound : public Sound {
public:
#ifdef SDW_MEMBERS_StreamSound
    SDW_MEMBERS_StreamSound
#endif
    virtual ~StreamSound();                           // StreamSound_ScalarDeletingDtor
    virtual s32 CreateFromWave(SoundDevice *device, WaveFile *wave); // StreamSound_CreateFromWave (override)
    virtual s32 CreateFromFile(SoundDevice *device, char *path); // StreamSound_CreateFromFile (override)
    virtual s32 Free();                            // StreamSound_Free (override)
    virtual s32 RestoreBuffer();                   // StreamSound_RestoreBuffer (override)
    virtual void Stop();                           // StreamSound_Stop (override)
    virtual void SetPaused(u8 pause);              // StreamSound_SetPaused (override)
    virtual void RewindBuffer();                   // Sound_RewindBuffer (override)
    virtual s32 SetBufferPosition(float frac);     // Sound_SetBufferPosition (override)
    virtual u8 IsPlaying();                        // StreamSound_IsPlaying (override)
    virtual s32 Sound_SetBufferVolume(float volume); // Sound_SetBufferVolume (override)
    virtual s32 SetBufferPan(float pan);           // Sound_SetBufferPan (override)
    virtual s32 Sound_SetBufferFrequency(u32 hz);  // Sound_SetBufferFrequency (override)
    virtual s32 FillBuffer();                      // StreamSound_FillBuffer (override)
    s32 CreateFromWaveEx(SoundDevice *device, WaveFile *wave, float seconds, u8 notifications);
    s32 ServiceNotify();
    s32 Play(u32 lipSync);                                       /* (non-virtual: slot 5 stays _purecall) */
    s32 ReadOrPadSilence(u8 *dest, u32 size);
    u32 GetVoiceAmplitude(s32 refresh);                          /* StreamSound_GetVoiceAmplitude */
    u8 stopped;                                    // 1 = the stream is not playing. Set by the ctor, by Create, by StreamSound_Stop and by ServiceNotify
    u32 lipSyncEnabled;                            // StreamSound_Play's argument. Non-zero arms the amplitude meter: StreamSound_GetVoiceAmplitude return
    SoundDevice *pDevice;                          // The SoundDevice this stream registered with; NULLed by StreamSound_Free after UnregisterStreamEvent.
    IDirectSoundNotify *pNotify;                   // IID_IDirectSoundNotify {b0210783-89cd-11d0-af08-00a0c925cd16} interface on the buffer, used once for
    u32 notifySize;
    u8 eofReached;                                 // 1 once a non-looping read came up short. From then on every block is filled with the silence byte, a
    u32 playProgress;                              // Total bytes of audio the play cursor has passed since the last FillBuffer. Advanced by the cursor de
    u32 writeOffset;
    u32 lastPlayCursor;                            // Previous IDirectSoundBuffer::GetCurrentPosition play cursor, used to derive the delta added to playP
    u8 bitsPerSample;                              // Cached WAVEFORMATEX.wBitsPerSample.
    u8 blockAlign;                                 // Cached WAVEFORMATEX.nBlockAlign (truncated to a byte via __ftol of the double).
    u8 channels;                                   // Cached WAVEFORMATEX.nChannels.
    u8 bytesPerSampleChannel;                      // blockAlign / channels, i.e. bytes per sample for one channel. The amplitude meter mmioReads exactly
    u32 samplesPerSec;                             // Cached WAVEFORMATEX.nSamplesPerSec. The amplitude meter uses a 16-tap window at >= 44100 (0xAC44) an
    u32 amplitudeWindow[16];                       // Moving-average ring of absolute sample magnitudes, shifted down by one each refresh with the newest
    u32 voiceAmplitude;                            // Gated lip-sync level: 0 when the mouth should be closed, the averaged magnitude when it should be op
};

class StreamPlayer {
public:
#ifdef SDW_MEMBERS_StreamPlayer
    SDW_MEMBERS_StreamPlayer
#endif
    virtual ~StreamPlayer();                          // StreamPlayer_ScalarDeletingDtor
    void Load_MusicVoiceBank();
    void StopAndFree();
    void PlayVoice(u32 clip, u8 language, u32 unused, u32 lipSync); /* clip is pushed as a dword */
    s32 UpdateVoice();
    void StopVoice();
    void RestartLevelMusic();
    void LoadLevelMusic();                                       /* starts the level's music clip */
    void SetPaused(s32 pause);
    void Halt();                                                 /* stops the current stream, state = 0 */
    void Update();                                               /* per-frame service */
    void ApplyVolume();
    s32 IsMusic();
    s32 IsBusy();                                                /* 1 while state is 3..6 */
    StreamSound *current;                          // the playing stream = this+8+slot*0x94
    StreamSound streams[2];                        // StreamSound[2] (0x94 each); double-buffered: the loader fills the idle one
    u8 activeSlot;                                 // index of the playing stream; the loader uses (activeSlot-1)&1
    u8 bankLoaded;                                 // set to 1 by Load_MusicVoiceBank once MusicVoice.BSV is read; zeroed by the ctor (
    u16 clipCount;                                 // entries in +0x134 (0x74 from MusicVoice.BSV)
    char **clipNames;                              // per-clip allocated entries from Load_MusicVoiceBank, used as file names by
    u16 currentClip;                               // clip index being loaded (g_pStreamPlayer id)
    char path[SDW_PATH_MAX];                       // full clip path built + clip name; the loader thread argument. Length inferred from the
    s32 state;                                     // stream state machine (3 load-next, 5 fade-out, 6 wait-for-load...), per
    s32 pausedState;                               // state saved by on pause and restored on resume; zeroed by the ctor (0
    s32 autoPlay;                                  // 1 by ctor: StreamSound_Play after a load completes
    u32 playArg;                                   // argument passed to StreamSound_Play (lip-sync enable)
    s32 stateStartMs;                              // g_rawTimeMs when the state was entered (fade/timeout timing)
};

struct SuperButtonInput {
public:
    ScnObject *sender;                             // The button that owns this slot; claimed the first time that sender sends MSG_SWITCH_ON/OFF (insert a
    s32 value;                                     // Last value that sender reported: 1 from MSG_SWITCH_ON 0x1f, 0 from MSG_SWITCH_OFF 0x20 (stored at 0x
};

class SuperButton : public ScnLogic {
public:
    virtual void PostLoadInit();                   // SuperButton_PostLoadInit (override)
    virtual void Update();                         // SuperButton_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SuperButton_HandleMessage (override)
    virtual void Reset();                          // SuperButton_Reset (override)
    s32 AreInputsOn();
    s32 GetInputValue(ScnObject *, s32 *);
    s32 SetInputValue(ScnObject *, s32 *);
    void ClearInputs();
    void SetDisabled(u8);
    u8 disabled;                                   // Non-zero makes Update return immediately. Sole writer is SuperButton_SetDisabled, whose only two cal
    SuperButtonInput inputs[2];                    // The two buttons registered with this gate, addressed as [reg + 8*i + 0x44] / [reg + 8*i + 0x48] with
    ScnObject *outTarget;                          // PROPERTY_SUPERBUTTON_OUT (prop 4) via Scn_GetPropObject; receives MSG_SWITCH_ON 0x1f while the gate
    ScnObject *notTarget;                          // PROPERTY_SUPERBUTTON_NOT (prop 0): the inverted output, getting MSG_SWITCH_OFF while the gate is on
    s32 latched;                                   // Set to 1 on every frame the gate is on and cleared only by PostLoadInit. Reset
    s32 state;                                     // 1 = gate on.
};

class SwirlSign : public ScnBody {
public:
    virtual void PostLoadInit();                   // SwirlSign_Init (override)
    virtual void Update();                         // SwirlSign_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // SwirlSign_HandleMessage (override)
    virtual void Reset();                          // SwirlSign_Reset (override)
    void SetState(u8);
    u8 state;                                      // 0 DUCK SEASON at rest (anim 2), 1 RABBIT SEASON at rest (anim 1), 2 flipping 0->1 (anim 3), 3 flippi
};

class Telescope : public ScnLogic {
public:
    virtual void PostLoadInit();                   // Telescope_Init (override)
    virtual void Update();                         // Telescope_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Telescope_HandleMessage (override)
    virtual void Reset();                          // Telescope_Reset (override)
    s32 UpdateView(Pad *);
    u8 state;                                      // TelescopeState: 0 viewing, 1 just started (mask drawn, input next frame), 2 idle
    u16 focal;                                     // camera focal passed to Camera_StartScripted = focal[mode]
    s16 basePitch;                                 // always 0 (msg 1)
    s16 baseYaw;                                   // 0x800 - rot.y (normal) or -rot.y (reverse)
    s16 baseRoll;                                  // always 0
    Vec3s eye;                                     // camera eye = pos + (0,-0xB4,0) + camDist along the view direction
    u8 _pad050[0x2];
    s16 yawOffset;                                 // current horizontal offset; starts at initHorAngle; clamped to Â±maxHorAngle (may overshoot by one ste
    s16 pitchOffset;                               // current vertical offset; starts at Â±initVertAngle (negated in reverse); clamped to Â±maxVertAngle
    s16 initVertAngle;                             // INITVERTANGLE * 11
    s16 initHorAngle;                              // INITHORANGLE * 11
    s16 maxVertAngle;                              // MAXVERTANGLE * 11
    s16 maxHorAngle;                               // MAXHORANGLE * 11
    u16 focalByMode[2];                            // [0] NORMALFOCAL, [1] REVERSEFOCAL
    u16 camDistByMode[2];                          // [0] CAMDISTNORMAL, [1] CAMDISTREVERSE; eye distance along the view direction
    u8 mode;                                       // 0 normal, 1 reverse; chosen by msg 2 from the box that holds the Wolf; also passed to Hud_DrawTelesc
    Box *normalBox;                                // NORMALBOXID (Scn_GetPropBox)
    Box *reverseBox;                               // REVERSEBOXID (Scn_GetPropBox); tested before normalBox
    s32 wolfFrozen;                                // Wolf msg 0xE reply at start; msg 0xF sent on exit; cleared when another object takes the freeze (msg
    s32 yawRemainder = 0;
    s32 pitchRemainder = 0;
};

struct TexScroll {
public:
    s32 scale;                                     // the page's size over the disc's: 1, or 2, 4... for a texture override
    s16 x;                                         // rect left in the texture page
    s16 y;                                         // rect top
    s16 w;                                         // rect width in texels
    s16 h;                                         // rect height
    s16 srcX;                                      // 0: backup origin x
    s16 srcY;                                      // 0: first backup row copied
    s16 srcW;                                      // = w; texels copied per row
    s16 srcH;                                      // = h; row count
    s16 offset;                                    // current scroll offset in rows, (offset+step*scale+H)%H each update
    s8 step;                                       // +1, or -1 when the list's reverse bit is set
    u16 texPage;                                   // index into g_pPolyBatcher->+0x14 texture table
};

class TextResBank {
public:
#ifdef SDW_MEMBERS_TextResBank
    SDW_MEMBERS_TextResBank
#endif
    virtual ~TextResBank();                           // TextResBank_DeletingDtor
    u16 Load(const char *path);
    char *LoadString(u16 group, u16 langMask, u16 index);
    u8 loaded;                                     // must be 1 or TextRes_LoadString returns NULL without searching
    u16 count;                                     // number of entries in the table
    TextResEntry *entries;                         // array of 0xC-byte entries {u32 key; u32 len; char *data} where key = (group << 24) + (sub << 8) + id
};

struct TextResEntry {
public:
    u32 key;                                       // group<<24 | index<<8 | languageMask (single AppLanguageMask bit)
    u32 len;
    char *text;                                    // operator-new copy of the string made by TextResBank_Load (NULL when len == 0)
};

class Texture {
public:
#ifdef SDW_MEMBERS_Texture
    SDW_MEMBERS_Texture
#endif
    virtual ~Texture();                               // Texture_ScalarDeletingDtor
    void Surface_Unlock();
    u32 GetFormat();                                             /* returns format (+0x0c) */
    u32 GetHeight();                                             /* returns height (+0x08) */
    SdwTexture *GetSurface();                                    /* returns surface (+0x10) */
    u32 GetWidth();                                              /* returns width (+0x04) */
    u32 width;                                     // surface width in texels ( : = desc.dwWidth, rounded up to a power of two when the device requ
    u32 height;                                    // surface height ( : = desc.dwHeight); the modulus of the TexScroll offset
    u32 format;                                    // pixel-format index passed to the constructor (its 4th argument, stored via, the
    SdwTexture *surface;                           // the DirectDraw surface: Lock +0x64 / Unlock +0x80 / BltFast +0x1c / Release +0x8 in the destructor
    SdwDdsd2Bytes desc;           // DDSURFACEDESC2 the constructor fills (dwSize 0x7c) and creates the surface from
};

class TimeKeeper : public ScnBody {
public:
    virtual void PostLoadInit();                   // TimeKeeper_Init (override)
    virtual void Update();                         // TimeKeeper_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TimeKeeper_HandleMessage (override)
    virtual void Reset();                          // TimeKeeper_Reset (override)
    void SetState(u8 value);
    u8 state;                                      // TimeKeeperState: 0 available, 1 ringing sequence, 2 collected
    u8 seqStep;                                    // ring-sequence step (0 anim1, 1 anim2 + loop sound, 2 anim3, 3 -> collected)
    u16 ringSoundHandle;                           // handle of the looping ring sound 0xF8, stopped at step 2
};

class TimeMachineChrono : public ScnMobile {
public:
    virtual void PostLoadInit();                   // TimeMachineChrono_Init (override)
    virtual void Update();                         // TimeMachineChrono_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TimeMachineChrono_HandleMessage (override)
    virtual void Reset();                          // TimeMachineChrono_Reset (override)
    void SetHeld(u8);
    u8 held;                                       // 1 while attached to the Wolf (msg 4), 0 otherwise (TimeMachineChrono_SetHeld). msg 0x15 starts time
    Vec3s homePos;                                 // Init pos (ground-snapped) or pos at msg 9 arg 1; Reset moves the chrono back here.
    TimeMachineSphere *sphere;                     // FindByClass(0x56); receives msg 0x2f00 on use. NULL when not exactly one exists; msg 0x15 does not c
    u8 _pad088[0x4];
};

struct TimeMachinePair {
public:
    ScnObject *past;                               // past-era copy (OBJECTnnPAST); uninitialised when the present slot is empty
    ScnObject *present;                            // present-era copy (OBJECTnnPRESENT)
};

struct TimeTravelArg {
public:
    ScnObject *obj;                                // partner (msgs 0x3B/0x3C) or the queried object (msg 0x3281)
    Vec3s pos;                                     // position mapped into the receiver's era (0x3B/0x3C) or the arrival position written by a Seed (0x328
    u16 padA;                                      // padding (stride 0xC)
};

class TimeMachineSphere : public ScnBody {
public:
    virtual void PostLoadInit();                   // TimeMachineSphere_Init (override)
    virtual void Update();                         // TimeMachineSphere_Update (override)
    virtual void Render(Camera *view);             // TimeMachineSphere_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TimeMachineSphere_HandleMessage (override)
    virtual void Reset();                          // TimeMachineSphere_Reset (override)
    void SetState(u8);
    void SwapToPast();
    void SwapToPresent();
    TimeMachinePair pairs[16];                     // OBJECTnnPAST / OBJECTnnPRESENT (prop offsets from table); used only when both are non-NULL
    u8 state;                                      // TimeMachineSphereState 0..8
    u8 seedCount;                                  // number of Seed (class 93) objects found, max 5
    ScnObject *chrono;                             // TimeMachineChrono (class 85) from FindByClass; written by Init, not read by the sphere code
    u8 _pad0ec[0x4];
    ScnObject *seeds[5];                           // Seed objects asked with msg 0x3281 for a return-arrival position
    s32 followerWaitMs;                            // state-7 follower wait: 3000, decremented by g_dtMs; at <= 0 the followers get msg 0x75
    u8 _pad108[0x104];
    s32 wolfFrozen;                                // 1 if the Wolf accepted msg 0xE in state 1/7; msg 0xF sent at the swap; cleared by msg 0xE to the sph
    s32 wolfInsideSinceArrival;                    // Set to 1 by the present->past swap. State 3 then opens the return path (5/6) instead of closing (4/0
    s32 dragonFollows;                             // Dragon reply to msg 0x38 (nonzero = will come along)
    ScnObject *dragon;                             // Dragon (class 84)
    s32 gossamerFollows;                           // Gossamer_Lev08 reply to msg 0x38
    ScnObject *gossamer;                           // Gossamer_Lev08 (class 87)
    ScnObject *bull;                               // bull (class 43); gets msg 0x38 (return home) when travel starts
    ScnBody billboard;                             // embedded ScnBody (ctor sets the ScnObject/ScnBody vtables at +0x228) built from export 0x7D WAR_IDO_
    ScnRecordSynth billboardRecord;                // synthetic WAR record written by Scn_BuildRecordFromExport for the billboard
    u8 _pad29e[0x2];
    u16 humSound;                                  // Sound 0xCD looping in state 6; stopped when state 7 starts and in Reset
    u16 openSound;                                 // Sound 0x13E started in state 2; stopped at the end of state 4/5 and in Reset
    TimeTravelArg wolfArrivalQuery;                // msg 0x3281 arg for the Wolf {g_pWolf; pos filled by a Seed}
    TimeTravelArg gossamerArrivalQuery;            // msg 0x3281 arg for the Gossamer {gossamer; its pos, overwritten by a Seed} (the sphere writes only .
    s32 releaseCamNextFrame;                       // set after an era swap; the next Update calls Camera_ReleaseScripted (a one-frame camera cut)
    s32 useDefaultArrival;                         // scratch: 1 until a Seed answers msg 0x3281 for the Gossamer
};

class Timer {
public:
#ifdef SDW_MEMBERS_Timer
    SDW_MEMBERS_Timer
#endif
    virtual ~Timer();                                 // Timer_ScalarDeletingDtor
    void Start();
    void Stop();
    void OffsetBase(double ticks);
    double GetElapsed(int unit);                                 /* samples */
    double GetDelta(int unit);                                   /* samples */
    double PeekElapsed(int unit);                                /* no sample */
    double PeekDelta(int unit);                                  /* no sample */
    s64 ReadTSC();                                               /* cpuid; rdtsc */
    double Convert(s64 ticks, int unit);
#ifdef __GNUC__
    u8 _vptrPad[0x4];
#endif
    u8 running;                                    // 1 between Timer_Start and Timer_Stop; GetDelta returns 0.0 and GetElapsed skips the TSC sample when
    u8 _pad009[0x3];
    u8 pad_0x0c[4];                                // padding that aligns the three LONGLONGs to 8 bytes
    s64 baseTsc;                                   // TSC value captured by Timer_Start; Timer_OffsetBase shifts it to compensate for paused intervals
    s64 elapsedTsc;                                // ticks from baseTsc to the last sample; returned by Timer_GetElapsed
    s64 deltaTsc;                                  // elapsedTsc minus the previous elapsedTsc, i.e. the ticks since the previous sample; returned by Time
};

class Torch : public ScnBody {
public:
    virtual void PostLoadInit();                   // Torch_PostLoadInit (override)
    virtual void Update();                         // Torch_Update (override)
    virtual void Render(Camera *view);             // Torch_Render (override)
    ScnBody flame;                                 // Embedded second ScnBody: the flame sprite. Torch_Create writes at this+0x64 and ne
    ScnRecordSynth flameRecord;                    // The synthetic 0x14-byte record the Torch builds for its flame: modelResIndex = WAR_IDO_ATORCH02 (221
};

class TrafficJams : public ScnBody {
public:
    virtual void PostLoadInit();                   // TrafficJams_Init (override)
    virtual void Update();                         // TrafficJams_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TrafficJams_HandleMessage (override)
    virtual void Reset();                          // TrafficJams_Reset (override)
    void SetPhase(u8 value);
    void SetState(u8 value);
    u8 phase;                                      // TrafficJamsPhase: 0 the train is on this station's route (timers run), 1 elsewhere (timers held full
    u8 state;                                      // TrafficJamsState: 0 clear (anim 0), 1 jammed (anim 1; the train waits BREAKTIME)
    s32 switchLatch;                               // msg 0x1F toggles the state only while 0; msg 0x20 clears it
    s32 trainAtStation;                            // cached train msg 0x4784 reply
    s32 wolfInBox;                                 // Wolf inside BOXSTATION (x/z); edges send Chronometer msg 0x4703
    s32 breakTimeMs;                               // BREAKTIME
    s32 arrivingTimeMs;                            // ARRIVINGTIME
    s32 lastSegTimeMs;                             // station msg 0x4801 reply (fetched lazily while 0)
    s32 breakRemainingMs;                          // break countdown (g_dtMs); msg 0x4683
    s32 arrivingRemainingMs;                       // arrival countdown; msg 0x4684
    s32 lastSegRemainingMs;                        // last-segment countdown; msg 0x4685
    s32 etaMs;                                     // estimated time until the train reaches this station, shown on the Chronometer (msg 0x4702)
    ScnObject *chronometer;                        // Chronometer (class 134) HUD clock
    ScnObject *station;                            // TRAINSTATION
    ScnObject *train;                              // Train (class 135)
    ScnObject *ring[4];                            // {self, next (NEXTJAMS), prev2 (msg 0x4682 arg), prev (msg 0x4682 sender)}: ring order in a 4-station
    Box *stationBox;                               // BOXSTATION
    u8 _pad0b0[0x20];
};

class TrainCarBody : public ScnBody {
public:
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TrainCarBody_HandleMessage (override)
};

struct TrainCar {
public:
    u16 exportId;                                  // WAR export the car is built from: 0xa8 ALOCOM1B, 0xb4 AWAGON2A, 0xb5 AWAGON2B, 0xb3 AWAGON1B, 0xb2 A
    u32 flags;                                     // 1 = the export existed and the body was built and added to the world; 4 = loads from DOCKA, 8 = from
    TrainCarBody body;                             // the car's own scenaric body, vtable: a ScnBody with only HandleMessage (+0x10) overridden,
    ScnRecordSynth record;                         // the synthesised WAR record Scn_BuildRecordFromExport writes and the body is initialised from
};

struct TrainWaypoint {
public:
    Vec3s pos;                                     // the point to drive to
    s16 heading;                                   // facing there (4096 per turn)
};

class Train : public ScnBody {
public:
#ifdef SDW_MEMBERS_Train
    SDW_MEMBERS_Train
#endif
    virtual void PostLoadInit();                   // Train_PostLoadInit (override)
    virtual void Update();                         // Train_Update (override)
    virtual void Render(Camera *view);             // Train_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Train_HandleMessage (override)
    virtual void Reset();                          // Train_Reset (override)
    void PlaceCars();
    void SetState(u8 newState);
    u8 state;                                      // TrainState: 0 TRAIN_ST_HIDDEN (out of play, waiting for a station to start its route), 1 TRAIN_ST_AT
    ScnObject *station;                            // the TrainStation (class 136) whose route the train is on: sent msg 0x4800 in state 0, msg 0x60 when
    TrainWaypoint waypoint;                        // the waypoint the train is driving to, copied from the station's msg 0x4781/0x4782 argument (mov dwor
    s32 placeRiders;                               // 1 = the next Train_PlaceCars must also drop whatever stands in the two dock boxes onto the cars; Pla
    s32 command;                                   // the last station message id the train received: 0x4781 start, 0x4782 final waypoint, 0x60 go (mov [e
    s32 runTicks;                                  // g_dt accumulated while state == 2; cleared when SetState(2) is entered.
    ScnObject *rider;                              // the object riding the train, found by Train_PlaceCars as the class-0 object above a car that answers
    s32 riderAttached;
    s32 blackout;                                  // 1 = hold the screen fully black (Fade_DrawOverlay(0,0x1f,0)); set when the train passes
    ZoneList offTrainZones;                        // OFFTRAINBOX (prop 0x10) boxes: the Wolf inside them is off the train - Scn_FindIdList
    ZoneList safeRecallZones;                      // SAFERECALBOX (prop 0x14) boxes: where a crushed object is put back
    CollBox hullBox;                               // the loco's first solid box in world space, rebuilt every update from the leading car for the current
    CollBox wolfBox;                               // hullBox copied and grown by 0x15e/0x160: the box the Wolf must be inside to keep
    Vec3s vel;                                     // this frame's movement, speed * (target - pos) / distance scaled by dt (Vec3s_ScaleByDt out at 0x4ffa
    u16 pad0c2;                                    // alignment before the sound handle
    u16 soundHandle;                               // Sound_Play handle: 0xe3 when the train parks inside an OFFTRAINBOX, 0xe6 while running ov
    u16 pad0c6;                                    // alignment before the speed
    s32 speed;
    s32 hornLatched;                               // 1 between msg 0x1f and msg 0x20 (the go button): the 0x1f handler only forwards msg 0x60 to the stat
    ScnObject *firstStation;                       // FIRST (prop 8) - Scn_GetPropObject, kept only when its classId is 136
    CollBox *docks[2];                             // DOCKA (prop 0) and DOCKB (prop 4): the two boxes whose contents are loaded onto a car. Indexed by th
    CollBox *jumpInSheepBox;                       // JUMPINSHEEPBOX (prop 0xc): a sheep (class 0xb) crushed by a car inside it is told msg 0x987 instead
    TrainCar cars[5];                              // 5 x 0x80 TrainCar slots: {u16 exportId; u32 flags; ScnBody body; u16 record[10]}. PostLoadInit fills
    EmitterDriftParams smokeParams;                // the chimney smoke: 60/-100 units per second, life 0x2000, one particle every 0x2000/0x18 ticks, size
    TrailEmitter smoke;                            // 16-particle chimney trail, driven every update with facing + 0x800 (Emitter_UpdateDrift)
    Vec3s smokePos;                                // the chimney mouth: pos, 500 above, then 0x46 units forward along the facing (sin/cos tables at 0x4fe
    u16 pad4e2;                                    // alignment before the sweep box
    CollBox sweepBox;                              // the loco's solid box moved to pos + vel, handed to ObjGrid_QueryBoxOverlap to find what the train is
};

class TrainStation : public ScnLogic {
public:
    virtual void PostLoadInit();                   // TrainStation_Init (override)
    virtual void Update();                         // TrainStation_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TrainStation_HandleMessage (override)
    virtual void Reset();                          // TrainStation_Reset (override)
    void SetState(u8);
    u8 state;                                      // TrainStationState: 0 idle, 1 the train is on this station's route
    ScnObject *train;                              // sender of msg 0x4800 / 0x60 (Train, class 135)
    ScnObject *passenger;                          // train msg 0x4783 reply; the station camera looks at it
    u32 timerTicks;                                // g_dt accumulated since the last command
    u32 lastSegTimeMs;                             // distXZ(p[n-2],p[n-1])*1000/(SPEEDPERCENT*15); returned by msg 0x4801
    u32 prevSegTimeMs;                             // same for segment n-3..n-2
    u32 jamsArrivingTimeMs;                        // ARRIVINGTIME of the registered TrafficJams (msg 0x5F arg)
    s32 commandPending;                            // a waypoint command is waiting for its delay
    s32 skipWait;                                  // msg 0x60 arrived while a command was pending; the next jam query passes NULL (no wait)
    s32 camActive;                                 // station camera started; released when there is no passenger
    s32 jammed;                                    // TrafficJams msg 0x1E reply
    Vec3s waypoint;                                // target sent to the train (arg of 0x4781/0x60/0x4782 together with +0x72)
    s16 waypointHeading;                           // atan2 heading for the waypoint (+0x800 on non-final points)
    u16 waypointIndex;                             // current trajectory point
    ScnObject *trafficJams;                        // TrafficJams (class 133) registered by msg 0x5F
    Box *box;                                      // BOX; written by Init, no reader found in TrainStation or Train code
    Trajectory *traj;                              // TRAJ {u16 count; Vec3s pts[]}, >= 3 points, straightened in place by Init
    ScnObject *next;                               // NEXT station (class 136 only); handed to the train via msg 0x60's out-arg at route end
    s32 waitTicks;                                 // WAIT ms -> ticks; written by Init, no reader found
    CamSetup *camera;                              // CAM setup record {u16 focal; s16 rot[3]; Vec3s eye}
    u32 speedPercent;                              // SPEEDPERCENT; msg 0x4802 reply (the train sets speed -1500*pct/100)
};

struct Trajectory3 {
public:
    u16 count;                                     // number of points: 3 (the seesaw keeps only TRAJ exports whose count is 3, and writes 3 into its hop
    Vec3s pts[3];                                  // start, middle and end point; seesaw_LaunchObject fits its arc through the object's position, pts[1]
};

class Tree : public ScnLogic {
public:
    virtual void PostLoadInit();                   // Tree_Init (override)
    virtual void Update();                         // Tree_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Tree_HandleMessage (override)
    virtual void Reset();                          // Tree_Reset (override)
    void SetVisible(u8 value);
    u8 visible;                                    // 0 hidden and non-solid (flags 0x800|0x400), 1 shown. Gates the sidle prompt.
    Vec3s seedPos;                                 // Filled by the Seed's reply to msg 0x3280 during the time-machine swap, then offset by g_timeMachineO
    u32 visibleDirectly;                           // VISIBLEDIRECTLY property (0 in Lvl-09, 1 in Lvl-10).
};

class TreeSection : public ScnBody {
public:
    virtual void PostLoadInit();                   // TreeSection_PostLoadInit (override)
    virtual void Update();                         // TreeSection_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TreeSection_HandleMessage (override)
    void SetState(u8);
    u8 state;                                      // 0 standing (anim 0, looping), 1 toppling (anim 5, started by Elmer's msg 0x3181), 2 landed (model sw
    AltModel mainModel;                            // The object's own model descriptor, filled by ScnObject_InitWithAltModels as its outMain argument. Al
    AltModel altModel;                             // The single alternate model (GameRes id 0x80) resolved at construction; ScnBody_SwapModel switches th
};

class TriggedStone : public ScnMobile {
public:
    virtual void PostLoadInit();                   // TriggedStone_PostLoadInit (override)
    virtual void Update();                         // TriggedStone_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // TriggedStone_HandleMessage (override)
    virtual void Reset();                          // TriggedStone_Reset (override)
    void CrushVictims();
    u8 state;                                      // TriggedStone state: 1 resting on the cliff (anim 1), 0 triggered (waiting TIMER ms), 2 rolling (anim
    u32 timer;                                     // ms accumulated with g_dtMs in states 0 and 2, zeroed on each state change; compared unsigned (jbe 0x
    u32 triggerDelayMs;                            // PROPERTY TIMER (record +0x08): delay between the trigger and the roll
    u32 crushTimeMs;                               // Anim_GetDurationMs(anim 0, all tracks) * 11 / 16: how far into the roll the ston
    s32 triggered;                                 // set by msg 0x1F when it starts the fall from state 1, cleared by msg 0x20 and Reset: a second 0x1F c
    s32 wolfOnButton;                              // set when Ralph is in the msg 0x1F object list; gates the scripted camera; cleared by msg
    Box *crushZone;                                // PROPERTY CRASHINGZONE (Scn_GetPropBox +0x00): the box whose objects are crushed (ObjGrid_QueryBoxPoi
    CamSetup *camera;                              // PROPERTY IDCAMERA (Scn_GetPropCamera +0x04): camera setup {u16 focal; u16 rot[3]; Vec3s pos} for Cam
};

class Twig : public ScnBody {
public:
    virtual void PostLoadInit();                   // Twig_PostLoadInit (override)
    virtual void Update();                         // Twig_Update (override)
    virtual void Render(Camera *view);             // Twig_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Twig_HandleMessage (override)
    void SetState(u8);
    Vec3s home;                                    // Spawn position copied from ScnObject.pos at init; the twig is teleported back here each time it has
    Vec3s frameDelta;                              // Scratch per-frame translation: velocity scaled by g_dt (Vec3s_ScaleByDt writes through its SECOND ar
    Vec3s velocity;                                // Drift velocity in units/s, hard-coded to (0, 0, -500) at init; the facing angle is derived from it.
    u8 state;                                      // 0 = wait until off-screen then hide and teleport home, 1 = wait until off-screen then unhide, 2 = dr
    s32 disappearDistSq;
    u32 drawnLastFrame;                            // Copy of inst.flags bit 0 (drawn this render pass), latched by Twig_Render and cleared at the end of
};

struct UiCursorFrame {
public:
    u8 u;                                          // U origin of the frame bitmap in its texture page (UiFrame_LoadSkin writes [this+8*i+0x18];
    u8 v;                                          // V origin of the frame bitmap ( write, read)
    u8 wMinus1;
    u8 hMinus1;
    u8 _pad004[0x2];
    u16 texIndex;                                  // texture page of the frame bitmap ( write as word [this+8*i+0x1e]; UiCursor_Draw rea
};

class UiCursor {
public:
    void SetPos(s16 x, s16 y);
    void SetSlide(u8 t256);
    void Animate();
    void UiFrame_LoadSkin(s16 x, s16 y);
    void Draw(u32 *layer);
    u32 frameColor;                                // RGB tint of the animated frame quad; initialised to
    u32 baseColor;                                 // RGB tint of the static base quad; initialised to
    s16 x;                                         // cursor X in virtual HUD units
    s16 y;                                         // cursor Y, stored already biased by half the first frame's height
    s16 frameW;                                    // on-screen width of the animated frame quad, taken from the last loaded frame bitmap
    s16 frameH;                                    // on-screen height of the animated frame quad
    u8 baseU;                                      // U origin of the base bitmap (resource 0x27) in its texture page
    u8 baseV;                                      // V origin of the base bitmap
    u8 baseWMinus1;                                // base bitmap width - 1, used as both quad width and UV span
    u8 baseHMinus1;                                // base bitmap height - 1
    u8 _pad014[0x2];
    u16 baseTexIndex;                              // texture index of the base bitmap
    UiCursorFrame frames[4];                       // four animation frames from resources 0x28..0x2B, 8 bytes each: {u8 u; u8 v; u8 wMinus1; u8 hMinus1;
    u8 frameIndex;                                 // current animation frame 0..3, advanced every 100 ms by UiCursor_Animate
    u8 slideX;                                     // horizontal offset added to the base quad's X, computed as >> 8
};

struct UiFrame {
public:
    s16 quads[4][8];                               // four border strips of four corners (x, y); written as packed dwords by Ui_BuildFrameQuads
    float uvs[4][8];                               // four strips x four (u, v) pairs, strip stride 0x20 (Ui_DrawFrameQuads)
    u16 cellSize;                                  // (bitmap width - 1) << 8 | (height - 1): written as one word, read as one word at 0x53d0b
    u16 texPage;                                   // texture page of the frame bitmap, passed as the page argument to Draw2D_TexRect
};

struct UmbrellaFlagBits {
public:
    u8 open : 1;                                   // (bits) bit view of Umbrella.umbrellaFlags (+0x83): read as a one-bit field (mov cl,[+0x83]; and cl,1; xor e
    u8 unused : 7;                                 // (bits) remaining bits
};

class Umbrella : public ScnMobile {
public:
    virtual void PostLoadInit();                   // Umbrella_Init (override)
    virtual void Update();                         // Umbrella_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Umbrella_HandleMessage (override)
    virtual void Reset();                          // Umbrella_Reset (override)
    Vec3s homePos;                                 // spawn position (Init; msg 9 arg 1); Reset returns here when IN_WORLD
    u8 state;                                      // UmbrellaState: 0 closed, 1 opening on the ground, 2 opening in the air, 3 open, 4 closing
    UmbrellaFlagBits umbrellaFlags;                // bit 0 = open (set when an opening anim ends, cleared by msg 0x16, Reset and Init); gates the msg 0x1
};

class Vdx7 {
public:
#ifdef SDW_MEMBERS_Vdx7
    SDW_MEMBERS_Vdx7
#endif
    virtual ~Vdx7();                                  // Vdx7_ScalarDeletingDtor
    u8 ok;                                         // 1 once the VDX7 magic matched and both tables were read (Vdx7_Open)
    u32 *entries;                                  // malloc'd array of u16 values widened to u32 (Vdx7_Open); free'd by Vdx7_Dtor. The
    Vdx7Record *records;                           // malloc'd array of 10-byte records (Vdx7_Open); free'd by Vdx7_Dtor. The decoders take &reco
};

struct Vdx7Record {
public:
    s16 x;                                         // U origin in texels of a texture rectangle: Bs_TexelToUV's origin argument for every U coordinate (Bs
    s16 w;
    s16 y;                                         // V origin in texels
    s16 h;                                         // V size in texels
    u16 page;                                      // texture page, zero-extended into the polygon's texIndex
};

struct Vec2s {
public:
    s16 x;
    s16 y;
};

struct Vec4i {
public:
    s32 x;
    s32 y;                                         // vertical points down
    s32 z;
    s32 pad;                                       // never written by the Mat34s_Transform* functions, copied out uninitialised
};

class Video {
public:
#ifdef SDW_MEMBERS_Video
    SDW_MEMBERS_Video
#endif
    virtual ~Video();                                 // Video_ScalarDeletingDtor
    s32 CloseFile();
    s32 ConnectAudioStream(s32);
    s32 CreateStream(IDirectDraw7 *);
    s32 OpenFile(const char *);
    s32 ReleaseFilter(s32);
    s32 RunLoop();
    RECT sourceRect;                               // GetSurface output; passed to the frame callback
    IGraphBuilder *graph;                          // OpenFile; RemoveFilter through it
    IAMMultiMediaStream *mediaStream;              // CreateStream: Initialize / AddMediaStream
    IUnknown *unknownStream;                       // only null tests and Release established
    IDirectDrawStreamSample *sample;               // CreateSample output; GetSurface and Update
    IDirectDrawSurface *surface;                   // GetSurface output, the legacy IDirectDrawSurface as in the SDK signature; passed to the c
    VideoFrameProc frameCallback;                  // VideoFrameProc s32 (__cdecl *)(IDirectDrawSurface *, RECT *): set by the constructor and
    u8 unknown2c[8];                               // no typed use established
    IBaseFilter *audioFilter;                      // CoCreateInstance, AddFilter, released by ReleaseFilter
};

class VideoPlayer {
public:
#ifdef SDW_MEMBERS_VideoPlayer
    SDW_MEMBERS_VideoPlayer
#endif
    virtual ~VideoPlayer();                           // VideoPlayer_ScalarDeletingDtor
    u8 Init(D3DApp *);
    u8 PlayFile(const char *, u8);
    u8 ready;                                      // set by the constructor; the boolean result of Init
    D3DApp *app;                                   // stored by Init, cleared by the destructor
    u8 unknown0c[256];                             // never accessed; not a proven string
    Video *stream;                                 // the stream object: allocated and constructed in Init, deleted through its vtab
};

class VisibilityManager : public ScnLogic {
public:
    virtual void PostLoadInit();                   // VisibilityManager_Init (override)
    virtual void Update();                         // VisibilityManager_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // VisibilityManager_HandleMessage (override)
    void CollectMeshes(s16);
    void SetMeshesVisible(s32);
    ZoneList whenInBox1;                           // zone list {boxes, count}: WHENINBOX list; count u16 at +0x44 | count (+4): count of whenInBox1
    ZoneList whenInBox2;                           // zone list {boxes, count}: WHENINBOX2 list | count (+4): count
    ZoneList whenInBox3;                           // zone list {boxes, count}: WHENINBOX3 list | count (+4): count
    ZoneList whenInBox4;                           // zone list {boxes, count}: WHENINBOX4 list | count (+4): count
    WorldObj *meshes[100];                         // g_worldObjs entries whose resource is in CANTSEE1..3
    u8 savedKinds[100];                            // each mesh's WorldObj.inst_kind at init, restored when shown
    s16 meshCount;                                 // number collected (cap 100)
    u8 state;                                      // VisMgrState: 0 initial, 1 camera in a box (hidden), 2 shown
};

class Volcano : public ScnBody {
public:
    virtual void PostLoadInit();                   // Volcano_Init (override)
    virtual void Update();                         // Volcano_Update (override)
    virtual void Render(Camera *view);             // Volcano_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Volcano_HandleMessage (override)
    void SetState(u8);
    s32 timerMs;                                   // dormant countdown (g_dtMs), or the vertical-mode anim timer
    s16 arcIndex;                                  // current arc sample = elapsed g_gameTimeMs / 25
    u32 lavaFxActive;                              // lavaBody animating and drawn; enables the 60-unit burn test
    u32 headVisible;                               // draw the volcano's own body (the bomb)
    u32 landed;                                    // bomb has splashed; stops the head collision sweep
    u32 vertical;                                  // PROPERTY_VOLCANO_VERTICAL (0 in every shipped placement)
    u32 uTurn;                                     // PROPERTY_VOLCANO_UTURN (0 in every shipped placement)
    u32 reverseNext;                               // with UTURN: the next eruption runs backward (state 2) after 500 ms
    u32 splashDone;                                // splash played for this eruption
    s16 trailEndIndex;                             // last arc index the trailing blobs may use; truncated to the impact index
    u32 startTimeMs;                               // g_gameTimeMs when the eruption started
    u32 blobActive[5];                             // per trailing blob: visible this frame
    u8 state;                                      // VolcanoState
    Vec3s homePos;                                 // crater position (pos at Init)
    Vec3s blobVel[5];                              // blobDelta*50; |y| sets the blob stretch in Render
    Vec3s arcPoints[128];                          // TRAJ1 arc pre-sampled at Init (38 used)
    Vec3s blobDelta[5];                            // per-blob step between consecutive arc samples; orients the blob
    u16 loopIndex;                                 // member used as a loop counter by Init, Update and Render
    u16 lastArcIndex;                              // number of sampled points - 2
    LaunchArc arc;                                 // quadratic through the 3 TRAJ1 points, 3-D mode (flags 6); used only at Init
    CollBox hitBox;                                // flags 1; crater box +-15 (a Rock overlapping it plugs the volcano), or the lava column in vertical m
    u32 hitPending;                                // head sweep hit a non-Wolf; splash on the next frame
    ScnBody lavaBody;                              // embedded ScnBody from WAR_IDO_ALAVE02 (fountain anim 1 / splash anim 2 / column anim 0); pos at +0x4
    ScnBody blobs[5];                              // 5 embedded ScnBody from WAR_IDO_ALAVE03, stride 0x64, trailing the bomb 2 samples apart
    ScnRecordSynth lavaRecord;                     // record synthesised for ALAVE02
    u8 _pad6a2[0x2];
    ScnRecordSynth blobRecord;                     // record synthesised for ALAVE03
    void *lavaModelRes;                            // Scn_BuildRecordFromExport result for ALAVE02; Render draws lavaBody only when it is set
    void *blobModelRes;                            // Scn_BuildRecordFromExport result for ALAVE03 (0 in vertical mode)
};

struct WarHeader {
public:
    u8 _pad000[0x4];
    char version[4];                               // 'V2.6'. Load_WAR prefix-compares it with sprintf('V%u.%u', 2, 6), temporarily NUL-ing +8. The same m
    u8 clearR;                                     // red of the level clear/fog colour; Load_DAVnWAR expands the whole RGB by 1.03
    u8 clearG;                                     // green of the clear/fog colour
    u8 clearB;                                     // blue of the clear/fog colour
    u32 resourceCount;                             // number of u32 entries in the table at +0x10; the loop bound of Load_WAR, Load_WarMeshes and every Lo
};

struct WarLevelHeader {
public:
    u16 texScrollListIds[4];                       // = g_texScrollListIds: four id-list ids of scrolling-texture rects (TexScroll_Init)
    u16 texScrollReverseMask;                      // = g_texScrollReverseMask: bit i reverses list i (read as bitfields by TexScroll_Init)
    u16 weatherType;                               // = g_weatherType: 1 rain, 2 snow (Load_DAVnWAR)
};

class Watch : public ScnBody {
public:
    virtual void PostLoadInit();                   // Watch_PostLoadInit (override)
    virtual void Update();                         // Watch_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Watch_HandleMessage (override)
    virtual void Reset();                          // Watch_Reset (override)
    void SetState(u8 newState);
    u8 state;                                      // 0 hidden (update mode 2 = never updated, anim 0, stopped), 1 shown (update mode 1 = always, anim 1 l
    u8 _pad065[0x3];
    u8 hudPrimBuf[2][400];                         // two 0x190-byte blocks handed alternately (by hudBufToggle) to ScnBody_RenderEx as its third argument
    u8 hudBufToggle;                               // picks hudPrimBuf[0] (nonzero) or [1] (0), then flipped with ! every drawn update
    Camera hudCam;                                 // the watch's own camera for the HUD draw: rot (0,0,0), dist 450, pos 450 back from the origin along r
    AnimSprite digits;                             // the ten digit frames of sprite resource 0x82 (AnimSprite_InitFromRes); the two digits of th
    s32 timeLeft;                                  // time left in g_dt ticks (4096 per second): set from the arg of msg 0x3781, -= g_dt per drawn update
    u32 running;                                   // the arg of msg 0x3782; while set the watch counts down, draws itself and the digits and sets the tic
    u16 tickSound;                                 // channel of the ticking loop, sound 0x3f started by msg 0x3782 arg 1 (flags 9, volume 0;) an
};

class WaterGeyser : public ScnBody {
public:
    virtual void PostLoadInit();                   // WaterGeyser_PostLoadInit (override)
    virtual void Update();                         // WaterGeyser_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // WaterGeyser_HandleMessage (override)
    virtual void Reset();                          // WaterGeyser_Reset (override)
    void SetState(u8);
    u8 state;                                      // 0 waiting for a swimmer, 1 swallow delay, 2 repositioning under the victim, 3 geyser-in animation ru
    s32 timeInHole;                                // PROPERTY_WATERGEYSER_TIMEINHOLE (prop 8). Stored by PostLoadInit and never read - a slic
    s32 animTimerMs;                               // How long the passenger's geyser-in animation lasts: the return of passenger->HandleMessage(this, 0x3
    s32 swallowTimerMs;                            // State-1 countdown, always initialised to 0x5dc = 1500 ms by SetState(1) and decremented by g_dtMs.
    ScnObject *geyserOut;                          // PROPERTY_WATERGEYSER_GEYSEROUT (prop 4) via Scn_GetPropObject: the GeyserOut this whirlpool delivers
    ScnObject *sheep;                              // Cached g_pSheepOutOfZone while testing whether a loose sheep is in the water (msg 0x984) an
    ScnObject *passenger;                          // The object being swallowed - Ralph or the out-of-zone Sheep. Its position drives the reposition in s
    ZoneList detectBoxes;                          // zone list {boxes, count}: PROPERTY_WATERGEYSER_BOXDECTECT (prop 0) resolved by Scn_FindIdList: the l
    Box *hitBox;                                   // The detect box the victim was found inside; its min on the vertical (+6, the surface on the down-pos
    u8 _pad08c[0x4];
    u16 soundHandle;                               // Handle of sound 0x131, started when the geyser-in sequence begins (state 3, volume 0xff) and stopped
};

struct WaterMineFlagBits {
public:
    u8 detonated : 1;                              // (bits) bit view of WaterMine.flags (+0xe7): read, s
    u8 unused : 7;                                 // (bits) remaining bits
};

class WaterMine : public ScnMobile {
public:
    virtual void PostLoadInit();                   // WaterMine_PostLoadInit (override)
    virtual void Update();                         // WaterMine_Update (override)
    virtual void Reset();                          // WaterMine_Reset (override)
    s32 KillOverlapping();
    void SetIdleModel();
    AltModel modelIdle;                            // Main model slot, the outMain argument of ScnObject_InitWithAltModels; selected by WaterMine_SetIdleM
    AltModel altModels[4];                         // Four alternates resolved from the id table = u16 {3, 0x5a, 0xdf, 0xe0}: [0] +0x8c shallo
    CollBox triggerBox;                            // 300-unit cube centred on the mine's LOAD position (min = pos-150 at +0xd0, max = pos+150 at +0xd6),
    Box *waterBox;                                 // The water zone containing the mine, from BoxList_FindContainingPoint over g_waterZones; its
    Vec3s homePos;                                 // Position captured in PostLoadInit as dword+word (0xe0..0xe5); restored by Reset through ScnMobile_Se
    u8 state;                                      // 0 armed, 1 blast animation playing, 2 spent. State 2 has no case body, so it is terminal until Reset
    WaterMineFlagBits flags;                       // Bit 0 = the mine has detonated and its visual state needs restoring; it is the only gate on Reset's
};

class WaveFile {
public:
#ifdef SDW_MEMBERS_WaveFile
    SDW_MEMBERS_WaveFile
#endif
    virtual ~WaveFile();                              // WaveFile_ScalarDeletingDtor
    s32 Open(char *name, u8 fromMemory, u32 size);               /* WaveFile_Open: fromMemory=1 opens `size` bytes at name */
    s32 ResetFile();
    s32 Read(u32 size, u8 *dest, u32 *read);
    s32 Close();
    u32 ReadAt(u32 offset, u8 *dest, u32 size);                  /* (port) the lip-sync meter's peek: no cursor moves */
    WAVEFORMATEX *format;                          // Heap-allocated format block filled by the header parser; free()d by the dtors. NULL means '
    u8 *samples;                                   // (port: was HMMIO hmmio) The whole 'data' chunk, read by SDL_LoadWAV_IO (Platform_WavLoad); NULL when closed.
    u32 sampleBytes;                               // (port) Size of `samples`.
    u32 readPos;                                   // (port) Offset in `samples` of the next byte Read copies (the mmio file position).
    MMCKINFO ckData;                               // The 'data' chunk descriptor (20 bytes, 0x0C..0x1F). WaveFile_ReadMmio decrements its cksize as a byt
    MMCKINFO ckRiff;                               // The RIFF/WAVE parent chunk descriptor (20 bytes, 0x20..0x33), filled by the header parser.
};

struct WeatherParticle {
public:
    Vec3f pos;                                     // world position, advanced by vel * g_dtMs / 1000 (Weather_Advance)
    Vec3f viewPos;                                 // last camera-space position (Weather_WrapToVolume); a particle that leaves the volume is mir
    Vec3f vel;                                     // velocity in units per second (SfxParticles_InitRandom)
    float alpha;                                   // 1.0 in the air; DrawRain / DrawSnow fade it below a water surface
    u8 inAir;                                      // 1 = drawn with the first texture (not under water), 0 = the second one (DrawRain
    u8 _pad29[3];                                  // padding to the 0x2c stride
};

struct WeatherTex {
public:

    u32 page;                                      // texture page of the bitmap record (DavBitmapRec.page); DrawRain/DrawSnow write page + 4 as
    union {
        u32 color;                                     // RGB of the drops, argument & 0xffffff
        u8 colorBytes[4];
    };
    float width;                                   // quad width in world units
    float height;                                  // quad height in world units
    float uv[8];                                   // four half-texel-corrected (u,v) corners from Tex_CornerUV: (0,0) (w,0) (0,h) (w,h) ( -0x52e47
};

class Weather {
public:
#ifdef SDW_MEMBERS_Weather
    SDW_MEMBERS_Weather
#endif
    virtual ~Weather();                               // Weather_ScalarDeletingDtor
    void SetVolume(float halfWidth, float radius, u32 count);
    u8 SetTexture(u16 resId, float width, float height, u32 color);
    u8 SetTexture2(u16 resId, float width, float height, u32 color);
    void SfxParticles_InitRandom(Vec3f *dir, float spread, float speedMin, float speedMax);
    void Advance(Mat44 *view);
    void DrawRain(Mat44 *view, float *cullNormal);
    void DrawSnow(Mat44 *view);
    u8 CreateVertexBuffers(u32 count);
    void WrapToVolume(Mat44 *view);
    u8 hasTex2;                                    // 1 once Weather_SetTexture2 has set the second texture; the ctor clears it
    WeatherTex tex;                                // first particle texture (Weather_SetTexture)
    WeatherTex tex2;                               // second particle texture (Weather_SetTexture2)
    Vec3f centre;                                  // camera-space centre of the particle volume: (0, 0, sqrt(radius^2 - halfWidth^2) + near plane) (Weath
    float radius;                                  // volume radius (Weather_SetVolume)
    float radiusSq;                                // radius squared
    WeatherParticle particles[500];                // the particles; count at +0x566c
    u32 count;                                     // live particles, capped at 500 by Weather_SetVolume
    SdwVertexBuffer *vb;                    // D3DFVF_XYZ vertex buffer, 4 vertices per particle (Weather_CreateVertexBuffers)
    SdwVertexBuffer *vbXf;                  // D3DFVF_XYZRHW ProcessVertices destination
    RenderPoly poly;                               // the one triangle every quad half is pushed through (RenderPoly_Ctor)
};

class Wheel : public ScnLogic {
public:
    virtual void PostLoadInit();                   // Wheel_PostLoadInit (override)
    virtual void Update();                         // Wheel_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Wheel_HandleMessage (override)
    virtual void Reset();                          // Wheel_Reset (override)
    void SetState(u8 newState);
    s32 ScanLift(u16 index);
    void UpdateSpin();
    s16 ComputeSpinStep();
    s32 MoveAlongRail(s32 limit, s8 dir);
    void PlaceLifts(s16 *rot);
    u16 soundHandle;                               // handle of the running / rolling sound (0x8d while a switch drives it, 0xf5 while it turns); 0 = none
    u8 state;                                      // 0 stopped, 1 switch on, 2 free to turn, 3 held by msg 0x43 arg 1, 4 switch off / returning (Wheel_Se
    s32 liftLoaded;                                // 1 while one of the four lifts overlaps the Wolf (Wheel_ScanLift via Wheel_UpdateSpin 0x5089
    s32 wolfAtHub;                                 // 1 while the Wolf overlaps the wheel's own model box, tested only once the glow is at least half up (
    u16 loadedLift;                                // index 0..3 of the lift that carries the Wolf
    Vec3s railStart;                               // the position the wheel returns to along the rail: the placed position, refreshed by state 0 and by m
    Vec3s homePos;                                 // placed position; Reset copies it back into railStart
    Vec3s railPos;                                 // the position reached along the rail, refreshed after every move
    Vec3s homeRot;                                 // placed rotation; Reset restores spin from it and re-places the lifts
    Vec3s spin;                                    // the wheel's working rotation, all three axes masked to 12 bits; +0x6a is the spin angle that Wheel_U
    Vec3s liftBoxSize;                             // (0, lift model box height, 0): added to each lift's world box in Wheel_PlaceLifts
    u16 armRadius;                                 // half the height of the wheel's own model box plus 30: the radius at which the lifts are placed (0x50
    s16 spinStep;                                  // angle added to the spin this frame, from Wheel_ComputeSpinStep; also sent to the MOBIL as m
    s16 switchAngle;                               // PROPERTY_WHEEL_SWITCHANGLE, moved one unit away from zero by PostLoadInit; read nowhere e
    s32 translation;                               // PROPERTY_WHEEL_TRANSLATION: the distance the wheel may travel along its rail
    s32 railTravel;                                // XZ distance from railStart to the current position (Vec3s_DistXZ); the move stops at `trans
    s32 glow;                                      // 0..1000 ramp driven by g_dtMs*1000 / (raisingTime/2): rises while the wheel is past half its travel,
    s32 railStartSet;                              // latched the first time state 0 records railStart so that a later stop does not move it
    u32 raisingTime;                               // PROPERTY_WHEEL_RAISINGTIME in ms: the time the rail move and the glow ramp take
    CollBox hubBox;                                // the wheel's own model box in world space, its top raised by 10, used for the hub overlap query (0x50
    ScnObject *overlap[64];                        // ObjGrid_QueryBoxOverlap output buffer shared by the hub query and the per-lift query ( calls
    s32 overlapCount;                              // number of objects the last overlap query returned
    WheelDummy *lifts[4];                          // PROPERTY_WHEEL_LIFT0..LIFT3 (Scn_GetPropObject). In Lvl-12 these are WheelDummy ob
    ScnObject *mobil;                              // PROPERTY_WHEEL_MOBIL: receives msg 0x42 with the frame's spin step while a lift is loaded
    ScnObject *switchSender;                       // first sender of MSG_SWITCH_ON / MSG_SWITCH_OFF; passed on to the boss in the msg 0x42 argument (0x50
    ScnObject *boss;                               // the level's Gossamer_Boss (class 113), found by scanning g_scnObjects; told when the whee
    u8 _pad1bc[0x4];
    ScnObject *bossMsgSender;                      // first word of the msg 0x42 argument sent to the boss: switchSender
    s32 bossMsgOn;                                 // second word: 1 when the wheel starts its rail move (state 2), 0 when it returns (state 4)
};

class WheelDummy : public ScnLogic {
public:
    virtual void PostLoadInit();                   // WheelDummy_PostLoadInit (override)
    virtual void Update();                         // WheelDummy_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // WheelDummy_HandleMessage (override)
    s32 AnswerGroundQuery(void *arg);                            /* WheelDummy_AnswerGroundQuery */
    CollBox box;                                   // world ground box: the parent Wheel copies a model box + its position into +0x44..+0x4f ( -0x5
    s16 railPhase;                                 // phase angle written by the parent Wheel and
    s16 spinPhase;
};

struct WolfLaunchAxis {
public:
    s32 p0;                                        // start coordinate
    s32 v;                                         // initial speed, times t >> 8
    s32 a;                                         // acceleration term, times t*t >> 15
};

class WolfLaunchPath {
public:
#ifdef SDW_MEMBERS_WolfLaunchPath
    SDW_MEMBERS_WolfLaunchPath
#endif
    WolfLaunchAxis x;                              // x axis of the launch parabola (Wolf+0x66c, set by msg 0xC)
    WolfLaunchAxis y;                              // vertical axis
    WolfLaunchAxis z;                              // z axis
    s32 t;                                         // trajectory parameter 0..0x100, += g_dtMs >> 2 per frame
    u8 unk28[16];                                  // the rest of the msg 0xC argument, copied with the path: Wolf_HandleMessage copies 14 dwords (0x38 by
};

class Wolf : public ScnControllable {
public:
#ifdef SDW_MEMBERS_Wolf
    SDW_MEMBERS_Wolf
#endif
#ifdef SDW_EXTRA_Wolf
    SDW_EXTRA_Wolf
#endif
    virtual void PostLoadInit();                   // Wolf_Init (override)
    virtual void Update();                         // Wolf_Update (override)
    virtual void Render(Camera *view);             // Wolf_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // Wolf_HandleMessage (override)
    virtual void Reset();                          // Wolf_Reset (override)
    virtual s16 GetStickHeading(s16 fallback);     // Wolf_GetStickHeading (override)
    void StateMachine(Pad *pad);
    void DropHeld(u32 flags);
    void EnterState(u8 state);
    void SetStateAnim(u8 state, u16 animId, s32 loop, s32 stopLoopSound);
    void SetState(u8 state);
    void SetStateKeepAnimStopSound(u8 state);
    void SetStateKeepSound(u8 state);
    void SetStateKeepAnim(u8 state);
    void SetMode(u8 newMode, u8 newState);
    void SetIdleState();
    void IdleAnimStep();
    void ResetState();
    void InitMoveConfig();
    void ClampDeltaToBox(Box *box, Vec3s *delta, u32 axes);
    void SetForwardVelocityMax(Vec3s *outVel, Vec3s *rot, const MoveRecord *rec);
    u32 LeapStep(s32 jumpHeight, s32 jumpTicks);
    s32 MoveStepDirected(s16 facingTarget, s16 headingTarget, s32 targetSpeed);
    void ApplyGravity(Vec3s *vel, s32 gravity, s32 terminal);
    u32 ApplyMove(Vec3s *step, Vec3s *rot, s16 collFlags, Vec3s *vel);
    s32 MoveStep(s32 faceMotion);
    u32 JumpAscendStep(s32 height, s32 ticks);
    u32 FallStep(s16 yLimit, s32 slowFall);
    s32 MoveTowardPoint(Vec3s *target, s16 facing, s32 useFacing);
    s32 MoveStepFixedFacing(s16 facing);
    s32 SidestepStep(s16 facing);
    s32 PushObjectStep(ScnObject *target);
    s32 RocketFlyStep(s32 boost, s32 launching);
    s32 ForcedHeadingGroundStep(s16 heading);
    void ClimbStep(Box *box, s32 isClimbZone);
    s32 ClimbBoxWalkStep();
    void SwimMove();
    void WaterFloatUpStep();
    s32 WaterSinkStep();
    s32 SwimStateStep();
    void UpdraftStep();
    void DeathRiseStep();
    u8 RunMove(s32 applySurface, s32 reflectOnCrash, u32 *outCollFlags);
    s32 LaunchArcStep();
    s32 CheckFallOrSlide(s32 onSlope);
    s32 State2B_Step();
    s32 MoveStepGrounded();
    s32 ForcedHeadingGroundStepChecked(s16 heading);
    s32 FaceTargetStep();
    s32 FaceTargetActionStep(s32 ctxType);
    s32 FaceStickStep();
    s32 GroundCommon();
    s32 SneakStateStep();
    s32 StrafeFixedFacingStep(s16 facing);
    s32 SidestepHeldStep(s16 facing);
    s32 BushCommon();
    s32 ModelSet3GroundStep();
    void GhostCostumeSetIdle();
    s32 GhostCostumeGroundStep();
    s32 StrafeAroundFocusStep();
    s32 SlowFallItemGroundStep();
    s32 SlowFallItemFallStep();
    s32 SlowFallItemAscendStep(s32 height, s32 duration, s32 isBoost);
    s32 FlattenedGroundStep();
    s32 JumpAscendStateStep(s16 height, s32 duration);
    s32 FallStepLandTo(u8 landState);
    s32 FallStateStep(s16 yLimit);
    s32 RunStep(Pad *pad);
    u32 MoveWithPlatform(Vec3s *delta, ContactInfo *contact, u16 mask, Vec3s *vel);
    void SetFrozenBy(ScnObject *sender);
    void Unfreeze();
    void PlayLoopSound(s16 sfxId);
    void StopLoopSound();
    void UpdateMoveLoopSound();
    void FootstepSound();
    void SneakStepSound();
    void RunSound(s32 isStart);
    void Msg74_Draw(DrawMsgArgs *args);
    s32 ShowProp(s32 propIndex, u16 animId, s32 loop);
    void MashCounterStep();
    s16 SnapHeadingToObject(ScnObject *obj, s16 heading);
    void SetScale(s16 newScale);
    void ScaleVec(Vec3s *v);
    void UpdateFallAnim();
    CollBox *FindClimbBoxOnTarget();
    void NotifyAttached_6A();
    void RestartNoAnim(s32 delayTicks);
    void Die(u8 deathState, u32 dropFlags, s32 extraDelay);
    void RunTapCheck(s32 playStartSound);
    s32 CanUseInventory();
    s32 CanControl();
    void FilterInput(Pad *pad, u32 inputMask);
    void ClearEffects();
    void UpdateTrailFx();
    void EquipItem(ScnObject *obj);
    s32 CanSwapItem(s32 needStateFlag);
    s32 SwapHeldItem(s32 animated, s32 needStateFlag);
    s32 QuickInventory(Pad *pad);
    void UpdateCamera(Pad *pad);
    void ScanContextActions();
    s32 IsActionAvailable(s32 onGround);
    s32 DoAction(s32 onGround);
    s32 CheckActionButton(s32 onGround);
    s32 TryStartPush(s32 enable);
    void CalcHeldObjWorldPos(const Vec3s *localOffset, Vec3s *outPos);
    s32 CanPutDownHeldObj(s32 atFeet, Vec3s *outPos);
    void Fx0_Start();
    void Fx0_Stop();
    void Fx2Body_StartAtSurface();
    void Fx2Body_Stop();
    void Fx3or4_Start(s32 variant);
    void Fx3or4_Stop();
    void Fx5_Start(s32 variant);
    void Fx5_Stop();
    void FxGeneric_Start(u8 propIndex, u8 animId);
    void FxGeneric_Stop();
    void Fx8_Start();
    void Fx8_Stop();
    void Fx6_Start(s32 variant);
    void Fx6_Stop();
    void HeldObj_SendMsg15();
    void HeldObj_SendMsg16();
    void SetActiveItem(ScnObject *item);
    void ClearActiveItem();
    void StoreActiveItemInInventory();
    s32 IsFalling();
    s32 CanJump();
    void DrawActionPrompt(ActionHit *, s32 heldAction, s32 viewPrompt, ActionHit *, s32);
    void Hud_DrawItemPrompt(u16 *, s32);
    void InitActionPromptStrings();
    ZoneList climbZones;                           // prop CLIMBBOXES list (record+0x14) {Box **boxes; u16 count}; ctx action 0xF/0x11. One field: Wolf_Cl
    TrailEmitter trailA;                           // 0x164-byte ribbon emitter (ptrs +0x128->+0x14C, +0x12C->+0x20C, count byte +0x149=16, flags byte +0x
    ParticleEmitter particles;                     // 0x2C-byte emitter (ctor), flags byte +0x2AE
    WallAvoid wallAvoid;
    ScnBody propBody;                              // embedded body showing one of 8 prop models (own pos +0x2C4, rot +0x2CC, model +0x2BC, anim +0x2F8, a
    u16 propRecord[10];                            // fake level record for propBody; u16 words like every WAR record
    AltModel propModels[9];                        // alt-model table for propBody (ids g_wolfPropModelIds)
    ScnBody splashBody;                            // water-entry splash body (class 0x5A record), pos +0x3CC, model +0x3C4, anim +0x400, anim flags +0x42
    u16 splashRecord[10];                          // fake record for splashBody; u16 words like every WAR record
    Box *waterZone;                                // current water box (flags: 0x1000000 freezing, 0x2000000 sink-through); a Box: flags +0, min.y (the w
    TrailEmitter trailB;                           // second ribbon emitter (flags +0x45E)
    AltModel modelSets[6];                         // 0x10 each; index = modelSet; entry 4 id at +0x5E0
    s32 stateTime;                                 // ticks since state entry
    s32 footstepTimer;                             // ticks since last footstep sound (cap 0x1E000); step when >= animDuration/2
    s32 runTapTimer;                               // run tap timer; reused as mash window (0x4f) and flatten timer (0xd5)
    s32 landDustTimer;                             // min(airTime>>3,0x400) at landing; counts down while landing dust trail plays
    s32 animDurationTicks;                         // current anim length in ticks
    u32 lastObjectContactTime;                     // g_gameTime when the mover last touched a scenaric object (contact[0]); FB 0x20 for 0x400 ticks
    u32 lastContact2Time;                          // g_gameTime when contact[2] was non-zero; FB 0x10 for 0x400 ticks (meaning of contact[2] not decoded)
    s32 flattenDuration;                           // ticks Ralph stays flattened (arg of msg 0x41b)
    u16 stepSoundCount;                            // sneak step counter (pitch/volume ramp, max 8) / run-start sound played
    u16 loopSfxHandle;                             // handle of the looping/state sound
    Vec3s dropOffset;                              // offset added to drop position with Wolf_DropHeld flag 8 (msg 0x428)
    ScnObject *heldObject;                         // object in hand / carried (mode 1)
    ScnObject *grabber;                            // object that froze the Wolf (msg 0xE); polled with msg 0x6B/0x49
    ScnObject *coverObject;                        // cover Ralph hides behind (kept by state flag 0x8000)
    ScnObject *killer;                             // object that crushed / sucked Ralph; 0x10 snaps onto its box top, 0x3c is pulled to it
    ScnObject *magnetTarget;                       // Wolf msg 0x53 stores arg[0], and the only 0x53 sender (Magnet_Update) passes {candidate, re
    ActionHit ctxAction;                           // ActionHit {action, target}: action = WolfContextAction; target = object offering the context action
    s32 heldActionType;                            // WolfHeldAction (also overwritten per state for the HUD prompt)
    s32 ctx4PromptIndex;                           // 4th word of the 0x10-byte context block; read as the index into g_wolfCtx4PromptText; writer not est
    ActionHit coverAction;                         // ActionHit {action, target}: action = 1 = cover zone available; target = cover zone record (side 0..3
    s32 coverWord2;                                // 3rd word of the 0x10-byte cover block at +0x650 (after the ActionHit coverAction.action/coverAction.
    s32 coverWord3;                                // 4th word of the cover block; only ever zeroed
    u16 itemPromptClassId;                         // u16 class id drawn by Scenaric_DrawClassIcon for the held-item prompt; Hud_DrawItemPrompt p
    u16 itemPromptLevel;                           // Word 1 of the msg 6 reply (0..0xffff, e.g. coin distance*0xffff/600). Hud_DrawItemPrompt copies it t
    ScnObject *bush;                               // Bush being worn
    ScnObject *elastic;                            // elastic being pulled (set by)
    WolfLaunchPath launchPath;                     // per-axis {p0,v,a} quadratic trajectory from msg 0xC plus its parameter t (+0x690); Wolf_LaunchArcSte
    ScnObject *frozenRiver;                        // cached class 0x53 object; msg 0x36
    ScnObject *sheepCostumeListener;               // object notified with msg 0x69 when Ralph bleats
    s32 timeOutOfWater;                            // ticks since leaving water (cap 0x3C000); wet footprints while <0x14000
    union {
        s16 savedBodyBox[8];                           // original collision box copied back before rescaling
        CollBox savedBox;                          // the same 16 bytes as a CollBox: Wolf::PostLoadInit copies the first model box here
    };
    ZoneList restrictions;                         // prop RESTRICTIONBOX list {Box **boxes; u16 count} (flag 0x40000000 = no run): Wolf_FilterInput takes
    Box *flyBox;                                   // prop FLYBOX: rocket flight clamp
    Box *updraftZone;                              // current updraft zone, 0 = left it; a Box: flags +0 (0x40000000 lift, 0x20000000 double), min.y +6 (0
    s32 glowDist;                                  // min distance to a light source this frame (msg 0x424); <0x200 tints, reset to 0x7fffffff
    u32 distanceTravelled;                         // accumulated XZ distance (msg 0x41A returns it)
    s32 bushNibbleTimer;                           // accumulates g_dt on msg 1 from a sheep in flag-0x10000 state; at 0x14000 -> state 0xA
    Vec3s savedRespawnPos;
    s16 savedRespawnFacing;
    Vec3s scriptWalkTarget;                        // destination of scripted walk (msg 0x12)
    s16 scriptWalkHeading;                         // final facing of scripted walk
    s16 fallLimitY;                                // Y the death falls stop at
    s16 waterDepthOffset;                          // offset added to water top for wade/swim thresholds
    Vec3s carryDropPos;                            // place position passed with msg 0x981 in state 0x15
    s16 lineHeading;                               // constrained heading (elastic via msg 0x2d, fishing rod)
    Vec3s savedCamRot;                             // camera rot saved when leaving follow mode
    s16 savedCamDist;                              // default 0x262
    Vec3s interactPos;                             // mine position (0xd0) / dance spot (0xc0)
    s16 interactHeading;                           // heading to hold during interactions
    s16 scale;                                     // 0x400 = 1.0, 0x155 shrunk
    u16 mashCount;                                 // action presses while frozen (10 to break out)
    u8 danceMove;
    u8 state;                                      // WolfState / WolfStateCarry id; switch variable
    u8 mode;                                       // 0 normal, 1 carrying lifted object; selects bank and state table
    u8 surface;                                    // 0 normal / 1 ice: selects per-surface tuning and record table
    u8 playerIndex;                                // 0/1 from g_wolfInstanceCount; 1 uses pad
    u8 modelSet;                                   // active model set (index into +0x5A0)
    u8 costume;                                    // 0 none, 4 rabbit
    u8 camOverride;                                // 0xFF none
    u32 fxFlags;                                   // attached-effect flags: 4 ice block floating, 8 dizzy fx, 0x4000 costume puff, 0x100000 ice block gro
    u32 flags;                                     // bits seen here: 0x200000 / lock the stick heading to one axis, 0x400/0xC00 resp. 0/0x800, 0
    s32 stuckFallTicks;                            // gameplay ticks at unchanged Y while in an ordinary falling state (4096 ticks/s)
    s16 stuckFallY;                                // Y at the previous falling-guard update
};

struct WolfArcScratch {
public:
    Vec3s rot;                                     // rotation passed to Wolf_ApplyMove; Wolf_LaunchArcStep's view of g_collScratchA
    u8 _pad006[0x2];
    Vec3s target;                                  // this frame's point on the launch parabola
    u8 _pad00e[0x2];
    Vec3s vel;                                     // velocity, zeroed
    u8 _pad016[0x2];
    Vec3s step;                                    // displacement target - pos
};

struct WolfClimbScratch {
public:
    Vec3s vel;                                     // velocity being built (u/s); Wolf_ClimbStep's view of g_collScratchA
    u8 _pad006[0x2];
    Vec3s rot;                                     // copy of the Wolf's rot
    u8 _pad00e[0x2];
    Vec3s target;
    u8 _pad016[0x2];
    Vec3s step;                                    // this frame's displacement
};

struct WolfDanceMove {
public:
    u16 padMask;
    u16 animId;                                    // animation of the dance move
};

struct WolfFloatScratch {
public:
    s32 sq[3];                                     // squares of vel's components; x and z give the speed by sqrt
    Vec3s vel;                                     // velocity (0,-200,0) plus the water current; Wolf_WaterFloatUpStep's view of g_collScratchA
    u8 _pad012[0x2];
    Vec3s rot;                                     // new rotation, facing kept
    u8 _pad01a[0x2];
    Vec3s flow;                                    // water current
    u8 _pad022[0x2];
    Vec3s step;                                    // this frame's displacement
};

struct WolfHoldScratch {
public:
    Mat34s m;                                      // the Wolf's rotation matrix, Mat34s_FromEulerScaled(&rot, m, 0) (Wolf_CalcHeldObjWorldPos);
    Vec4s out;                                     // rotated offset, output of Mat34s_TransformVec3s
    Vec3s in;                                      // local offset + held object's attach-link offset
};

struct WolfLineArg {
public:
    s32 reverse;                                   // msg 0x2d argument: nonzero turns the result by half a turn
    Vec3s *from;                                   // first point of the segment; also the origin of the side test
    Vec3s *to;                                     // second point of the segment
};

struct WolfMoveBank {
public:
    union {
        s16 surfaceTuning[2][9];                       // per-surface tuning, index Wolf.surface (0 normal, 1 ice), 0x12 bytes each; [4..8] read by Wolf_Surfa
        u16 surfaceTuningU[2][9];
    };
    WolfStateDesc *states;                         // state descriptor table, index Wolf.state
    MoveRecord *profiles[2];                       // movement profile tables, index Wolf.surface
    u16 stateCount;                                // 0xDF for bank 0 (Wolf_InitMoveConfig)
    u16 profileCount;                              // 0x12 for bank 0
    s16 params[9];                                 // nine more tuning words written by Wolf_InitMoveConfig; +0x3E/+0x40 are the run-tim
};

struct WolfMoveScratch {
public:
    Vec3s vel;                                     // velocity being built this step (the Wolf move steps point a local at g_collScratchA: 0x488e
    u8 _pad006[0x2];
    Vec3s rot;                                     // rotation being built (g_wolfTmpRot)
    u8 _pad00e[0x2];
    Vec3s step;                                    // per-frame displacement vel*dt (g_wolfTmpStep)
};

struct WolfRespawnArg {
public:
    Vec3s pos;                                     // msg 0x402 argument (sent by the CheckpointManager, built on its stack): the respawn positio
    s16 facing;                                    // the respawn facing, copied to Wolf.savedRespawnFacing
};

struct WolfSpotArg {
public:
    Vec3s pos;                                     // msg 0x2e argument: the spot, copied to Wolf.interactPos; also the first argument of Vec3s
    s16 radius;
};

struct WolfStateDesc {
public:
    u8 anim;
    u8 profile;                                    // index into the bank's MoveRecord table
    u8 heldMsgArg : 4;                             // (bits) low nibble: the argument of the held-object messages 0x10 / 0x15 / 0x16 (Wolf_EnterState mo
    u8 modelSet : 4;
    u8 camMode;                                    // camera mode used unless Wolf+0x713 camOverride != 0xff (Wolf_UpdateCamera byte [desc+3])
    u32 flags;                                     // state flags: bit 0 = loop the state animation (Wolf_SetState [desc+4] & 1); Wolf_EnterState
};

struct WolfSwimScratch {
public:
    Vec3s vel;                                     // velocity being built (Mobile_Steer output); the swim/sink steps' view of g_collScratchA (0x
    u8 _pad006[0x2];
    Vec3s rot;                                     // copy of the Wolf's rot
    u8 _pad00e[0x2];
    Vec3s flow;                                    // water current from Zone_GetFlowVelocity, added to vel
    u8 _pad016[0x2];
    Vec3s step;                                    // this frame's displacement
};

class WolfTrap : public ScnMobile {
public:
    virtual void PostLoadInit();                   // WolfTrap_PostLoadInit (override)
    virtual void Update();                         // WolfTrap_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // WolfTrap_HandleMessage (override)
    virtual void Reset();                          // WolfTrap_Reset (override)
    s32 SetVictimFrozen(s32 frozen);
    void SetState(u8 newState);
    s32 victimFrozen;                              // 1 while the trap holds the victim's input frozen. Zeroed by PostLoadInit, Reset and by HandleMessage
    s32 timerMs;                                   // Countdown in ms drained by g_dtMs with a SIGNED test. In state 1 it is the 0x400 ms (1.024 s) mashin
    ScnObject *pSam;                               // Sam (class 1), resolved once in PostLoadInit; sent MSG_SAM_FORCE_ALERT (0x33, arg 1) when the trap s
    ScnObject *pVictim;                            // The controllable currently held (Wolf class 0 or Robot class 0x65). Set from the MSG_USE sender; cle
    ScnObject *pBell;                              // The Bell (class 0x66), resolved once in PostLoadInit; sent msg 0x33 arg 1 when the trap closes â€” cap
    Vec3s homePos;                                 // Position captured at PostLoadInit; restored through ScnMobile_SetPosition when the trap returns to s
    u16 struggleCount;                             // PAD_CROSS presses registered in the current 0x400 ms window; at 6 the trap opens. Reset on entering
    Vec3s homeRot;                                 // Rotation captured at PostLoadInit and restored on entering state 0. While holding a victim the trap
    u8 state;                                      // 0 idle/armed (anim 4, home, no victim), 1 holding (anim 1, victim frozen, mash window), 2 opening (a
};

struct WolfWalkToArg {
public:
    Vec3s *target;                                 // msg 0x12 (scripted walk) argument: the destination, copied to Wolf.scriptWalkTarget ( mov ec
    Vec3s *rot;                                    // optional: the final rotation; its y (facing) goes to Wolf.scriptWalkHeading, 0 when null ( c
    Box *dropZone;
};

class WoodenLift : public ScnLogic {
public:
    virtual void PostLoadInit();                   // WoodenLift_PostLoadInit (override)
    virtual void Update();                         // WoodenLift_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // WoodenLift_HandleMessage (override)
    virtual void Reset();                          // WoodenLift_Reset (override)
    void UpdateBoxes();
    s32 IsDriverInBoxXZ(Box *box);
    s32 Move(s16 dir);
    s32 CarryObjects(Vec3s delta);
    ScnObject *robot;                              // first Robot (class 101) in the level or NULL; when it answers msg 0x3681 the camera-box test uses it
    Box *cameraBox;                                // IDCAMERABOX (prop +8); XZ-only test for the scripted lift camera
    s16 bottomY;                                   // lower end of travel (larger y, y points down)
    s16 topY;                                      // upper end of travel; one end is the start pos.y, the other -MAXZVALUE
    s16 cameraChangeY;                             // -CAMERACHANGEZVALUE; the scripted camera runs only while pos.y < this
    u16 speed;                                     // STEP*30 units/s; per-frame step = speed*g_dt>>12 (truncated)
    CamSetup *camera;                              // CAMERA property resource (Scn_GetPropCamera); u16 at +0, s16 at +2/+4/+6 and the pointer +8 are pass
    s16 dirOn;                                     // direction taken on msg 0x1f (points from the start end toward the other end)
    s16 dirOff;                                    // direction taken on msg 0x20 (back toward the start end)
    s16 dir;                                       // current direction, +1 down or -1 up; initialised to dirOn
    u16 soundHandle;                               // channel handle of motor sound 0x5e
    u8 frozen;                                     // Update does nothing while non-zero; only ever written 0 (PostLoadInit), so dead
    u8 switchedOn : 1;                             // (bits) WoodenLiftFlags bit0: switched on / camera armed (or/and byte ops ; read shr-fre
    u8 camReleased : 1;                            // (bits) WoodenLiftFlags bit1: scripted camera released (read shr al; and al 1; set or cl 2 at 0x
    u8 soundOn : 1;                                // (bits) WoodenLiftFlags bit2: motor sound playing (read shr dl 2; and dl 1; set or al 4 at 0x50a
    CollBox rideBox;                               // model box 0 + pos with min.y-5; objects overlapping it are carried
    CollBox underBox;                              // model box 0 + pos with max.y+50; bodyless objects inside block downward motion
    u16 downProbeResult;                           // Collide_ResolveMove result of the 30-unit downward probe (0 = clear)
    ContactInfo probeContact;                      // contact info of the downward probe; floorObj with inst flag 8 is ignored
    Vec3s probeDelta;                              // (0,30,0), the probe move vector
};

class WoodenPlatForm : public ScnLogic {
public:
    virtual void PostLoadInit();                   // WoodenPlatForm_PostLoadInit (override)
    virtual void Update();                         // WoodenPlatForm_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // WoodenPlatForm_HandleMessage (override)
    void MoveTowards(s16 targetY);
    void CarryObjects(Vec3s *delta);
    s32 GetLoad();
    Box *activationZone;                           // ACTIVATIONZONE box; its y extent is moved in place with the platform; objects overlapping it are car
    Box *bodyBox;                                  // model box 0 (model-space) used by the load test
    ScnObject *door;                               // DOOR object, translated by -delta (moves opposite the platform)
    ScnObject *mechanism;                          // IDMECHANISM object (DoorMechanism): msg 0x21 while moving, 0x22 on arrival
    s16 targetY[3];                                // indexed by WoodenPlatForm_GetLoad: [0] rest (start pos.y), [1] halfway, [2] -MINZVALUE
    s16 speed;                                     // STEP*30 units/s; step = speed*g_dt>>12
    u8 disabled;                                   // 1 when there is no ACTIVATIONZONE; Update does nothing
};

class WorldObj : public InstanceBase {
public:
    Aabb aabb;                                     // min = World-space minimum of the mesh vertices ((s16)__ftol), from WorldObj_CalcMeshAabb. Passed to
};

class balance : public ScnBody {
public:
    virtual void PostLoadInit();                   // balance_Init (override)
    virtual void Update();                         // balance_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // balance_HandleMessage (override)
    ScnObject *wolf;                               // The Wolf, found once at init by Scenaric_FindByClass(0). If the search fails it stays NULL and Updat
    u8 wolfInside;                                 // Latched 'the Wolf is in the trigger box' flag; the animation only changes on a transition (anim 2 pr
    Box triggerBox;                                // World-space trigger volume baked once at init from the model's first NON-solid box plus the object p
};

class bipbip : public ScnMobile {
public:
    virtual void PostLoadInit();                   // bipbip_PostLoadInit (override)
    virtual void Update();                         // bipbip_Update (override)
    virtual void Render(Camera *view);             // bipbip_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // bipbip_HandleMessage (override)
    virtual void Reset();                          // bipbip_Reset (override)
    EmitterDriftParams dustParams;                 // the dust trail's parameters, filled by PostLoadInit with Ralph's run-dust values {100, -20, 0xa30, 0
    TrailEmitter dust;                             // 16-particle dust trail (inline ctor in bipbip_Create); spawns while running (0x499
    u8 state;                                      // 0 waiting hidden until Ralph is inside startBox; 1 running along roadPath; 2 on the wall path into t
    u8 shadowOn : 1;                               // (bits) bit 0: the blob shadow is updated and drawn (Render); set by Reset, cleared on e
    PathFollower roadPath;                         // TRAJECTORY: the run along the road, started at SPEED_ONE by Reset; the bipbip sits at its
    PathFollower wallPath;                         // TRAJ_WALL: started at SPEED_TWO when the road path reaches its last segment, with the roa
    s32 speedOne;                                  // SPEED_ONE (s16 of prop +4): the road path's starting speed
    s32 speedTwo;                                  // SPEED_TWO (s16 of prop +8): the road speed after the first segment and the wall path's spe
    Box *startBox;                                 // BOX_DECL: Ralph's origin inside it starts the run; set only when BOX_DECL, TRAJECTORY and
    u32 inTunnel;                                  // 1 in state 2: Render draws the model moved on screen onto the wall path's projected point
};

class box : public ScnMobile {
public:
    virtual void PostLoadInit();                   // box_Init (override)
    virtual void Update();                         // box_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // box_HandleMessage (override)
    virtual void Reset();                          // box_Reset (override)
    ScnObject *contents;                           // The object this crate delivers. The crate never resolves it â€” Mailbox_Deliver writes it at
    u16 unk80;                                     // Zeroed at the moment the crate opens and referenced nowhere else in. Na
    u8 state;                                      // 0 idle/parked, 1 falling (driven down 800/s with a landing test), 2 opening (waiting for the open an
    s32 canRelease;                                // Gate on the release step: set by Init/Reset and by msg 0x30, cleared by msg 0x31. With it clear the
};

class bridge : public ScnBody {
public:
#ifdef SDW_MEMBERS_bridge
    SDW_MEMBERS_bridge
#endif
    virtual void PostLoadInit();                   // bridge_Init (override)
    virtual void Update();                         // bridge_Update (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // bridge_HandleMessage (override)
    virtual void Reset();                          // bridge_Reset (override)
    void StartSnapCamera();
    u8 CountWolfAndSheep();
    void Collapse();
    void PushObjectsDown();
    u8 state;                                      // 0 idle: counts Ralph + sheep in fallZone every update; 1 creaking (anim 5, one loop per anim end); 2
    Box *fallZone;                                 // FALLINGZONE (Scn_GetPropBox(record, 0)): where Ralph and a sheep together start the creaki
    Box *pushZone;                                 // IDOBJECTSFALLZONE box (first entry of its id list), else fallZone; bridge_PushO
    ScnObject *otherPart;                          // OTHERPART (Scn_GetPropObject(record, 8)). Only the half that has one is the master: it fre
    u8 creakLoops;                                 // creak animations finished in state 1: 0 on entering from state 0; 6 = freeze + snap camer
    u32 pairDist;                                  // XZ distance Ralph-sheep, (s32)sqrt, written by bridge_CountWolfAndSheep; > 300 starts the
    u32 pushDown;                                  // set by bridge_Collapse; while set every update calls bridge_PushObjectsDown. C
    u32 wolfFrozen;                                // Ralph's reply to msg 0xE (the freeze) at creak loop 6; while set the bridge sends msg 0xF
    u8 _pad084[0x6];
    Vec3s snapCamRot;                              // rotation of the snap camera: (0, bridge facing + 0x400, 0) (bridge_StartSnapCamera
    Vec3s snapCamPos;                              // eye of the snap camera: midpoint + the offset (0, 0, -max(pairDist, 600)) turned by (0, facing + 0x4
    u8 _pad096[0x2];
    Vec3s midpoint;                                // midpoint of Ralph and the sheep ((a + b) / 2 per axis), written by bridge_CountWolfAndSheep (0x49dc1
    u8 _pad09e[0x12];
};

class bull : public ScnMobile {
public:
    virtual void PostLoadInit();                   // bull_Init (override)
    virtual void Update();                         // bull_Update (override)
    virtual void Render(Camera *view);             // bull_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // bull_HandleMessage (override)
    virtual void Reset();                          // bull_Reset (override)
    void SetState(u8 state);
    u8 state;                                      // BullState (bull_SetState writes it at entry and exit)
    s32 chargeSpeed;                               // charge and return speed in u/s: 800, or 1100 when the target is Gossamer_Lev08 (0x44c)
    u32 hitTime;                                   // g_gameTime at SetState(4); state 4 goes back to state 2 after 0xa000 ticks
    u32 stateTime;                                 // g_gameTime at SetState(1/6/3); 2-s charge timeout, 1-s slide
    u32 tossClockBase;                             // g_gameTime of the last frame that was not tossing; the toss arc advances by (now - this)*256/0x28000
    s16 homeFacing;                                // rot.y at Init; restored by SetState(0)
    s16 skidFacing;                                // facing saved by SetState(3); never read
    u8 _pad094[0x2];
    u16 targetDist;                                // XZ distance output of Scenaric_FindBestInRadius in SetState(1); a hit is used only when < 900
    u16 wakeUpSpeedPct;                            // PROPERTY_BULL_WAKEUPSPEED; wake anim 0xe speed = pct*0x1000/100
    u32 woken;                                     // wake anim started; cleared by SetState(0) when asleep
    u32 chasingGossamer;                           // set when a charge targets class 0x57; only Init clears it; makes Reset restart the charge
    u32 asleep;                                    // PROPERTY_BULL_ASLEEP
    Vec3s chargeTargetPos;                         // target position snapshotted when a charge starts; the charge runs straight at it
    Vec3s velocity;                                // charge velocity (x, 250, z) in u/s; SetState(9) makes it (x/4, y*4, z/4)
    Vec3s homePos;                                 // ground-snapped Init position; target of state 8 and Reset
    CamSetup *sheepCam;                            // Scn_GetPropCamera(SHEEPCAM); CamShot started when a sheep is caught in SHEEPBOX
    Trajectory *sheepTraj;                         // Scn_GetPropTrajectory(SHEEPTRAJ); points 0..2 define the toss arc
    Box *sheepBox;                                 // Scn_GetPropBox(SHEEPBOX); sheep whose origin is inside get tossed
    ZoneList movementBoxes;                        // zone list {boxes, count}: Scn_FindIdList(MOVEMENTBOX); the charge target must be inside and each ste
    ZoneList activationBoxes;                      // zone list {boxes, count}: Scn_FindIdList(ACTIVATIONBOX); a noisy Wolf inside wakes the bull | count
    Box *noReturnBox;                              // Scn_GetPropBox(NORETURNBOX); escape volume for state 9
    ScnObject *target;                             // current charge target (Wolf, a 0x1e responder such as Gossamer, or the tossed sheep)
    u8 _pad0e0[0x4];
    ScnObject *tossedSheep;                        // sheep being tossed along tossArc; the toss driver runs while it is set and state == 4
    ScnObject *gossamer;                           // Scenaric_FindByClass(0x57 Gossamer_Lev08); inside a movement box it triggers a charge from state 2
    LaunchArc tossArc;                             // sheep toss arc: coefficients from SHEEPTRAJ, t at +0x110 (advanced by bull_Update's own quadratic cl
    CamShot sheepCamShot;                          // toss camera shot (initialised with noFreeze = 1)
    EmitterDriftParams dustParams;                 // Emitter_UpdateDrift params {s32 50, -60, 0x2000, 0x200; u16 size 0x50->0xb4; u8 0}
    InlineEmitter16 dustEmitter;                   // dust emitter with inline storage (0x150..0x2b4): bull_Create forms it, slotPool
    u16 soundHandle;                               // channel of the charge loop sfx 0xfd or the escape sfx 0xca
};

class elastic : public ScnBody {
public:
    virtual void PostLoadInit();                   // elastic_Init (override)
    virtual void Update();                         // elastic_Update (override)
    virtual void Render(Camera *view);             // elastic_Render (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // elastic_HandleMessage (override)
    virtual void Reset();                          // elastic_Reset (override)
    s32 ApplyTension(const Vec3s *stretch, const Vec3s *velocity, Vec3s *step);
    s32 StepPullUp(Vec3s *);
    s32 StepVibration(Vec3s *);
    s32 TrackWolfCrossing(const Vec3s *step);
    s8 ProjectOntoBand(const Vec3s *);
    void AddWolfRider();
    void AimCamera(const Vec3s *, CamSetup *, u32);
    void BuildBandFrame();
    void RemoveWolfRider();
    void RenderBand(Camera *);
    void ResetToItem();
    void SetBandMode(s32);
    void SetWolfFrozen(s32);
    void SetupLaunch(s32, const Vec3s *, s32, s32, s32);
    void StartPullUp(const Vec3s *, s32);
    void StartVibration(const Vec3s *, const Vec3s *, s32);
    void StepLaunch(Vec3s *);
    void UpdateBandMesh(const Vec3s *, s32);
    void UpdateTensionColour();
    Vec3s homePos;                                 // return position: set to pos by elastic_Init (after SnapToGround) and by msg 9 arg 1; elastic_Reset p
    u8 _pad06a[0x10a];
    u32 tensionColourTable[59];                    // 30 dwords at stride 8 (0x174 + 8*i, i < 30) set to bandColour every frame by elastic_UpdateTensionCo
    u8 bandMeshKey[16];                            // 16 bytes copied from the item model. Its address is the key of the elastic's private Mesh in g_texKe
    Model *itemModel;                              // the original (coiled item) model, restored by elastic_SetBandMode(0)
    u32 bandColour;                                // Rgb24_Lerp(0x10040ff, 0x10000d0, tension); written only
    ScnObject *tree1;                              // ElasticTree the first end is tied to (msg 0x17 finds it; msg 0x15 ties). When tree1 is untied while
    ScnObject *tree2;                              // second ElasticTree: the nearest tree found by the state-3 msg-2 query; set as the second anchor by m
    ScnObject *launched;                           // object being thrown in state 6: the Wolf (state-4 launch) or the nearest candidate found by elastic_
    ScnObject *camRestrict1;                       // CameraRestriction (IDCAMREST) of tree1, from msg 0x1a80; switched with msg 0xd80
    ScnObject *camRestrict2;                       // CameraRestriction of tree2, from msg 0x1a80
    Vec3s anchorA;                                 // tree1.pos with 0x50 subtracted from the vertical (80 above the tree base)
    Vec3s anchorB;                                 // tree2.pos with 0x50 subtracted from the vertical
    Vec3s bandMid;                                 // (A+B)/2: centre of the vibration after a release or launch
    u8 _pad29e[0x6];
    Vec3s wolfPrevPos;                             // Wolf position copied every Update in state 3; msg 0x1a subState 1 adds (Wolf pos - wolfPrevPos) to t
    Mat34s bandFrame;                              // yaw-only frame of the Aâ†’B line (heading + 0x800), built by elastic_BuildBandFrame; trans at +0x2c0 i
    s16 perpDist;                                  // |perpendicular distance| of the last projected point from the band line; 0 when outside the span. Mo
    s8 engagedSide;                                // ElasticSide (+0x2c / -0x2c) of the side the Wolf pushed into; 0x4d = none
    u8 engaged;                                    // 1 while the Wolf has crossed into the strung band and it follows him
    Vec3s *pullUpTarget;                           // ElasticTree pullUpPoint (&IDPOINTUP box.min) from msg 0x1a80: where the bungee pull-up takes Ralph
    Vec3s pullUpStart;                             // position at which the pull-up began
    s16 tension;                                   // 4.12: (len - max/2)/(max/2), clamped at 0 or above (0x1000 = LONGUEUR_MAX). Decays 0x40 per frame in
    s16 pullUpAngle;                               // eased pull-up phase, 0..0xa00
    s16 pullUpRate;                                // angle per ms << 12, from a pull speed of 614 u/s
    s16 launchTension;                             // tension latched at launch or release; scales DISTANCE and HAUTEUR
    s16 holdMs;                                    // ms held at tension >= 0xe00 while grounded; the launch fires above 999
    s8 overstretched;                              // set when the stretch exceeds LONGUEUR_MAX (elastic_ApplyTension)
    u8 unk2e5;                                     // zeroed by elastic_ResetToItem only; no reader found
    u8 _pad2e6[0x2];
    Vec3s launchDir;                               // 4.12 horizontal unit vector (x, 0, z) = -stretch/|stretch|
    s16 launchTravelled;                           // horizontal distance covered by the launch
    s16 launchSpeed;                               // VITESSE_PROJECTION*1024/1000; the step is launchSpeed*g_dtMs>>10
    s16 launchAnglePerUnit;                        // 0x800000/distance, so the sine hump ends at 0x800; wraps for distance <= 256, and distance 0 is an i
    s16 launchBaseY;                               // vertical position of the launched object at the start
    s16 launchHeight;                              // HAUTEUR_PROJECTION*T>>12, the arc apex
    Vec3s vibFrom;                                 // vibration end point 1 (release position)
    Vec3s vibTo;                                   // vibration end point 2 = 2*mid - from, with +0xa0 on the vertical
    s16 vibT;                                      // lerp parameter around 0x800
    s16 vibRate;                                   // (rate<<12)/1000 truncated to s16 (0x2000 â†’ -31982); sign flips at each end
    s16 vibAmp;                                    // amplitude, starts at 0x800 and is multiplied by 3/4 at each end; the vibration stops below 1
    Vec3s stretch;                                 // state 3: Wolf - A. States 4 and 5: 2*Wolf - (A+B). Input of elastic_ApplyTension and the launch dire
    s16 VITESSE_PROJECTION;                        // designer property: launch speed, units/s
    s16 DISTANCE_PROJECTION;                       // designer property: launch distance at full tension
    s16 HAUTEUR_PROJECTION;                        // designer property: launch arc height at full tension
    s16 LONGUEUR_MAX;                              // designer property: maximum stretch length
    CamSetup *camera;                              // IDCAMERA setup {u16 focal; u16 rot[3]; Vec3s pos}; the launch of a non-Wolf object is followed from
    u8 state;                                      // ElasticState
    u8 subState;                                   // ElasticSubState (state 3 only)
    u8 isWolfRider;
    u8 wolfFrozen;                                 // 1 if this elastic froze the Wolf (msg 0xe) for its launch camera
    s32 tensionRemainder = 0;
};

class seesaw : public ScnBody {
public:
#ifdef SDW_MEMBERS_seesaw
    SDW_MEMBERS_seesaw
#endif
    virtual void PostLoadInit();                   // seesaw_Init (override)
    virtual void Update();                         // seesaw_Update (override)
    virtual s32 CustomCollide(ScnObject *querier, CollBox *mover, Vec3s *disp, s32 *outFrac, s32 *outY, CollContact *contacts, s32 *nContacts, u32 mode); // seesaw_CollideBody (override)
    virtual sptr HandleMessage(ScnObject *sender, u32 msgId, void *arg); // seesaw_HandleMessage (override)
    virtual void Reset();                          // seesaw_Reset (override)
    s32 GroundQuery(GroundQuery *query);
    s32 IsBodyObstructed();
    void AddBody(ScnObject *obj, s32 landingSpeed);
    void ArcCoeffsFrom3Points(s32 p0, s32 pMid, s32 pEnd, s32 *outP0, s32 *outV, s32 *outA);
    void LaunchObject(ScnObject *obj, const Trajectory3 *trajectory, CamSetup *camera, u32 camParam, u32 hop);
    void RemoveLaunch(s32 index);
    void UpdateEjectionTimers();
    void UpdateLaunches();
    Box footprint;                                 // world box centred on pos with the model-box half extents (flags 0 at +0x64, min +0x68, max +0x6e); u
    Box *raisedBox;                                // Box* set by Init to &+0x88; read 31 times as a Box by seesaw_CollideBody (in the
    Box footprintUp500;                            // copy of the footprint with the top (min.y +0x7e) raised 500
    Box footprintUp1500;                           // copy of +0x78 with the top raised another 1000
    s16 centreOnAxis;                              // coordinate subtracted from obj.x (or .z when axisIsZ) to classify the landing side (+-30 dead zone)
    s16 plankTopOffset;                            // -modelBox.min.y: height of the plank top above the pivot; pivotY = pos.y - plankTopOffset*cos>>12
    u16 halfWidth;                                 // half extent across the plank axis
    u16 halfLength;                                // half extent along the plank axis
    s16 tilt;                                      // plank angle (4096/turn), clamped to [-maxTiltNeg, maxTiltPos]; written into rot.z
    s16 maxTiltNeg;                                // MAXANGLE degrees * 4096/360
    s16 maxTiltPos;                                // MAXANGLE2 * 4096/360, or maxTiltNeg when 0
    s32 tiltVel;                                   // low-pass filtered tilt error (3*delta when |delta| > 0xcd, else (3*v + delta)/4)
    u16 bodyCount;                                 // entries in bodies (max 32)
    u16 launchCount;                               // seesaw-driven launches (max 32)
    SeesawBodyEntry bodies[32];                    // 32 x {s32 landingSpeed; s32 canLaunch; ScnObject *obj} filled by seesaw_AddBody
    LaunchArc launches[32];                        // arcs the seesaw drives itself (objects that returned 0 to msg 0xC)
    Vec3s launchPos[32];                           // last arc position per launch
    Trajectory3 *traj[2];                          // TRAJ1/TRAJ2 export records (kind 3 only); objects on the negative/positive side are launched along t
    CamSetup *launchCam[2];                        // CAMERA1/CAMERA2 camera setups handed to seesaw_LaunchObject
    u32 launchCamFlags[2];                         // CamScriptFlags: (CAMERAnINTERPB != 0) | (CAMERAnINTERPE != 0)<<1
    s16 pivotY;                                    // plank surface height at the pivot
    s16 pivotAxisCoord;                            // pos.x (X plank) or pos.z (Z plank)
    s16 tiltSin;                                   // sin(tilt) 4.12
    s16 tiltCos;                                   // cos(tilt) 4.12 (0x1000 at Create)
    CamShot camShot;                               // launch camera shot
    Box *ejectionBox1;                             // EJECTIONBOX1 (Scn_FindIdList, only when the list has exactly one entry;..): tested against
    Box *ejectionBox2;                             // EJECTIONBOX2 (Scn_FindIdList, only when the list has exactly one entry;..): tested against
    s16 ejectionTimer1;                            // ejection timer (ms) for side 1: 4000 while Ralph is inside its ejection box, 100 while Wolf msg 0x40
    s16 ejectionTimer2;                            // ejection timer (ms) for side 2: 4000 while Ralph is inside its ejection box, 100 while Wolf msg 0x40
    u16 creakSound;                                // handle of sound 0x108; cleared when it stops or on Reset
    u32 seesawFlags;                               // bit 1: launch camera shot does not freeze the Wolf
    u32 axisIsZ;                                   // 0: plank along X, else along z
    SeesawPendingBits pendingLaunch;               // bit 0 negative side, bit 1 positive side, bit 2 set by a falling/rolling Rock landing (msg 0x5C == 1
    s8 hopIdx;                                     // next hopTraj slot (mod 4)
    Trajectory3 hopTraj[4];                        // 4 trajectory records of 0x14 bytes: {3; obj pos; (x, y-300, z); (x, seesaw.y, z)}, a vertical hop fo
};

#endif
