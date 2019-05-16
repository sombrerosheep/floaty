#ifndef _FLOATY_GAME
#define _FLOATY_GAME

#include <SDL.h>
#include <Player.h>
#include <World.h>

typedef struct GameContext {
  SDL_Window      *window;
  SDL_Renderer    *renderer;
  Player          *player;
  World           *world;
} GameContext;

void floaty_handle_input(GameContext *ctx);
void floaty_update(GameContext *ctx);
void floaty_draw(GameContext *ctx);

unsigned int get_delta_ticks(void);
float get_delta_time(void);

#endif // _FLOATY_GAME