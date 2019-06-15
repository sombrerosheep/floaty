#include <floaty/player.h>
#include <floaty/floaty.h>
#include <graphics/renderer.h>
#include <graphics/color.h>

const color floaty_player_color = { 0x0, 0x0, 0xFF, 0xFF };
const color intersect_color = { 0xFF, 0x0, 0x0, 0x80 };

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
  collisions *col_data = (collisions*)SDL_calloc(1, sizeof(collisions));
  col_data->count = num_rectfs;
  rectf *rectf_intersections = (rectf*)SDL_calloc(num_rectfs, sizeof(rectf));
  col_data->intersections = rectf_intersections;
  rectf intersection;

  for (int i = 0, r = 0; i < num_rectfs; i++) {
    if (rectf_intersects_rectf(player_rect, &rectfs[i], &intersection) == SDL_TRUE) {
      col_data->intersections[r] = intersection;
      r++;
    } else {
      col_data->count--;
    }
  }

  return col_data;
}

void free_player(player *p) {
  SDL_free(p);
  free_collisions(p->frame_collisions);
}

void free_collisions(collisions *col) {
  SDL_free(col->intersections);
  SDL_free(col);
}