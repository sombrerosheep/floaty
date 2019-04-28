#include <Game.h>

void floaty_handle_input(GameContext *ctx) {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  ctx->player->handle_input(ctx->player, keys);
}

void floaty_update(GameContext *ctx) {
  ctx->player->update(ctx->player, ctx->world);
}

void floaty_draw(GameContext *ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(ctx->renderer);

  ctx->world->draw(ctx->world, ctx->renderer);
  ctx->player->draw(ctx->player, ctx->renderer);

  SDL_RenderPresent(ctx->renderer);
}