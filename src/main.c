#include <stdio.h>
#include <SDL.h>

#include <core/rectf.h>
#include <core/vec2f.h>
#include <graphics/renderer.h>
#include <graphics/color.h>

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

  SDL_Event event;

  rectf rec = {
    40.f, 50.f,
    150.f, 200.f
  };
  vec2f point = { 20.f, 20.f };

  color render_clear_color = { 0x0, 0x0, 0x0, 0xFF };
  color rec_color = { 0xFF, 0x0, 0x0, 0xFF };
  color point_color = { 0x0, 0xFF, 0x0, 0xFF };

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

      render_clear(&render_clear_color);
      render_rectf(&rec, &rec_color);
      render_vec2f(&point, &point_color);
      floaty_display();
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}