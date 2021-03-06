#include <floaty/player.h>
#include <graphics/renderer.h>
#include <graphics/color.h>
#include <core/time.h>

#include <stdbool.h>

const color floaty_player_color = { 0x0, 0x0, 0xFF, 0xFF };
const color floaty_player_adjusted_color = { 0xFF, 0xFF, 0xFF, 0xFF };
const color intersect_color = { 0xFF, 0x0, 0x0, 0x80 };

rectf get_minkowski_rectf(const rectf *a, const rectf *b) {
  rectf rec = {
    a->x - (b->x + b->w),
    a->y - (b->y + b->h),
    a->w + b->w,
    a->h + b->h
  };

  return rec;
}

SDL_bool is_minkowski_rectf_colliding(const rectf *rec) {
  SDL_bool colliding = rec->x <= 0.f &&
                       rec->y <= 0.f &&
                       rec->x + rec->w >= 0.f &&
                       rec->y + rec->h >= 0.f;

  return colliding;
}

vec2f get_minkowski_offset(const rectf *rec) {
  float variant;

  vec2f origin = (vec2f){ 0.f, 0.f };
  vec2f offset = (vec2f){ rec->x, origin.y };
  float min = SDL_fabsf(rec->x - origin.x);

  variant = SDL_fabsf((rec->x + rec->w) - origin.x);
  if (variant < min) {
    min = variant;
    offset.x = rec->x + rec->w;
    offset.y = origin.y;
  }

  variant = SDL_fabsf(rec->y - origin.y);
  if (variant < min) {
    min = variant;
    offset.x = origin.x;
    offset.y = rec->y;
  }

  variant = SDL_fabsf((rec->y + rec->h) - origin.y);
  if (variant < min) {
    min = variant;
    offset.x = origin.x;
    offset.y = rec->y + rec->h;
  }

  return offset;
}

vec2f handle_collisions(player *p, const game_state *state) {
  vec2f result;

  int num_geometry = state->current_world->num_geometry;
  rectf *rects = state->current_world->geometry;
  rectf player_rect = (rectf){ state->player_pos.x, state->player_pos.y, p->rec_size.x, p->rec_size.y };
  
  for (int g = 0; g < num_geometry; g++) {
    rectf min_rect = get_minkowski_rectf(&rects[g], &player_rect);
    if (is_minkowski_rectf_colliding(&min_rect)) {
      vec2f offset = get_minkowski_offset(&min_rect);
      player_rect.x += offset.x;
      player_rect.y += offset.y;
    }
  }

  result = (vec2f){
    player_rect.x - state->player_pos.x,
    player_rect.y - state->player_pos.y
  };
  return result;
}

void update_player(player *p, game_state *state, const game_input *input) {
  float movement_mag;
  vec2f normalized, normalized_movement, position_offset;
  rectf player_rect;

  game_time frame_time = time_get_game_time();

  p->velocity.x = 0.f;
  p->velocity.y = 0.f;
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

  movement_mag = vec2f_magnitude(&p->velocity);
  normalized = vec2f_normalized(&p->velocity);
  normalized_movement = vec2f_mul_float(&normalized, movement_mag);
  state->player_pos = vec2f_add_vec2f(&state->player_pos, &normalized_movement);

  position_offset = handle_collisions(p, state);
  state->player_pos.x = state->player_pos.x + position_offset.x;
  state->player_pos.y = state->player_pos.y + position_offset.y;

  player_rect = (rectf){
    state->player_pos.x,
    state->player_pos.y,
    p->rec_size.x,
    p->rec_size.y
  };
  if (rectf_intersects(&player_rect, &state->current_world->goal)) {
    state->current_world->goal_met = SDL_TRUE;
  } else {
    state->current_world->goal_met = SDL_FALSE;
  }
}

void draw_player(const player *p, const vec2f *pos) {
  rectf rec = {
    pos->x,
    pos->y,
    p->rec_size.x,
    p->rec_size.y
  };

  render_fill_rectf(&rec, &floaty_player_color);
}

void free_player(player *p) {
  SDL_free(p);
}