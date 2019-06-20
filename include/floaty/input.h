#ifndef _FLOATY_CORE_INPUT
#define _FLOATY_CORE_INPUT

#include <SDL.h>

typedef struct floaty_key_bindings {
  SDL_Scancode move_up;
  SDL_Scancode move_down;
  SDL_Scancode move_left;
  SDL_Scancode move_right;
} floaty_key_bindings;

typedef struct game_button_state {
  SDL_bool is_down;
} game_button_state;

typedef struct game_input {
  game_button_state move_up;
  game_button_state move_down;
  game_button_state move_left;
  game_button_state move_right;
} game_input;

#endif