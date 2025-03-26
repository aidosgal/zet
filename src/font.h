#ifndef FONT_H
#define FONT_H

#include <SDL_ttf.h>
#include <SDL.h>
#include <stdbool.h>

bool init_font_system();

void close_font_system();

void render_text_ttf(SDL_Renderer *renderer, TTF_Font *font, const char *text, 
                     SDL_Color color, SDL_Rect *position);

#endif // FONT_H
