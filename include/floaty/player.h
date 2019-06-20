#ifndef _FLOATY_FLOATY_PLAYER
#define _FLOATY_FLOATY_PLAYER

#include <core/vec2f.h>
#include <core/rectf.h>
#include <floaty/input.h>
#include <floaty/floaty.h>

#define PLAYER_MOVEMENT_SPEED 30.f

typedef struct collisions {
  int count;
  rectf *intersections;
} collisions;

typedef struct player {
  vec2f rec_size;
  vec2f velocity;
  collisions *frame_collisions;
} player;

collisions* player_collisions_with_rectfs(const rectf *player_rect, int num_rectfs, const rectf *rectfs);
void draw_player(const player *p, const vec2f *pos);
void draw_collisions(const collisions *col);
void update_player(player *p, game_state *state, const game_input *input);
void free_player(player *p);
void free_collisions(collisions *col);

#endif