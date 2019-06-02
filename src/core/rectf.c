#include <core/rectf.h>

SDL_bool vec2f_in_rectf(const vec2f *vec, const rectf *rect) {
  return vec->x > rect->x &&
         vec->x < rect->x + rect->w &&
         vec->y > rect->y &&
         vec->y < rect->y + rect->h;
}

SDL_Rect rectf_to_sdl_rect(const rectf *rect) {
    SDL_Rect rec = {
        rect->x,
        rect->y,
        rect->w,
        rect->h
    };

    return rec;
}

rectf rectf_intersection(const rectf *a, const rectf *b) {
  float a_min, b_min, a_max, b_max;

  rectf result = {0.f, 0.f, 0.f, 0.f};

  a_min = a->x;
  a_max = a->x + a->w;
  b_min = b->x;
  b_max = b->x + b->w;

  // set intersection x
  if (b_min > a_min) {
    a_min = b_min;
  }
  result.x = a_min;

  // set intersection width
  if (b_max < a_max) {
    a_max = b_max;
  }
  result.w = a_max - a_min;

  a_min = a->y;
  a_max = a->y + a->h;
  b_min = b->y;
  b_max = b->y + b->h;

  // set intersection x
  if (b_min > a_min) {
    a_min = b_min;
  }
  result.y = a_min;

  // set intersection width
  if (b_max < a_max) {
    a_max = b_max;
  }
  result.h = a_max - a_min;

  return result;
}

rectf make_containment_rectf(const rectf *a, const rectf *b) {
  float a_min, b_min, a_max, b_max;

  rectf result = {0.f, 0.f, 0.f, 0.f};

  a_min = a->x;
  a_max = a->x + a->w;
  b_min = b->x;
  b_max = b->x + b->w;

  if (b_min < a_min) {
    a_min = b_min;
  }
  result.x = a_min;

  if (b_max > a_max) {
    a_max = b_max;
  }
  result.w = a_max - a_min;

  a_min = a->y;
  a_max = a->y + a->h;
  b_min = b->y;
  b_max = b->y + b->h;

  if (b_min < a_min) {
    a_min = b_min;
  }
  result.y = a_min;

  if (b_max > a_max) {
    a_max = b_max;
  }
  result.h = a_max - a_min;

  return result;
}

SDL_bool rectf_intersects(const rectf *a, const rectf *b) {
  float a_min, a_max, b_min, b_max;

  a_min = a->x;
  a_max = a->x + a->w;
  b_min = b->x;
  b_max = b->x + b->w;

  if (b_min > a_min) {
    a_min = b_min;
  }
  if (b_max < a_max) {
    a_max = b_max;
  }
  if (a_min >= a_max) {
    return SDL_FALSE;
  }

  a_min = a->y;
  a_max = a->y + a->h;
  b_min = b->y;
  b_max = b->y + b->h;

  if (b_min > a_min) {
    a_min = b_min;
  }
  if (b_max < a_max) {
    a_max = b_max;
  }
  if (a_min >= a_max) {
    return SDL_FALSE;
  }

  return SDL_TRUE;
}