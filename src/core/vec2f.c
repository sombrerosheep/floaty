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

vec2f vec2f_mul_float(const vec2f *vec, float f) {
  vec2f result = {
    vec->x * f,
    vec->y * f
  };

  return result;
}

float vec2f_magnitude(const vec2f *vec) {
  float sums = SDL_powf(vec->x, 2.f) + SDL_powf(vec->y, 2.f);
  float result = SDL_sqrtf(sums);

  return result;
}

vec2f vec2f_normalized(const vec2f *vec) {
  vec2f result;
  float mag = vec2f_magnitude(vec);

  if (mag == 0.f) {
    result.x = 0.f;
    result.y = 0.f;
  } else {
    result.x = vec->x / mag;
    result.y = vec->y / mag;
  }

  return result;
}
