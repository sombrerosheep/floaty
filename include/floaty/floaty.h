#ifndef _FLOATY_FLOATY_FLOATY
#define _FLOATY_FLOATY_FLOATY

#include <core/rectf.h>
#include <core/vec2f.h>
#include <floaty/world.h>
#include <floaty/player.h>
#include <graphics/color.h>

typedef struct game_state {
  vec2f player_pos;
  world *current_world;
} game_state;

void init_floaty(game_state *state, player *p);
void free_floaty(game_state *state, player *p);
void floaty_draw(const game_state *state, const player *p);

#endif