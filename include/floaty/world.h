#ifndef _FLOATY_FLOATY_WORLD
#define _FLOATY_FLOATY_WORLD

#include <core/rectf.h>

typedef struct world {
  rectf *geometry;
  int num_geometry;
  rectf goal;
  SDL_bool goal_met;
} world;

void draw_world(const world *w);

void free_world(world *w);

#endif