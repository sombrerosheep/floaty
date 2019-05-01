#ifndef _FLOATY_CORE_VEC
#define _FLOATY_CORE_VEC

#include <stdarg.h>

typedef struct vec2f {
  float x;
  float y;
} vec2f;

/**
 * add's a vec2f to another vec2f.
 */
vec2f add_vec2f(const vec2f *v, const vec2f *d);

/**
 * add's multiple vec2fs and returns the result
 */
vec2f add_vec2fs(int count, ...);

/**
 * add's a vec2f and a scalar float.
 */
vec2f add_vec2f_float(const vec2f *v, float f);

/**
 * multiplies a vec2f by a scalar float.
 */
vec2f mul_vec2f_float(const vec2f *v, float f);

/**
 * divides a vec2f by a scalar float.
 */
vec2f div_vec2f_float(const vec2f *v, float f);

/**
 * returns magnitude of the provided vec2f
 */
float vec2f_magnitude(const vec2f *p);

/**
 * returns a normalized copy of the provided vec2f
 */
vec2f normalize_vec2f(const vec2f *p);

#endif // _FLOATY_CORE_VEC