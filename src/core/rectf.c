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

SDL_bool rectf_intersects(const rectf *A, const rectf *B) {
  float a_min, a_max, b_min, b_max;

  a_min = A->x;
  a_max = A->x + A->w;
  b_min = B->x;
  b_max = B->x + B->w;

  if (b_min > a_min)
    a_min = b_min;
  if (b_max < a_max)
    a_max = b_max;
  if (a_max <= a_min)
    return SDL_FALSE;

  a_min = A->y;
  a_max = A->y + A->h;
  b_min = B->y;
  b_max = B->y + B->h;

  if (b_min > a_min)
    a_min = b_min;
  if (b_max < a_max)
    a_max = b_max;
  if (a_max <= a_min)
    return SDL_FALSE;

  return SDL_TRUE;
}
