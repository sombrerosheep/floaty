#include <Player.h>
#include <stdio.h>
#include <core/rectf.h>

#define PLAYER_MOVE_SPEED 3.0f
#define COLLISION_SOLVER_ITERATIONS 3
#define COLLISION_POINTS_PER_DIRECTION 2

enum DIRECTION {
  DIRECTION_TOP = 0,
  DIRECTION_BOTTOM = 1,
  DIRECTION_LEFT = 2,
  DIRECTION_RIGHT = 3
};

void player_move(Player *player, const vec2f *p) {
  player->position = add_vec2f(&player->position, p);
}

void player_handle_input(Player *player, const Uint8 *keys) {
  player->velocity.x = 0;
  player->velocity.y = 0;

  if (keys[SDL_SCANCODE_W]) {
    player->velocity.y = -1.f;
  }

  if (keys[SDL_SCANCODE_A]) {
    player->velocity.x = -1.f;
  }

  if (keys[SDL_SCANCODE_S]) {
    player->velocity.y = 1.f;
  }

  if (keys[SDL_SCANCODE_D]) {
    player->velocity.x = 1.f;
  }

  player->velocity = normalize_vec2f(&player->velocity);
}

vec2f* get_collision_points(vec2f player_size) {
  // dynamically setup collision points for the character based on size.
  // TODO: Move this to new_player
  float p_x_offset = player_size.x / (COLLISION_POINTS_PER_DIRECTION + 1);
  float p_y_offset = player_size.y / (COLLISION_POINTS_PER_DIRECTION + 1);

  vec2f *player_col_points = (vec2f*)SDL_calloc(4 * COLLISION_POINTS_PER_DIRECTION, sizeof(vec2f));

  for (int i = 0; i < COLLISION_POINTS_PER_DIRECTION; i++) {
    player_col_points[DIRECTION_TOP * COLLISION_POINTS_PER_DIRECTION + i] =
      (vec2f){ p_x_offset * (float)i + p_x_offset, 0.f };
    player_col_points[DIRECTION_BOTTOM * COLLISION_POINTS_PER_DIRECTION + i] =
      (vec2f){ p_x_offset * i + p_x_offset, player_size.y - 1.f};
    player_col_points[DIRECTION_LEFT * COLLISION_POINTS_PER_DIRECTION + i] =
      (vec2f){ 0.f, p_y_offset * i + p_y_offset };
    player_col_points[DIRECTION_RIGHT * COLLISION_POINTS_PER_DIRECTION + i] =
      (vec2f){ player_size.x - 1.f, p_y_offset * i + p_y_offset };
  }

  return player_col_points;
}

