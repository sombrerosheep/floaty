#include <World.h>

void world_update(World *world) {
}

void world_draw(World *world, SDL_Renderer *renderer) {
  SDL_Rect *rects = (SDL_Rect*)SDL_malloc(sizeof(SDL_Rect) * world->geometry.count);
  for (int i = 0; i < world->geometry.count; i++) {
    rects[i] = rectf_to_sdl_rect(&world->geometry.rects[i]);
  }

  SDL_SetRenderDrawColor(renderer, 0x32, 0xB4, 0x32, 0xFF);
  SDL_RenderFillRects(renderer, rects, world->geometry.count);

  SDL_free(rects);
}

void free_world(World *world) {
  SDL_free(world->geometry.rects);
  SDL_free(world);
}

World* new_world(const rectf *rects, int num_rects) {
  World *w = (World*)SDL_calloc(1, sizeof(World));

  w->geometry.count = num_rects;
  w->geometry.rects = (rectf*)SDL_calloc(num_rects, sizeof(rectf));
  SDL_memcpy(w->geometry.rects, rects, sizeof(rectf) * num_rects);

  w->draw = world_draw;
  w->update = world_update;

  return w;
}