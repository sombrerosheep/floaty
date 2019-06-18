#ifndef _FLOATY_CORE_TIME
#define _FLOATY_CORE_TIME

#include <SDL.h>

typedef struct game_time {
  unsigned int milliseconds;
  float seconds;
} game_time;

game_time time_get_game_time();
void time_set_frame(unsigned int ticks);
void time_frame_tick();

#endif