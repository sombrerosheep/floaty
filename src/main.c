#include <stdio.h>
#include <SDL.h>

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
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}