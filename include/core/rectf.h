#ifndef _FLOATY_CORE_RECTF
#define _FLOATY_CORE_RECTF

#include <SDL.h>
#include <core/vec.h>

typedef struct rectf {
  float x;
  float y;
  float w;
  float h;
} rectf;

SDL_bool vec2f_in_rectf(const vec2f *vec, const rectf *rect) {
  return vec->x > rect->x &&
         vec->x < rect->x + rect->w &&
         vec->y > rect->y &&
         vec->y < rect->y + rect->h;
}

#endif