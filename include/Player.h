#ifndef _PLAYER
#define _PLAYER

#include <SDL.h>
#include <core/vec.h>
#include <World.h>

typedef struct PlayerInputState {
  SDL_bool up;
  SDL_bool down;
  SDL_bool left;
  SDL_bool right;
} PlayerInputState;

typedef struct Player {
  vec2f             position;
  vec2f             size;
  vec2f             velocity;
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
  void (*update)(struct Player *player, const World *world);

  /**
   * Draws the player with the provided renderer
  */
  void (*draw)(struct Player *player, SDL_Renderer *renderer);
} Player;

/**
 * Creates a new player at the given x/y position
 */
Player* new_player(float x, float y);

/**
 * free's memeory used by a player object
 */
void free_player(Player *player);

void raycast(Player *player, World *world, SDL_Renderer *renderer);

#endif // _PLAYER