#include <floaty/player.h>
#include <floaty/floaty.h>
#include <graphics/renderer.h>
#include <graphics/color.h>

const color floaty_player_color = { 0x0, 0x0, 0xFF, 0xFF };

void draw_player(const player *p, const game_state *state) {
  rectf rec = {
    state->player_pos.x,
    state->player_pos.y,
    p->rec_size.x,
    p->rec_size.y
  };

  render_rectf(&rec, &floaty_player_color);
}

void free_player(player *p) {
  SDL_free(p);
}