#include <graphics/renderer.h>

static SDL_Renderer *floaty_renderer;

void set_floaty_renderer(SDL_Renderer *renderer) {
  floaty_renderer = renderer;
}

void render_clear(const color *c) {
  SDL_SetRenderDrawColor(floaty_renderer, c->r, c->g, c->b, c->a);
  SDL_RenderClear(floaty_renderer);
}

void floaty_display() {
  SDL_RenderPresent(floaty_renderer);
}

void release_renderer() {
  floaty_renderer = NULL;
}

void render_fill_rectf(const rectf *rect, const color *c) {
  SDL_Rect sdl_rec = rectf_to_sdl_rect(rect);
  SDL_SetRenderDrawColor(floaty_renderer, c->r, c->g, c->b, c->a);
  SDL_RenderFillRect(floaty_renderer, &sdl_rec);
}

void render_fill_rectfs(int count, const rectf *rect, const color *c) {
  SDL_Rect *sdl_rects = SDL_malloc(sizeof(SDL_Rect) * count);
  for (int i = 0; i < count; i++) {
    sdl_rects[i] = rectf_to_sdl_rect(&rect[i]);
  }

  SDL_SetRenderDrawColor(floaty_renderer, c->r, c->g, c->b, c->a);
  SDL_RenderFillRects(floaty_renderer, sdl_rects, count);

  SDL_free(sdl_rects);
}

void render_vec2f(const vec2f *vec, const color *c) {
  SDL_SetRenderDrawColor(floaty_renderer, c->r, c->g, c->b, c->a);
  SDL_RenderDrawPoint(floaty_renderer, (int)vec->x, (int)vec->y);
}