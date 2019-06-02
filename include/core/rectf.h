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
rectf rectf_intersection(const rectf *a, const rectf *b);
SDL_Rect rectf_to_sdl_rect(const rectf *rect);
rectf make_containment_rectf(const rectf *a, const rectf *b);
SDL_bool rectf_intersects(const rectf *a, const rectf *b);

#endif