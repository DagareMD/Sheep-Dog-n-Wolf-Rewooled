#ifndef SDW_ENGINE_POLYBATCHER_H
#define SDW_ENGINE_POLYBATCHER_H

/* The functions and globals polybatcher.cpp defines, declared once for every file that uses them. */
#include "sdw_types.h"

class RenderPoly;

int PolyBatcher_CompareSortZ(RenderPoly **a, RenderPoly **b);
/* Rewooled: a page holding the PNG at path (a language pack's picture or font), after the level's pages (before the
 * four blank pages, which stay last) and put back there whenever they are loaded again; its index, or -1. square: the image in the top of a square page. Kept until
 * the next level's batcher. */
s32 PolyBatcher_AddImagePage(const char *path, u32 square);

#endif
