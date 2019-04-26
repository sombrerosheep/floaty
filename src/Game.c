#include <Game.h>

void floaty_handle_input(GameContext *ctx) {
}

void floaty_update(GameContext *ctx) {
}

void floaty_draw(GameContext *ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(ctx->renderer);

  SDL_RenderPresent(ctx->renderer);
}