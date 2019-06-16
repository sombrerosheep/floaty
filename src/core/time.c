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

unsigned int time_get_frame_milliseconds() {
  return get_this_frame_ticks() - get_last_frame_ticks();
}

float time_get_frame_seconds() {
  unsigned int frame_milliseconds = time_get_frame_milliseconds();

  if (frame_milliseconds < 1) {
    return 0.f;
  }

  float result = 1.f / (float)frame_milliseconds;
  return result;
}

void time_set_frame(unsigned int ticks) {
  set_last_frame_ticks(ticks);
}

void time_frame_tick() {
  set_last_frame_ticks(get_this_frame_ticks());
  set_this_frame_ticks(SDL_GetTicks());
}