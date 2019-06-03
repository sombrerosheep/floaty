#ifndef _FLOATY_CORE_VEC2F
#define _FLOATY_CORE_VEC2F

#include <SDL.h>

typedef struct vec2f {
  float x, y;
} vec2f;

SDL_Point vec2f_to_sdl_point(const vec2f *vec);

#endif