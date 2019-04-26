#include <Player.h>

void move(Player *player, int x, int y) {
  player->position.x += x;
  player->position.y += y;
}

void handle_input(Player *player, const Uint8 *keys) {
  if (keys[SDL_SCANCODE_W]) {
    player->input_state.up = SDL_TRUE;
  } else {
    player->input_state.up = SDL_FALSE;
  }

  if (keys[SDL_SCANCODE_A]) {
    player->input_state.left = SDL_TRUE;
  } else {
    player->input_state.left = SDL_FALSE;
  }

  if (keys[SDL_SCANCODE_S]) {
    player->input_state.down = SDL_TRUE;
  } else {
    player->input_state.down = SDL_FALSE;
  }

  if (keys[SDL_SCANCODE_D]) {
    player->input_state.right = SDL_TRUE;
  } else  {
    player->input_state.right = SDL_FALSE;
  }
}

void update(Player *player) {
  const int SPEED = 3;
  SDL_Point movement = { 0, 0 };

  if (player->input_state.up) {
    movement.y -= SPEED;
  }
  if (player->input_state.down) {
    movement.y += SPEED;
  }
  if (player->input_state.left) {
    movement.x -= SPEED;
  }
  if (player->input_state.right) {
    movement.x += SPEED;
  }

  player->move(player, movement.x, movement.y);
}

void draw(Player *player, SDL_Renderer *renderer) {
  SDL_Rect rect = { player->position.x, player->position.y, 10, 10 };
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderFillRect(renderer, &rect);
}

Player* new_player(int x, int y) {
  Player *player = (Player*)SDL_calloc(1, sizeof(Player));

  player->handle_input = handle_input;
  player->update = update;
  player->draw = draw;
  player->move = move;

  player->position.x = x;
  player->position.y = y;

  player->input_state.up = SDL_FALSE;
  player->input_state.down = SDL_FALSE;
  player->input_state.left = SDL_FALSE;
  player->input_state.right = SDL_FALSE;

  return player;
}

void free_player(Player *player) {
  SDL_free(player);
}