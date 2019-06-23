#include <floaty/player.h>
#include <floaty/floaty.h>
#include <graphics/renderer.h>
#include <graphics/color.h>
#include <core/time.h>

const color floaty_player_color = { 0x0, 0x0, 0xFF, 0xFF };
const color floaty_player_adjusted_color = { 0xFF, 0xFF, 0xFF, 0xFF };
const color intersect_color = { 0xFF, 0x0, 0x0, 0x80 };

void set_collisions(player *p, const game_state *state) {
  rectf player_rect = { state->player_pos.x, state->player_pos.y, p->rec_size.x, p->rec_size.y };
  free_collisions(p->frame_collisions);
  p->frame_collisions = player_collisions_with_rectfs(&player_rect, state->current_world->num_geometry, state->current_world->geometry);
}

void update_player(player *p, game_state *state, const game_input *input) {
  game_time frame_time = time_get_game_time();

  p->velocity = (vec2f){ 0.f, 0.f };
  if (input->move_up.is_down) {
    p->velocity.y = -PLAYER_MOVEMENT_SPEED * frame_time.seconds;
  }
  if (input->move_down.is_down) {
    p->velocity.y = PLAYER_MOVEMENT_SPEED * frame_time.seconds;
  }
  if (input->move_left.is_down) {
    p->velocity.x = -PLAYER_MOVEMENT_SPEED * frame_time.seconds;
  }
  if (input->move_right.is_down) {
    p->velocity.x = PLAYER_MOVEMENT_SPEED * frame_time.seconds;
  }

  float movement_mag = vec2f_magnitude(&p->velocity);
  vec2f normalized = vec2f_normalized(&p->velocity);
  vec2f normalized_movement = vec2f_mul_float(&normalized, movement_mag);
  state->player_pos = vec2f_add_vec2f(&state->player_pos, &normalized_movement);

  set_collisions(p, state);
}

void draw_player(const player *p, const vec2f *pos) {
  rectf rec = {
    pos->x,
    pos->y,
    p->rec_size.x,
    p->rec_size.y
  };

  render_rectf(&rec, &floaty_player_color);
  draw_collisions(p->frame_collisions);
}

void draw_collisions(const collisions *col) {
  render_rectfs(col->count, col->intersections, &intersect_color);
}

collisions* player_collisions_with_rectfs(const rectf *player_rect, int num_rectfs, const rectf *rectfs) {
  collisions *col_data = SDL_calloc(1, sizeof(collisions));
  col_data->count = num_rectfs;
  rectf *rectf_intersections = SDL_calloc(num_rectfs, sizeof(rectf));
  col_data->intersections = rectf_intersections;
  rectf intersection;

  for (int i = 0, r = 0; i < num_rectfs; i++) {
    if (rectf_intersection(player_rect, &rectfs[i], &intersection) == SDL_TRUE) {
      col_data->intersections[r] = intersection;
      r++;
    } else {
      col_data->count--;
    }
  }

  return col_data;
}

void free_player(player *p) {
  free_collisions(p->frame_collisions);
  SDL_free(p);
}

void free_collisions(collisions *col) {
  SDL_free(col->intersections);
  SDL_free(col);
}