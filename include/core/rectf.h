#ifndef _FLOATY_CORE_RECTF
#define _FLOATY_CORE_RECTF

#include <SDL.h>

typedef struct rectf {
  float x, y;
  float w, h;
} rectf;

SDL_Rect rectf_to_sdl_rect(const rectf *rec);
SDL_bool rectf_intersects(const rectf *a, const rectf *b);
SDL_bool rectf_intersection(const rectf *a, const rectf *b, rectf *intersection);

#endif