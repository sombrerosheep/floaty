#include <SDL.h>
#include <core/vec.h>

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