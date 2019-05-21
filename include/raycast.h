#include <SDL.h>
#include <core/vec.h>

typedef struct intersect {
  vec2f point;
  float t1;
} intersect;

typedef struct linef {
  vec2f start;
  vec2f end;
} linef;

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

intersect* get_collision_point_on_segment(const linef *ray, const linef *segment) {
  intersect *intersection = (intersect*)SDL_calloc(1, sizeof(intersect));
  if (do_segments_intersect(ray, segment, intersection)) {
    return intersection;
  }

  SDL_free(intersection);
  return NULL;
}

linef* get_segments_from_rectf(const rectf *rec) {
  linef* segments = (linef*)SDL_calloc(4, sizeof(linef));

  segments[0] = (linef){ { rec->x, rec->y }, { rec->x + rec->w, rec->y } }; // top side
  segments[1] = (linef){ { rec->x + rec->w, rec->y }, { rec->x + rec->w, rec->y + rec-> h } }; // right side
  segments[2] = (linef){ { rec->x + rec->w, rec->y + rec-> h }, { rec->x, rec->y + rec->h } }; // bottom size
  segments[3] = (linef){ { rec->x, rec->y + rec->h }, { rec->x, rec->y } }; // left side

  return segments;
}

intersect* get_collision_point_on_rectf(const linef *ray, const rectf *rect) {
  linef *segs = get_segments_from_rectf(rect);
  intersect *collision_point = NULL;

  for (int i = 0; i < 4; i++) {
    intersect *intersection = get_collision_point_on_segment(ray, &segs[i]);

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

  SDL_free(segs);
  return collision_point;
}

linef make_ray_line(float angle, const vec2f *origin) {
  vec2f ray_vec = {
    SDL_cosf(angle),
    SDL_sinf(angle)
  };
  return (linef){
    { origin->x, origin->y },
    { origin->x + ray_vec.x, origin->y + ray_vec.y }
  };
}

void draw_lines(SDL_Renderer *renderer, const vec2f *origin, const vec2f *points, int count) {
    SDL_Point *p = (SDL_Point*)SDL_calloc(count * 2, sizeof(SDL_Point));

    for (int i = 0, c = 0; c < count; i += 2, c++) {
        p[i] = (SDL_Point){ origin->x, origin->y };
        p[i + 1] = (SDL_Point){ points[c].x, points[c].y };
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 40);
    SDL_RenderDrawLines(renderer, p, count * 2);
    SDL_free(p);
}

void draw_points(SDL_Renderer *renderer, const vec2f *points, int count) {
    SDL_Rect *p = (SDL_Rect*)SDL_calloc(count, sizeof(SDL_Rect));

    for (int i = 0; i < count; i++) {
        p[i] = (SDL_Rect){ points[i].x, points[i].y, 2, 2 };
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_RenderFillRects(renderer, p, count);
    SDL_free(p);
}
