#include <core/time.h>

#include <stdio.h>

static unsigned int this_frame_ticks;
static unsigned int last_frame_ticks;

static unsigned int get_this_frame_ticks() {
  return this_frame_ticks;
}

static void set_this_frame_ticks(unsigned int ticks) {
  this_frame_ticks = ticks;
}

static unsigned int get_last_frame_ticks() {
  return last_frame_ticks;
}

static void set_last_frame_ticks(unsigned int ticks) {
  last_frame_ticks = ticks;
}

game_time time_get_game_time() {
  game_time g;
  g.milliseconds = get_this_frame_ticks() - get_last_frame_ticks();;

  if (g.milliseconds < 1) {
    g.seconds = 0.f;
  } else {
    g.seconds = 1.f / (float)g.milliseconds;
  }

  return g;
}

void time_set_frame(unsigned int ticks) {
  set_last_frame_ticks(ticks);
}

void time_frame_tick() {
  set_last_frame_ticks(get_this_frame_ticks());
  set_this_frame_ticks(SDL_GetTicks());
}