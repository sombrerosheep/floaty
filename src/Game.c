#include <Game.h>

unsigned int get_delta_ticks(void) {
  static float last_frame_ticks = -1;
  if (last_frame_ticks == -1) {
    last_frame_ticks = SDL_GetTicks();
  }

  float this_frame_ticks = SDL_GetTicks();
  float delta = this_frame_ticks - last_frame_ticks;
  last_frame_ticks = this_frame_ticks;
  return delta;
}

float get_delta_time(void) {
  return get_delta_ticks() / 1000.f;
}

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

  raycast(ctx->player, ctx->world, ctx->renderer);
  ctx->player->draw(ctx->player, ctx->renderer);

  SDL_RenderPresent(ctx->renderer);
}