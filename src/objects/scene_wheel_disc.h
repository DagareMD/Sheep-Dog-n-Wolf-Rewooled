#ifndef SDW_OBJECTS_SCENE_WHEEL_DISC_H
#define SDW_OBJECTS_SCENE_WHEEL_DISC_H

/* Rewooled: the language wheel model's disc cut into a slice per language (scene_wheel_disc.cpp) */
class Instance;

/* rebuilds the wheel's model with a slice per language, once a load (Scene_Wheel::PostLoadInit) */
void SceneWheel_BuildDisc(Instance *wheel);

#endif
