#include <Player.h>
#include <stdio.h>
#include <core/rectf.h>
#include <Game.h>

#include <raycast.h>

#define PLAYER_MOVE_SPEED 3.0f
#define COLLISION_SOLVER_ITERATIONS 3
#define COLLISION_POINTS_PER_DIRECTION 2

enum RECT_POINT {
  RECT_POINT_UL = 0,
  RECT_POINT_UR = 1,
  RECT_POINT_LL = 2,
  RECT_POINT_LR = 3
};

enum DIRECTION {
  DIRECTION_TOP     = 0,
  DIRECTION_BOTTOM  = 1,
  DIRECTION_LEFT    = 2,
  DIRECTION_RIGHT   = 3
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

SDL_Rect make_point_at_vec2f(const vec2f *vec) {
  return (SDL_Rect){ vec->x, vec->y, 3, 3 };
}

void draw_line(const linef *line, SDL_Renderer *renderer) {
  SDL_RenderDrawLine(
    renderer,
    line->start.x,
    line->start.y,
    line->end.x,
    line->end.y
  );
}

vec2f get_collision_point_on_world_geometry(const linef *ray, const rectf *rects, int num_rects) {
  intersect *collision_point = NULL;

  for (int r = 0; r < num_rects; r++) {
    intersect* intersection = get_collision_point_on_rectf(ray, &rects[r]);

    if (intersection != NULL) {
      if (collision_point == NULL) {
        collision_point = (intersect*)SDL_calloc(1, sizeof(intersect));
        *collision_point = *intersection;
      }

      if (intersection->t1 < collision_point->t1) {
        *collision_point = *intersection;
      }
    }

    SDL_free(intersection);
  }

  if (collision_point == NULL) {
    return ray->end;
  }
  vec2f result = { collision_point->point.x, collision_point->point.y };
  SDL_free(collision_point);
  return result;
}

// casts rays at each unique point in the world geometry and
// captures each collision for a proper visibility polygon.
void unique_rays(Player *player, World * world, SDL_Renderer *renderer) {
  vec2f player_center = {
    player->position.x + player->size.x / 2,
    player->position.y + player->size.y / 2
  };
  
  // we'll check against each unique point in the world
  // each piece of world geometry is a rect which has 4 points
  int num_angle_points = world->geometry.count * 4;
  vec2f *angle_points = (vec2f*)SDL_calloc(num_angle_points, sizeof (vec2f));

  // We'll cast 3 rays for each unique point.
  int num_hit_coords = num_angle_points * 3;
  vec2f *hit_coords = (vec2f*)SDL_calloc(num_hit_coords, sizeof(vec2f));
  float angle_diff = 0.0001f;

  // get all the points
  for (int i = 0, g = 0; g < world->geometry.count; i += 4, g++) {
    linef* world_segments = get_segments_from_rectf(&world->geometry.rects[g]);
    angle_points[i] = world_segments[0].end;
    angle_points[i + 1] = world_segments[1].end;
    angle_points[i + 2] = world_segments[2].end;
    angle_points[i + 3] = world_segments[3].end;

    SDL_free(world_segments);
  }

  draw_points(renderer, angle_points, num_angle_points);

  for (int i = 0, h = 0; i < num_angle_points; i++, h += 3) {
    vec2f diff = sub_vec2f(&player_center, &angle_points[i]);
    float angle = SDL_atan2f(diff.y, diff.x);
    float angle_plus = angle + angle_diff;
    float angle_minus = angle - angle_diff;
    linef ray = make_ray_line(angle, &player_center);
    linef ray_plus = make_ray_line(angle_plus, &player_center);
    linef ray_minus = make_ray_line(angle_minus, &player_center);

    // get collision point for each angle
    // hit coords are returning the point they're aiming at,
    // and not the point they first collide with something.
    hit_coords[h] = get_collision_point_on_world_geometry(&ray, world->geometry.rects, world->geometry.count);
    hit_coords[h + 1] = get_collision_point_on_world_geometry(&ray_plus, world->geometry.rects, world->geometry.count);
    hit_coords[h + 2] = get_collision_point_on_world_geometry(&ray_minus, world->geometry.rects, world->geometry.count);

  
    draw_lines(renderer, &player_center, &hit_coords[h], 3);
    draw_points(renderer, &hit_coords[h], 3);
  }

  SDL_free(angle_points);
  SDL_free(hit_coords);
}

float print_fps() {
  static unsigned int last_ticks = 0;
  unsigned int this_ticks = SDL_GetTicks();
  unsigned int frame_ticks = this_ticks - last_ticks;
  float frame_ms = frame_ticks / 1000.f;

  if (frame_ticks != last_ticks) {
    printf("FPS: %f\n\n", 1.f / frame_ms);
  }

  last_ticks = this_ticks;
}

void raycast(Player *player, World *world, SDL_Renderer *renderer) {
  print_fps();
  unique_rays(player, world, renderer);
}

void handle_world_collisions2(Player *player, const World *world) {
  if (SDL_fabsf(player->velocity.x) < 0.001 && SDL_fabsf(player->velocity.y) < 0.001) {
    return;
  }

  vec2f adjusted_velocity = player->velocity;
  
  vec2f frame_dependent_movement = mul_vec2f_float(&player->velocity, PLAYER_MOVE_SPEED);
  // vec2f next_movement_vec = mul_vec2f_float(&frame_dependent_movement, get_delta_time());
  vec2f next_movment_vec = frame_dependent_movement;
  vec2f next_point = {
    player->position.x + next_movment_vec.x,
    player->position.y + next_movment_vec.y
  };
  float collision_width = SDL_fabsf(next_point.x - player->position.x) + player->size.x;
  float collision_height = SDL_fabsf(next_point.y - player->position.y) + player->size.y;

  rectf movement_box = {
    SDL_min(player->position.x, next_point.x),
    SDL_min(player->position.y, next_point.y),
    collision_width,
    collision_height
  };

  for (int geometry = 0; geometry < world->geometry.count; geometry++) {
    if (rectf_intersects(&movement_box, &world->geometry.rects[geometry])) {
      // now do the raycast
      linef *segments = get_segments_from_rectf(&world->geometry.rects[geometry]);
      vec2f points[4] = {
        { player->position.x, player->position.y },                                       // RECT_POINT_UL
        { player->position.x + player->size.x - 1, player->position.y },                  // RECT_POINT_UR
        { player->position.x, player->position.y + player->size.y },                      // RECT_POINT_LL
        { player->position.x + player->size.x - 1, player->position.y + player->size.y }  // RECT_POINT_LR
      };
      intersect collision_points[4];

      // get the closest collision point for each of the points on the player
      // get the smallest of that and apply offset from that points reference
      
      // loop points on player
      for (int p = 0; p < 4; p++) {
        intersect intersection = { { 0, 0 }, -1 };

        // loop segments on collided geometry
        for (int s = 0; s < 4; s++) {
          intersect iter_intersection = {{0.f, 0.f}, 0.f };
          linef point_ray = {
            points[p],
            add_vec2f(&points[p], &next_movment_vec)
          };

          // check intersection
          if (do_segments_intersect(&point_ray, &segments[s], &iter_intersection)) {
            // update intersection
            if (intersection.t1 < 0 || iter_intersection.t1 < intersection.t1) {
              intersection = iter_intersection;
            }
          }
        }

        // set our final intersection
        collision_points[p] = intersection;
      }

      SDL_free(segments);

      // see which intersection is closest and set that offset as our velocity
      int closest = -1;
      for (int p = 0; p < 4; p++) {
        // if closest is < 0 then its unitialized
        // if collision_points[p].t1 < 0 its uninitialized
        if (collision_points[p].t1 < 0) continue;
        if (closest < 0 || collision_points[p].t1 < collision_points[closest].t1) {
          closest = p;
        }
      }

      if (closest < 0) break; // there was no hit...why are we here?
      player->velocity = sub_vec2f(&points[closest], &collision_points[closest].point);
    }
  }
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
    vec2f frame_dependent_movement = mul_vec2f_float(&player->velocity, PLAYER_MOVE_SPEED);
    // vec2f next_movement_vec = mul_vec2f_float(&frame_dependent_movement, get_delta_time());
    vec2f next_movement_vec = frame_dependent_movement;
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
          // TODO: Make ray_directiond_vec2f ray variray_directionic
          for (int p = 0; p < COLLISION_POINTS_PER_DIRECTION; p++) {
            col_points[p] = add_vec2fs(3, player_col_points[dir * COLLISION_POINTS_PER_DIRECTION + p], player->position, target_movement_vec);
          }

          // TODO: Variray_directionic "if any SDL_Point in SDL_Rect". Then COLLISION_POINTS_PER_DIRECTION
          //       can be changed at any time.
          if (
            vec2f_in_rectf(&col_points[0], &world->geometry.rects[geometry]) ||
            vec2f_in_rectf(&col_points[0], &world->geometry.rects[geometry])
          ) {
            if (dir == DIRECTION_TOP) target_movement_vec.y++;
            if (dir == DIRECTION_BOTTOM) target_movement_vec.y--;
            if (dir == DIRECTION_LEFT) target_movement_vec.x++;
            if (dir == DIRECTION_RIGHT) target_movement_vec.x--;
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
  // handle_world_collisions(player, world);
  handle_world_collisions2(player, world);

  player->move(player, &player->velocity);
  // vec2f movement = mul_vec2f_float(&player->velocity, PLAYER_MOVE_SPEED);
  // player->move(player, &movement);
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