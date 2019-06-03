#include <floaty/floaty.h>

#include <graphics/renderer.h>
#include <graphics/color.h>

const color clear_color = { 0x0, 0x0, 0x0, 0xFF };

void load_rect(game_state *state) {
  color* rec_color = (color*)SDL_malloc(sizeof(color));
  rectf* rec = (rectf*)SDL_malloc(sizeof(rectf));

  rec_color->r = 0xFF;
  rec_color->g = 0x0;
  rec_color->b = 0x0;
  rec_color->a = 0xFF;
  rec->x = 40.f;
  rec->y = 50.f;
  rec->w = 150.f;
  rec->h = 200.f;

  state->rec_color = rec_color;
  state->rec = rec;
}

void load_point(game_state *state) {
  color* vec_color = (color*)SDL_malloc(sizeof(color));
  vec2f* vec = (vec2f*)SDL_malloc(sizeof(vec2f));

  vec_color->r = 0x0;
  vec_color->g = 0xFF;
  vec_color->b = 0x0;
  vec_color->a = 0xFF;
  vec->x = 20.f;
  vec->y = 20.f;
  
  state->point_color = vec_color;
  state->point = vec;
}

void init_floaty(game_state *state) {
  load_rect(state);
  load_point(state);
}

void free_floaty(game_state *state) {
  SDL_free(state->rec_color);
  SDL_free(state->rec);
  SDL_free(state->point_color);
  SDL_free(state->point);
  
  SDL_free(state);
}

void floaty_draw(const game_state *state) {
  render_clear(&clear_color);

  render_rectf(state->rec, state->rec_color);
  render_vec2f(state->point, state->point_color);

  floaty_display();
}