#include "font.h"
#include <stdio.h>
#include <string.h>

bool init_font_system() {
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void close_font_system() {
    TTF_Quit();
}

void render_text_ttf(SDL_Renderer *renderer, TTF_Font *font, const char *text, 
                     SDL_Color color, SDL_Rect *position) {
    if (!renderer || !font || !text || strlen(text) == 0) {
        return;
    }

    if (strspn(text, " \t\n\r") == strlen(text)) {
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) {
        fprintf(stderr, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_QueryTexture(texture, NULL, NULL, &position->w, &position->h);
    SDL_RenderCopy(renderer, texture, NULL, position);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
