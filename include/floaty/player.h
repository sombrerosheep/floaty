#ifndef _FLOATY_FLOATY_PLAYER
#define _FLOATY_FLOATY_PLAYER

#include <core/vec2f.h>

#define PLAYER_MOVEMENT_SPEED 3.f

typedef struct player {
  vec2f rec_size;
  vec2f velocity;
} player;

void draw_player(const player *p, const vec2f *pos);
void free_player(player *p);

#endif