#include <core/rectf.h>

SDL_Rect rectf_to_sdl_rect(const rectf *rec) {
  SDL_Rect sdl_rec = {
    (int)rec->x,
    (int)rec->y,
    (int)rec->w,
    (int)rec->h
  };

  return sdl_rec;
}