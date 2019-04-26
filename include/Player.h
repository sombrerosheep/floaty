#ifndef _PLAYER
#define _PLAYER

#include <SDL.h>

typedef struct PlayerInputState {
  SDL_bool up;
  SDL_bool down;
  SDL_bool left;
  SDL_bool right;
} PlayerInputState;

typedef struct Player {
  SDL_Point         position;
  PlayerInputState  input_state;

  /**
   * Move player position by provided x/y offset
  */
  void (*move)(struct Player *player, int x, int y);

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

Player* new_player(int x, int y);
void free_player(Player *player);

#endif // _PLAYER