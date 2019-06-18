#ifndef _FLOATY_GRAPHICS_RENDERER
#define _FLOATY_GRAPHICS_RENDERER

#include <SDL.h>
#include <graphics/color.h>
#include <core/rectf.h>
#include <core/vec2f.h>

void set_floaty_renderer(SDL_Renderer *renderer);
void render_clear(const color *c);
void floaty_display();
void release_renderer();

void render_rectf(const rectf *rect, const color *c);
void render_rectfs(int count, const rectf *rect, const color *c);
void render_vec2f(const vec2f *vec, const color *c);

#endif