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

SDL_bool vec2f_in_rectf(const vec2f *vec, const rectf *rect);
SDL_Rect rectf_to_sdl_rect(const rectf *rect);
SDL_bool rectf_intersects(const rectf *A, const rectf *B);

#endif