#include <core/vec2f.h>

SDL_Point vec2f_to_sdl_point(const vec2f *vec) {
  SDL_Point p = {
    (int)vec->x,
    (int)vec->y
  };

  return p;
}