#include <World.h>

void world_update(World *world) {
}

void world_draw(World *world, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0x32, 0xB4, 0x32, 0xFF);
  SDL_RenderFillRects(renderer, world->geometry.rects, world->geometry.count);
}

void free_world(World *world) {
  SDL_free(world->geometry.rects);
  SDL_free(world);
}

World* new_world(const SDL_Rect *rects, int num_rects) {
  World *w = (World*)SDL_calloc(1, sizeof(World));

  w->geometry.count = num_rects;
  w->geometry.rects = SDL_calloc(num_rects, sizeof(SDL_Rect));
  SDL_memcpy(w->geometry.rects, rects, sizeof(SDL_Rect) * num_rects);

  w->draw = world_draw;
  w->update = world_update;

  return w;
}