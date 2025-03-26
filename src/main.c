#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "font.h"
#include "vector.h"
#include "rope.h"

#define BUFFER_CAPACITY 1024
#define FONT_SIZE 24

char buffer[BUFFER_CAPACITY];
size_t buffer_cursor = 0;
size_t buffer_size = 0;

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    if (!init_font_system()) {
        SDL_Quit();
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Zet", 
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        close_font_system();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        close_font_system();
        SDL_Quit();
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("./assets/font/IosevkaNerdFontMono-Regular.ttf", FONT_SIZE);
    if (!font) {
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        close_font_system();
        SDL_Quit();
        return 1;
    }
    SDL_Color textColor = {255, 255, 255, 255};
    bool quit = false;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
                
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (buffer_size > 0) {
                        buffer_size--;
                        buffer[buffer_size] = '\0';
                        buffer_cursor = buffer_size;
                    }
                }
                break;
                
            case SDL_TEXTINPUT:
                if (buffer_size < BUFFER_CAPACITY - 1) {
                    buffer[buffer_size] = event.text.text[0];
                    buffer_size++;
                    buffer[buffer_size] = '\0';
                    buffer_cursor = buffer_size;
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect textPosition = {0, 0, 0, 0};
        render_text_ttf(renderer, font, buffer, textColor, &textPosition);

        int cursor_x = 0;
        char temp_buffer[BUFFER_CAPACITY];
        strncpy(temp_buffer, buffer, buffer_cursor);
        temp_buffer[buffer_cursor] = '\0';

        SDL_Surface* surface = TTF_RenderText_Solid(font, temp_buffer, textColor);
        if (surface) {
            cursor_x = surface->w;
            SDL_FreeSurface(surface);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect cursorRect = {
            cursor_x,
            0,  
            2,
            TTF_FontHeight(font)
        };
        SDL_RenderFillRect(renderer, &cursorRect);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    close_font_system();
    SDL_Quit();
    return 0;
}
