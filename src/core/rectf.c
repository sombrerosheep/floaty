#include <core/rectf.h>

SDL_Rect rectf_to_sdl_rect(const rectf *rec) {
  SDL_Rect sdl_rec = {
    (int)rec->x,
    (int)rec->y,
    (int)rec->w,
    (int)rec->h
  };

  return sdl_rec;
}

SDL_bool rectf_intersects_rectf(const rectf *a, const rectf *b, rectf *intersection) {
  float a_min, a_max;
  float b_min, b_max;

  // x
  a_min = a->x;
  a_max = a->x + a->w;
  b_min = b->x;
  b_max = b->x + b->w;

  if (a_min < b_min) {
    a_min = b_min;
  }
  intersection->x = a_min;

  // w
  if (b_max < a_max) {
    a_max = b_max;
  }
  intersection->w = a_max - a_min;

  if (a_max <= a_min) {
    return SDL_FALSE;
  }

  // y
  a_min = a->y;
  a_max = a->y + a->h;
  b_min = b->y;
  b_max = b->y + b->h;

  if (a_min < b_min) {
    a_min = b_min;
  }
  intersection->y = a_min;

  // h
  if (b_max < a_max) {
    a_max = b_max;
  }
  intersection->h = a_max - a_min;

  if (a_max <= a_min) {
    return SDL_FALSE;
  }

  return SDL_TRUE;
}
