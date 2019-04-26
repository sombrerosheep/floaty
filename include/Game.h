#ifndef _FLOATY_GAME
#define _FLOATY_GAME

#include <SDL.h>
#include <Player.h>

typedef struct GameContext {
  SDL_Window      *window;
  SDL_Renderer    *renderer;
  Player          *player;
} GameContext;

void floaty_handle_input(GameContext *ctx);
void floaty_update(GameContext *ctx);
void floaty_draw(GameContext *ctx);

#endif // _FLOATY_GAME