#include <core/vec2f.h>

SDL_Point vec2f_to_sdl_point(const vec2f *vec) {
  SDL_Point p = {
    (int)vec->x,
    (int)vec->y
  };

  return p;
}

vec2f vec2f_add_vec2f(const vec2f *vec, const vec2f *delta) {
  vec2f new_vec = {
    vec->x + delta->x,
    vec->y + delta->y
  };

  return new_vec;
}