#include <core/vec.h>
#include <SDL_assert.h>
#include <SDL_stdinc.h>

vec2f add_vec2f(const vec2f *v, const vec2f *d) {
  vec2f res = {
    v->x + d->x,
    v->y + d->y
  };

  return res;
}

vec2f add_vec2fs(int count, ...) {
  vec2f vec = {0.f, 0.f };
  va_list args;
  va_start(args, count);

  for (int i = 0; i < count; i++) {
    vec2f v = va_arg(args, vec2f);
    vec.x += v.x;
    vec.y += v.y;
  }

  va_end(args);
  return vec;
}

vec2f add_vec2f_float(const vec2f *v, float f) {
vec2f res = {
    v->x + f,
    v->y + f
  };

  return res;
}

vec2f mul_vec2f_float(const vec2f *v, float f) {
  vec2f res = {
    v->x * f,
    v->y * f
  };

  return res;
}

vec2f div_vec2f_float(const vec2f *v, float f) {
  SDL_assert(f != 0);
  vec2f res = {
    v->x / f,
    v->y / f
  };

  return res;
}

float vec2f_magnitude(const vec2f *p) {
  return (float)SDL_sqrt(SDL_pow(p->x, 2) + SDL_pow(p->y, 2));
}

vec2f normalize_vec2f(const vec2f *p) {
  float mag = vec2f_magnitude(p);
  if (mag == 0 ) {
    return *p;
  }
  if (mag < 0) {
    mag *= -1.0;
  }

  return div_vec2f_float(p, mag);
}