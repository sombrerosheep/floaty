#include <Player.h>
#include <stdio.h>
#include <core/rectf.h>
#include <Game.h>

#include <raycast.h>

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

//////////////////////////////////////////////////////////////////////////////?
// magnitude    square root of the sums of the squares
// normalize    divide the vector by its magnitude

#define PI   3.14159265358979f
#define radToDeg(x) ((x)*180.f/PI)
#define degToRad(x) ((x)*PI/180.f)

typedef struct linef {
  vec2f start;
  vec2f end;
} linef;

linef* get_segments_from_rect(const rectf *rec) {
  linef* segments = (linef*)SDL_calloc(4, sizeof(linef));

  segments[0] = (linef){ { rec->x, rec->y }, { rec->x + rec->w, rec->y } }; // top side
  segments[1] = (linef){ { rec->x + rec->w, rec->y }, { rec->x + rec->w, rec->y + rec-> h } }; // right side
  segments[2] = (linef){ { rec->x + rec->w, rec->y + rec-> h }, { rec->x, rec->y + rec->h } }; // bottom size
  segments[3] = (linef){ { rec->x, rec->y + rec->h }, { rec->x, rec->y } }; // left side

  return segments;
}

typedef struct intersect {
  vec2f point;
  float t1;
} intersect;

SDL_bool do_segments_intersect(const linef *ray, const linef *segment, intersect *intersection) {
  vec2f ray_direction = sub_vec2f(&ray->start, &ray->end);
  vec2f segment_direction = sub_vec2f(&segment->start, &segment->end);

  //test parallel
  float ray_mag = vec2f_magnitude(&ray_direction);
  float seg_mag = vec2f_magnitude(&segment_direction);

  if (ray_direction.x / ray_mag && segment_direction.x / seg_mag &&
    ray_direction.y / ray_mag && segment_direction.y / seg_mag) {
    return SDL_FALSE;
  }

  // Solve for T2!
  float top = (ray_direction.x * (segment->start.y - ray->start.y) + ray_direction.y * (ray->start.x - segment->start.x));
  float bot = (segment_direction.x * ray_direction.y - segment_direction.y * ray_direction.x);
  float T2 = top/bot;

  // Plug the value of T2 to get T1
  float T1 = (segment->start.x + segment_direction.x * T2 - ray->start.x)
    / ray_direction.x;

  if (T1 < 0) {
    return SDL_FALSE;
  }

  if (T2 < 0 || T2 > 1) {
    return SDL_FALSE;
  }

  intersection->point.x = ray->start.x + ray_direction.x * T1;
  intersection->point.y = ray->start.y + ray_direction.y * T1;
  intersection->t1 = T1;

  return SDL_TRUE;
}

SDL_Rect make_raycast_point(const vec2f *vec) {
  return (SDL_Rect){ vec->x, vec->y, 3, 3 };
}

void cast_rays(Player *player, World *world, SDL_Renderer *renderer) {
  vec2f player_center = {
    player->position.x + player->size.x / 2,
    player->position.y + player->size.y / 2
  };

  int ray_length = 200;
  int ray_count = 30; // ray every 15 degrees
  int deg_per_ray = 360 / ray_count;

  vec2f *ray_coords = (vec2f*)SDL_calloc(ray_count, sizeof(vec2f));
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x0, 0x0, 0xFF);

  for (int i = 0; i < ray_count; i++) {
    int angle = i * deg_per_ray;

    vec2f movement_vec = {
      SDL_cosf(degToRad(angle)),
      SDL_sinf(degToRad(angle))
    };

    vec2f ray_pos = mul_vec2f_float(&movement_vec, ray_length);
    ray_coords[i] = add_vec2f(&player_center, &ray_pos);
    SDL_Rect rec = make_raycast_point(&ray_coords[i]);
    SDL_RenderDrawRect(renderer, &rec);
  }

  SDL_free(ray_coords);
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

void single_ray(Player *player, World *world, SDL_Renderer *renderer) {
  vec2f player_center = {
    player->position.x + player->size.x / 2,
    player->position.y + player->size.y / 2
  };

  // An imaginary length
  int ray_length = 200;
  int ray_count = 50; // ray every 15 degrees
  float deg_per_ray = 360.f / ray_count;

  // the final cooredinates of the ray
  vec2f *ray_coords = (vec2f*)SDL_calloc(ray_count, sizeof(vec2f));

  // loop through each ray we're going to draw
  for (int i = 0; i < ray_count; i++) {
    // get the angle of the ray from player_center
    float angle = i * deg_per_ray;

    // Unit vector of the ray
    vec2f movement_vec = {
      SDL_cosf(degToRad(angle)),
      SDL_sinf(degToRad(angle))
    };

    // length of the ray
    vec2f ray_pos = mul_vec2f_float(&movement_vec, ray_length);
    // offset the ray by the player center
    ray_coords[i] = add_vec2f(&player_center, &ray_pos);
    // placeholder for intersection data
    intersect ray_intersect = { ray_coords[i], -1 };
    SDL_bool collided = SDL_FALSE;

    // Loop through world geometry
    for (int g = 0; g < world->geometry.count; g++) {

      // convert the rect to line segments
      linef* segments = get_segments_from_rect(&world->geometry.rects[g]);

      // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      // draw_line(segments, renderer);

      // each rect will wield 4 line segments.
      for (int s = 0; s < 4; s++) {
        // create a line for our ray; player center to ray_coords
        linef ray_line = { player_center, ray_coords[i]};
        // intersect data for current iteration
        intersect this_intersect = { ray_coords[i], ray_length };

        // Check for an intersection
        if (do_segments_intersect(&ray_line, &segments[s], &this_intersect) && this_intersect.t1 < ray_intersect.t1 || ray_intersect.t1 == -1) {
          collided = SDL_TRUE;
          // If there's an intersection and t1 of this intersection is less than current; store new values
          ray_intersect.point.x = this_intersect.point.x;
          ray_intersect.point.y = this_intersect.point.y;
          ray_intersect.t1 = this_intersect.t1;
        }
      }
    }

    if (collided) {
      // the intersection will have potentially new coords for our ray
      ray_coords[i].x = ray_intersect.point.x;
      ray_coords[i].y = ray_intersect.point.y;
    }

    SDL_Rect rec = make_raycast_point(&ray_coords[i]);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x0, 0x0, 0xFF);
    SDL_RenderFillRect(renderer, &rec);
  }

  draw_lines(renderer, &player_center, ray_coords, ray_count);
  draw_points(renderer, ray_coords, ray_count);
  SDL_free(ray_coords);
}

void raycast(Player *player, World *world, SDL_Renderer *renderer) {
  // cast_rays(player, world, renderer);
  single_ray(player, world, renderer);
}

//////////////////////////////////////////////////////////////////////////////?

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