void handle_world_collisions(Player *player, const World *world) {
  SDL_bool collide_x = SDL_TRUE;
  SDL_bool collide_y_top = SDL_TRUE;
  SDL_bool collide_y_bot = SDL_TRUE;

  vec2f *player_col_points = get_collision_points(player->size);

  for (
    int iteration = 0;
    iteration < COLLISION_SOLVER_ITERATIONS && (collide_x || collide_y_top || collide_y_bot);
    iteration++
  ) {
    // set up movement delta (vel * speed in our case)
    vec2f next_movement_vec = mul_vec2f_float(&player->velocity, PLAYER_MOVE_SPEED);
    vec2f original_movment_vec = next_movement_vec;
    vec2f target_movement_vec = { 0.f, 0.f };

    collide_x = SDL_FALSE;
    collide_y_top = SDL_FALSE;
    collide_y_bot = SDL_FALSE;

    for (
      int geometry = 0;
      geometry < world->geometry.count && !collide_x && !collide_y_top && !collide_y_bot;
      geometry++
    ) {
      for (int dir = 0; dir < 4; dir++) {
        // Skip an iteration if we're not moving that direction
        if (dir == DIRECTION_TOP  && next_movement_vec.y > 0.f) continue;
        if (dir == DIRECTION_BOTTOM && next_movement_vec.y < 0.f) continue;
        if (dir == DIRECTION_LEFT && next_movement_vec.x > 0.f) continue;
        if (dir == DIRECTION_RIGHT && next_movement_vec.x < 0.f) continue;

        if (dir < 2) {
          target_movement_vec.y = next_movement_vec.y;
        } else {
          target_movement_vec.x = next_movement_vec.x;
        }

        while (SDL_TRUE) {
          vec2f col_points[COLLISION_POINTS_PER_DIRECTION];
          // TODO: Make add_vec2f a variadic
          for (int p = 0; p < COLLISION_POINTS_PER_DIRECTION; p++) {
            col_points[p].x = player_col_points[dir * COLLISION_POINTS_PER_DIRECTION + p].x + player->position.x + target_movement_vec.x;
            col_points[p].y = player_col_points[dir * COLLISION_POINTS_PER_DIRECTION + p].y + player->position.y + target_movement_vec.y;
          }

          // TODO: Variadic "if any SDL_Point in SDL_Rect". Then COLLISION_POINTS_PER_DIRECTION
          //       can be changed at any time.
          if (
            vec2f_in_rectf(&col_points[0], &world->geometry.rects[geometry]) ||
            vec2f_in_rectf(&col_points[0], &world->geometry.rects[geometry])
          ) {
            if (dir == DIRECTION_TOP) target_movement_vec.y++;
            if (dir == DIRECTION_BOTTOM) target_movement_vec.y--;
            if (dir == DIRECTION_LEFT) target_movement_vec.x++;
            if (dir == DIRECTION_RIGHT) target_movement_vec.x--;
          // } else if (dir == DIRECTION_RIGHT &&
          //   player->position.x > 290.0 &&
          //   geometry == 3
          // ) {
          //   // 295, 150, 10, 250
          //   printf("You should be colliding\n");
          } else {
            break;
          }
        } // END while (SDL_TRUE)

        // update target_movement_vec for next iteration
        if (dir < 2) {
          next_movement_vec.y = target_movement_vec.y;
        } else {
          next_movement_vec.x = target_movement_vec.x;
        }
      } // END direction loop

      // set detected collision type
      if (next_movement_vec.y > original_movment_vec.y && original_movment_vec.y < 0.f) {
        collide_y_top = SDL_TRUE;
      }
      if (next_movement_vec.y < original_movment_vec.y && original_movment_vec.y > 0.f) {
        collide_y_bot = SDL_TRUE;
      }
      if (SDL_abs(next_movement_vec.x - original_movment_vec.x) > 0) {
        collide_x = SDL_TRUE;
      }
    } // END world geometry loop

    if (collide_y_bot || collide_y_top) {
      player->position.y += next_movement_vec.y;
      player->velocity.y = 0.f;
    }
    if (collide_x) {
      player->position.x += next_movement_vec.x;
      player->velocity.x = 0.f;
    }
  } // END COLLISION_SOLVER_ITERATIONS

  SDL_free(player_col_points);
}

void player_update(Player *player, const World *world) {
  handle_world_collisions(player, world);

  vec2f movement = mul_vec2f_float(&player->velocity, PLAYER_MOVE_SPEED);
  player->move(player, &movement);
}

void player_draw(Player *player, SDL_Renderer *renderer) {
  SDL_Rect rect = {
    (int)player->position.x,
    (int)player->position.y,
    (int)player->size.x,
    (int)player->size.y
  };
  SDL_SetRenderDrawColor(renderer, 0xF1, 0xF1, 0xF1, 0xFF);
  SDL_RenderFillRect(renderer, &rect);
}

Player* new_player(float x, float y) {
  Player *player = (Player*)SDL_calloc(1, sizeof(Player));

  player->handle_input = player_handle_input;
  player->update = player_update;
  player->draw = player_draw;
  player->move = player_move;

  player->position.x = x;
  player->position.y = y;
  player->size.x = 10.f;
  player->size.y = 10.f;

  player->input_state.up = SDL_FALSE;
  player->input_state.down = SDL_FALSE;
  player->input_state.left = SDL_FALSE;
  player->input_state.right = SDL_FALSE;

  return player;
}

void free_player(Player *player) {
  SDL_free(player);
}