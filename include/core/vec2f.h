#ifndef _FLOATY_CORE_VEC2F
#define _FLOATY_CORE_VEC2F

#include <SDL.h>

typedef struct vec2f {
  float x, y;
} vec2f;

SDL_Point vec2f_to_sdl_point(const vec2f *vec);
vec2f vec2f_add_vec2f(const vec2f *vec, const vec2f *delta);
vec2f vec2f_mul_float(const vec2f *vec, float f);
float vec2f_magnitude(const vec2f *vec);
vec2f vec2f_normalized(const vec2f *vec);

#endif