#include <Player.h>

void move(Player *player, int x, int y) {
  player->position.x += x;
  player->position.y += y;
}

void draw(Player *player, SDL_Renderer *renderer) {
  SDL_Rect rect = { player->position.x, player->position.y, 10, 10 };
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderFillRect(renderer, &rect);
}

Player* new_player(int x, int y) {
  Player *player = (Player*)SDL_calloc(1, sizeof(Player));

  player->draw = draw;
  player->move = move;

  player->position.x = x;
  player->position.y = y;

  player->input.up = SDL_FALSE;
  player->input.down = SDL_FALSE;
  player->input.left = SDL_FALSE;
  player->input.right = SDL_FALSE;

  return player;
}

void free_player(Player *player) {
  SDL_free(player);
}