#ifndef _FLOATY_FLOATY_WORLD
#define _FLOATY_FLOATY_WORLD

#include <core/rectf.h>

typedef struct world {
  rectf *geometry;
  int num_geometry;
} world;

void draw_world(const world *w);

void free_world(world *w);

#endif