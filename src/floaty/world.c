#include <floaty/world.h>

#include <graphics/renderer.h>
#include <graphics/color.h>

const color world_geometry_color = { 0x0, 0xFF, 0x0, 0x0 };

void draw_world(const world *w) {
  render_fill_rectfs(w->num_geometry, w->geometry, &world_geometry_color);
}

void free_world(world *w) {
  SDL_free(w->geometry);
  SDL_free(w);
}