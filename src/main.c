#include <stdio.h>
#include <SDL.h>

#include <Game.h>
#include <core/rectf.h>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 900;

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Error initializing SDL:\n%s\n", SDL_GetError());
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow(
    "floaty",
    200,
    100,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_OPENGL
  );

  if (window == NULL) {
    printf("Unable to create SDL_Window:\n%s\n", SDL_GetError());
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (renderer == NULL) {
    printf("Unable to create renderer:\n%s\n", SDL_GetError());
    return -1;
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  rectf r[] = {
    { 145.f, 350.f, 300.f, 10.f },
    { 170.f, 325.f, 10.f, 350.f },
    { 420.f, 300.f, 10.f, 350.f },
    { 295.f, 150.f, 10.f, 250.f },
    // { 145.f, 650.f, 300.f, 10.f },
    { 0.f, 0.f, (float)WINDOW_WIDTH, 10.f }, // bounds top
    { 0.f, 890.f, (float)WINDOW_WIDTH, 10.f }, // bounds bot
    { 0.f, 0.f, 10.f, (float)WINDOW_HEIGHT }, // bounds left
    { (float)WINDOW_WIDTH - 10.f, 0.f, 10.f, (float)WINDOW_HEIGHT }, // bounds left
  };
  World *world = new_world(r, 8);
  Player *player = new_player(10.f, 10.f);
  // player->position.x = 300.f;
  player->position.x = 227.f;
  // player->position.y = 435.f;
  player->position.y = 385.f;
  GameContext context = { window, renderer, player, world };

  SDL_Event event;

  while (SDL_TRUE) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }

      if (event.type == SDL_KEYUP
        && event.key.keysym.sym == SDLK_ESCAPE) {
        break;
      }
    }

    floaty_handle_input(&context);
    floaty_update(&context);
    floaty_draw(&context);
  }

  free_player(player);
  free_world(world);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}