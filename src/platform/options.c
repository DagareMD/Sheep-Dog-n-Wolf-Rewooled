/* The game's start-up options (options.h). */
#include "options.h"
#include "save_store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPT_REG_SZ 1 /* the registry's string type, which the save store keeps */

GameOptions g_options;

void Options_SetDefaults(GameOptions *o)
{
    memset(o, 0, sizeof(*o));
    o->width = 1920;
    o->height = 1080;
    o->renderScale = 100;
    o->msaa = 0;
    o->fullscreen = 0;
    o->controller = OPTIONS_CONTROLLER_KEYBOARD;
    o->startLevel = -1;
    o->exeDir[0] = 0;
    o->checkUpdates = 1;
}

static int Options_ReadString(SaveKey *key, const char *name, char *out, unsigned outSize)
{
    unsigned type = 0, size = outSize - 1;
    if (Save_QueryValue(key, name, &type, out, &size) != SAVE_OK || type != OPT_REG_SZ)
        return 0;
    out[size < outSize ? size : outSize - 1] = 0;
    return 1;
}

static void Options_ReadInt(SaveKey *key, const char *name, int *out)
{
    char text[32];
    if (Options_ReadString(key, name, text, sizeof(text)))
        *out = atoi(text);
}

void Options_LoadSaved(GameOptions *o)
{
    SaveKey *key;
    if (!Save_KeyExists("") || !(key = Save_OpenKey("")))
        return;
    Options_ReadString(key, "Renderer", o->renderer, sizeof(o->renderer));
    Options_ReadInt(key, "Width", &o->width);
    Options_ReadInt(key, "Height", &o->height);
    Options_ReadInt(key, "RenderScale", &o->renderScale);
    Options_ReadInt(key, "Msaa", &o->msaa);
    Options_ReadInt(key, "Fullscreen", &o->fullscreen);
    Options_ReadInt(key, "Controller", &o->controller);
    Options_ReadInt(key, "StartLevel", &o->startLevel);
    Options_ReadString(key, "GameDir", o->exeDir, sizeof(o->exeDir));
    Options_ReadInt(key, "CheckUpdates", &o->checkUpdates);
    Save_CloseKey(key);
}

static void Options_WriteString(SaveKey *key, const char *name, const char *value)
{
    Save_SetValue(key, name, OPT_REG_SZ, value, (unsigned)strlen(value) + 1);
}

static void Options_WriteInt(SaveKey *key, const char *name, int value)
{
    char text[32];
    snprintf(text, sizeof(text), "%d", value);
    Options_WriteString(key, name, text);
}

void Options_Save(const GameOptions *o)
{
    SaveKey *key = Save_OpenKey("");
    if (!key)
        return;
    Options_WriteString(key, "Renderer", o->renderer);
    Options_WriteInt(key, "Width", o->width);
    Options_WriteInt(key, "Height", o->height);
    Options_WriteInt(key, "RenderScale", o->renderScale);
    Options_WriteInt(key, "Msaa", o->msaa);
    Options_WriteInt(key, "Fullscreen", o->fullscreen);
    Options_WriteInt(key, "Controller", o->controller);
    Options_WriteInt(key, "StartLevel", o->startLevel);
    Options_WriteString(key, "GameDir", o->exeDir);
    Options_WriteInt(key, "CheckUpdates", o->checkUpdates);
    Save_CloseKey(key);
}

void Options_SaveRenderSettings(int renderScale, int msaa)
{
    SaveKey *key = Save_OpenKey("");
    if (!key)
        return;
    Options_WriteInt(key, "RenderScale", renderScale);
    Options_WriteInt(key, "Msaa", msaa);
    Save_CloseKey(key);
}

void Options_SaveCheckUpdates(int on)
{
    SaveKey *key = Save_OpenKey("");
    if (!key)
        return;
    Options_WriteInt(key, "CheckUpdates", on);
    Save_CloseKey(key);
}

/* the value of "--name=value" / "-name=value", or 0 when arg is not that option */
static const char *Options_Value(const char *arg, const char *name)
{
    size_t len = strlen(name);
    if (arg[0] != '-')
        return 0;
    arg += arg[1] == '-' ? 2 : 1;
    if (strncmp(arg, name, len) != 0 || arg[len] != '=')
        return 0;
    return arg + len + 1;
}

static int Options_Flag(const char *arg, const char *name)
{
    if (arg[0] != '-')
        return 0;
    arg += arg[1] == '-' ? 2 : 1;
    return strcmp(arg, name) == 0;
}

void Options_ParseArgs(GameOptions *o, int argc, char **argv)
{
    int i;
    const char *v;
    for (i = 1; i < argc; i++) {
        const char *a = argv[i];
        if ((v = Options_Value(a, "renderer")) != 0) {
            strncpy(o->renderer, v, sizeof(o->renderer) - 1);
            o->renderer[sizeof(o->renderer) - 1] = 0;

        } else if ((v = Options_Value(a, "game-dir")) != 0) {
            strncpy(o->exeDir, v, sizeof(o->exeDir) - 1);
            o->exeDir[sizeof(o->exeDir) - 1] = 0;
        } else if ((v = Options_Value(a, "width")) != 0 && atoi(v) > 0) {
            o->width = atoi(v);
        } else if ((v = Options_Value(a, "height")) != 0 && atoi(v) > 0) {
            o->height = atoi(v);
        } else if ((v = Options_Value(a, "scale")) != 0 && atoi(v) > 0) {
            o->renderScale = atoi(v);
        } else if ((v = Options_Value(a, "msaa")) != 0) {
            o->msaa = atoi(v);
        } else if (Options_Flag(a, "fullscreen")) {
            o->fullscreen = 1;
        } else if (Options_Flag(a, "windowed")) {
            o->fullscreen = 0;
        } else if ((v = Options_Value(a, "controller")) != 0) {
            o->controller = strcmp(v, "keyboard") == 0 ? OPTIONS_CONTROLLER_KEYBOARD : atoi(v);
        } else if ((v = Options_Value(a, "level")) != 0) {
            o->startLevel = atoi(v);
        }
    }
}

int Options_ToArgs(const GameOptions *o, char buf[][OPTIONS_ARG_SIZE], int max)
{
    int n = 0;
    if (o->renderer[0] && n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--renderer=%s", o->renderer);
    if (n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--width=%d", o->width);
    if (n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--height=%d", o->height);
    if (n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--scale=%d", o->renderScale);
    if (n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--msaa=%d", o->msaa);
    if (n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "%s", o->fullscreen ? "--fullscreen" : "--windowed");
    if (n < max) {
        if (o->controller == OPTIONS_CONTROLLER_KEYBOARD)
            snprintf(buf[n++], OPTIONS_ARG_SIZE, "--controller=keyboard");
        else
            snprintf(buf[n++], OPTIONS_ARG_SIZE, "--controller=%d", o->controller);
    }
    if (o->exeDir[0] && n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--game-dir=%s", o->exeDir);
    if (o->startLevel != -1 && n < max)
        snprintf(buf[n++], OPTIONS_ARG_SIZE, "--level=%d", o->startLevel);
    return n;
}
