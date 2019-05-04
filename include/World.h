#ifndef _FLOATY_WORLD
#define _FLOATY_WORLD

#include <SDL.h>

typedef struct WorldGeometry {
  SDL_Rect *rects;
  int count;
} WorldGeometry;

typedef struct World {
  WorldGeometry geometry;

  void (*update)(struct World *world);
  void (*draw)(struct World *world, SDL_Renderer *renderer);
} World;

World* new_world(const SDL_Rect *rects, int num_rects);
void free_world(World *world);

#endif // _FLOATY_WORLD