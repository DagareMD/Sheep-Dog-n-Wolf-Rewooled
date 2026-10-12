#ifndef SDW_ENGINE_SCREEN_H
#define SDW_ENGINE_SCREEN_H

/* The functions and globals screen.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

class Screen;

extern Screen g_screen;

/* A drawing group: nested groups belong to the outermost one. Its centre is read on the first
 * coordinate, after the drawing function has established its rectangle or text window. */
class HudElement {
public:
    enum Anchor { Start, Centre, End };
    enum Mapping { Stretch };
    typedef void (*CentreFn)(const void *data, float &x, float &y);

    HudElement(Anchor x, Anchor y, CentreFn centre = 0, const void *data = 0);
    HudElement(Anchor x, Anchor y, float centreX, float centreY);
    explicit HudElement(Mapping mapping);
    ~HudElement();

    static bool Active();
    static float MapX(s32 x);
    static float MapY(s32 y);
    static float EdgeX(s32 x);
    static float EdgeY(s32 y);
    static float FromCentreX(s32 x);
    static float FromCentreY(s32 y);
    static float SizeX(s32 x);
    static float SizeY(s32 y);
    static void CentreScreen(const void *data, float &x, float &y);
    static void CentreRect(const void *data, float &x, float &y);

private:
    HudElement(const HudElement &);
    HudElement &operator=(const HudElement &);
    void Resolve();

    bool stock, resolved;
    Anchor ax, ay;
    CentreFn centre;
    const void *data;
    float cx, cy, mx, bx, my, by;
};

#endif
