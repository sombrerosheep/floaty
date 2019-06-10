#include <floaty/floaty.h>

#include <graphics/renderer.h>
#include <graphics/color.h>

const color clear_color = { 0x0, 0x0, 0x0, 0xFF };
const floaty_key_bindings key_bindings = {
  SDL_SCANCODE_W,
  SDL_SCANCODE_S,
  SDL_SCANCODE_A,
  SDL_SCANCODE_D
};

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

game_input floaty_input(player *p) {
  game_input input;
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  if (keys[key_bindings.move_up]) {
    input.move_up = (game_button_state){ SDL_TRUE };
  } else {
    input.move_up = (game_button_state){ SDL_FALSE };
  }

  if (keys[key_bindings.move_down]) {
    input.move_down = (game_button_state){ SDL_TRUE };
  } else {
    input.move_down = (game_button_state){ SDL_FALSE };
  }

  if (keys[key_bindings.move_left]) {
    input.move_left = (game_button_state){ SDL_TRUE };
  } else {
    input.move_left = (game_button_state){ SDL_FALSE };
  }

  if (keys[key_bindings.move_right]) {
    input.move_right = (game_button_state){ SDL_TRUE };
  } else {
    input.move_right = (game_button_state){ SDL_FALSE };
  }

  return input;
}

void floaty_update(game_state *state, player *p, const game_input *input) {
  p->velocity = (vec2f){ 0.f, 0.f };
  if (input->move_up.is_down) {
    p->velocity.y = -PLAYER_MOVEMENT_SPEED;
  }
  if (input->move_down.is_down) {
    p->velocity.y = PLAYER_MOVEMENT_SPEED;
  }
  if (input->move_left.is_down) {
    p->velocity.x = -PLAYER_MOVEMENT_SPEED;
  }
  if (input->move_right.is_down) {
    p->velocity.x = PLAYER_MOVEMENT_SPEED;
  }

  state->player_pos = vec2f_add_vec2f(&state->player_pos, &p->velocity);
}

void floaty_draw(const game_state *state, const player *p) {
  render_clear(&clear_color);

  draw_world(state->current_world);
  draw_player(p, &state->player_pos);

  floaty_display();
}