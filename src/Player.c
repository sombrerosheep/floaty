#include <Player.h>
#include <stdio.h>

void player_move(Player *player, const vec2f *p) {
  player->position = add_vec2f(&player->position, p);
}

void player_handle_input(Player *player, const Uint8 *keys) {
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

void player_update(Player *player) {
  const float SPEED = 3.f;
  vec2f direction_vec = { 0.f, 0.f };

  if (player->input_state.up) {
    direction_vec.y -= 1;
  }
  if (player->input_state.down) {
    direction_vec.y += 1;
  }
  if (player->input_state.left) {
    direction_vec.x -= 1;
  }
  if (player->input_state.right) {
    direction_vec.x += 1;
  }

  vec2f normalized = normalize_vec2f(&direction_vec);
  vec2f movement = mul_vec2f_float(&normalized, SPEED);

  player->move(player, &movement);
}

void player_draw(Player *player, SDL_Renderer *renderer) {
  SDL_Rect rect = { (int)player->position.x, (int)player->position.y, 10, 10 };
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

  player->input_state.up = SDL_FALSE;
  player->input_state.down = SDL_FALSE;
  player->input_state.left = SDL_FALSE;
  player->input_state.right = SDL_FALSE;

  return player;
}

void free_player(Player *player) {
  SDL_free(player);
}