#include <floaty/world.h>

#include <graphics/renderer.h>
#include <graphics/color.h>

const color world_geometry_color = { 0xAF, 0x0, 0x0, 0xFF };
const color goal_color = { 0x08, 0xAF, 0x8, 0xFF };

void draw_world(const world *w) {
  if (w->goal_met) {
    render_fill_rectfs(w->num_geometry, w->geometry, &goal_color);
  } else {
    render_fill_rectfs(w->num_geometry, w->geometry, &world_geometry_color);
  }
  render_fill_rectf(&w->goal, &goal_color);
}

void free_world(world *w) {
  SDL_free(w->geometry);
  SDL_free(w);
}