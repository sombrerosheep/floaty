#ifndef _FLOATY_FLOATY_FLOATY
#define _FLOATY_FLOATY_FLOATY

#include <core/rectf.h>
#include <core/vec2f.h>
#include <graphics/color.h>

typedef struct game_state {
  color *rec_color;
  color *point_color;
  rectf *rec;
  vec2f *point;
} game_state;

void init_floaty(game_state *state);
void free_floaty(game_state *state);
void floaty_draw(const game_state *state);

#endif