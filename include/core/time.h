#ifndef _FLOATY_CORE_TIME
#define _FLOATY_CORE_TIME

#include <SDL.h>

unsigned int time_get_frame_milliseconds();
float time_get_frame_seconds();
void time_set_frame(unsigned int ticks);
void time_frame_tick();

#endif