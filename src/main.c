#include <stdio.h>
#include <SDL.h>

#include <graphics/renderer.h>
#include <floaty/floaty.h>
#include <floaty/player.h>
#include <core/time.h>

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

  set_floaty_renderer(renderer);
  game_state *floaty_game_state = (game_state*)SDL_calloc(1, sizeof(game_state));
  player *floaty_player = (player*)SDL_calloc(1, sizeof(player));
  floaty_player->frame_collisions = (collisions*)SDL_malloc(sizeof(collisions));
  floaty_player->frame_collisions->count = 0;
  floaty_player->frame_collisions->intersections = NULL;
  init_floaty(floaty_game_state, floaty_player);

  SDL_Event event;
  time_frame_tick();

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

    game_input input = floaty_input(floaty_player);
    floaty_update(floaty_game_state, floaty_player, &input);
    floaty_draw(floaty_game_state, floaty_player);
    time_frame_tick();
  }

  free_floaty(floaty_game_state, floaty_player);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}