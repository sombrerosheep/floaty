#include <Game.h>

void floaty_handle_input(GameContext *ctx) {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  ctx->player->handle_input(ctx->player, keys);
}

void floaty_update(GameContext *ctx) {
  ctx->player->update(ctx->player);
}

void floaty_draw(GameContext *ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(ctx->renderer);

  SDL_SetRenderDrawColor(ctx->renderer, 0xF1, 0xF1, 0xF1, 0xFF);
  ctx->player->draw(ctx->player, ctx->renderer);

  SDL_RenderPresent(ctx->renderer);
}