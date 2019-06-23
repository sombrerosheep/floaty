#include <floaty/floaty.h>

#include <graphics/renderer.h>
#include <graphics/color.h>
#include <core/time.h>
#include <floaty/player.h>

const color clear_color = { 0x0, 0x0, 0x0, 0xFF };
const floaty_key_bindings key_bindings = {
  SDL_SCANCODE_W,
  SDL_SCANCODE_S,
  SDL_SCANCODE_A,
  SDL_SCANCODE_D
};

rectf make_world_rect(int x, int y, int w, int h) {
  float WINDOW_WIDTH = 600.f;
  float WINDOW_HEIGHT = 900.f;
  float bounds_width = 375.f;
  float tiles_width = 10.f;
  float bounds_height = 450.f;
  float tiles_height = 12.f;
  float x_scale = bounds_width / tiles_width;
  float y_scale = bounds_height / tiles_height;
  float x_offset = (WINDOW_WIDTH - bounds_width) / 2;
  float y_offset = (WINDOW_HEIGHT - bounds_height) / 2;
  rectf result;

  result = (rectf){
    x_offset + (x * x_scale),
    y_offset + (y * y_scale),
    w * x_scale,
    h * y_scale
  };

  return result;
}

void init_world_geometry(game_state *state) {
  int num_geometry = 8;
  world *w = SDL_malloc(sizeof(world));
  rectf *rects = SDL_malloc(sizeof(world) * num_geometry);

  rects[0] = make_world_rect(0.f, 4.f, 1.f, 5.f );
  rects[1] = make_world_rect(1.f, 2.f, 8.f, 1.f );
  rects[2] = make_world_rect(7.f, 0.f, 1.f, 9.f );
  rects[3] = make_world_rect(3.f, 6.f, 1.f, 6.f );
  rects[4] = make_world_rect(3.f, 8.f, 2.f, 1.f );
  rects[5] = make_world_rect(0.f, 11.f, 10.f, 1.f );
  rects[6] = make_world_rect(6.f, 10.f, 1.f, 2.f );
  rects[7] = make_world_rect(9.f, 6.f, 1.f, 6.f );

  w->num_geometry = num_geometry;
  w->geometry = rects;
  w-> goal = make_world_rect( 4.f, 10.f, 1.f, 1.f);
  w->goal.x += 10.f;
  w->goal.y -= 10.f;
  w->goal_met = SDL_FALSE;

  state->current_world = w;
}

void load_player(game_state *state, player *p) {
  state->player_pos = (vec2f){ 20.f, 20.f };
  p->rec_size = (vec2f){ 20.f, 20.f };
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
  update_player(p, state, input);
}

void floaty_draw(const game_state *state, const player *p) {
  render_clear(&clear_color);

  draw_world(state->current_world);
  draw_player(p, &state->player_pos);

  floaty_display();
}