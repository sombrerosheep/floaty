#include <floaty/floaty.h>

#include <graphics/renderer.h>
#include <graphics/color.h>

const color clear_color = { 0x0, 0x0, 0x0, 0xFF };

void init_world_geometry(game_state *state) {
  int num_geometry = 8;
  world *w = (world*)SDL_malloc(sizeof(world));
  rectf *rects = (rectf*)SDL_malloc(sizeof(world) * num_geometry);

  rects[0] = (rectf){ 200.f, 360.f, 20.f, 100.f };
  rects[1] = (rectf){ 220.f, 320.f, 160.f, 20.f };
  rects[2] = (rectf){ 340.f, 280.f, 20.f, 180.f };
  rects[3] = (rectf){ 260.f, 400.f, 20.f, 120.f };
  rects[4] = (rectf){ 260.f, 440.f, 40.f, 20.f };
  rects[5] = (rectf){ 200.f, 500.f, 200.f, 20.f };
  rects[6] = (rectf){ 320.f, 480.f, 20.f, 40.f };
  rects[7] = (rectf){ 380.f, 400.f, 20.f, 120.f };

  w->num_geometry = num_geometry;
  w->geometry = rects;
  state->current_world = w;
}

void load_player(game_state *state, player *p) {
  state->player_pos = (vec2f){ 20.f, 20.f };
  p->rec_size = (vec2f){ 10.f, 10.f };
}

void init_floaty(game_state *state, player *p) {
  init_world_geometry(state);
  load_player(state, p);
}

void free_floaty(game_state *state, player *p) {
  free_world(state->current_world);
  free_player(p);
  
  SDL_free(state);
}

void floaty_draw(const game_state *state, const player *p) {
  render_clear(&clear_color);

  draw_world(state->current_world);
  draw_player(p, &state->player_pos);

  floaty_display();
}