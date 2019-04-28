#ifndef _PLAYER
#define _PLAYER

#include <SDL.h>
#include <core/vec.h>

typedef struct PlayerInputState {
  SDL_bool up;
  SDL_bool down;
  SDL_bool left;
  SDL_bool right;
} PlayerInputState;

typedef struct Player {
  vec2f             position;
  PlayerInputState  input_state;

  /**
   * Move player position by provided x/y offset
  */
  void (*move)(struct Player *player, const vec2f *p);

  /**
   * Handles player input
  */
  void (*handle_input)(struct Player *player, const Uint8 *keys);

  /**
  * Update player logic
  */
  void (*update)(struct Player *player);

  /**
   * Draws the player with the provided renderer
  */
  void (*draw)(struct Player *player, SDL_Renderer *renderer);
} Player;

Player* new_player(float x, float y);
void free_player(Player *player);

#endif // _PLAYER