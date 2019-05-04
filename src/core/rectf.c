#include <core/rectf.h>

SDL_bool vec2f_in_rectf(const vec2f *vec, const rectf *rect) {
  return vec->x > rect->x &&
         vec->x < rect->x + rect->w &&
         vec->y > rect->y &&
         vec->y < rect->y + rect->h;
}

SDL_Rect rectf_to_sdl_rect(const rectf *rect) {
    return (SDL_Rect) {
        rect->x,
        rect->y,
        rect->w,
        rect->h
    };
}