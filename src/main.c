#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

void scc(int code) {
    if (code > 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void *scp(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}

SDL_Surface *surface_from_file(const char *file_path) {
    int w, h, n;
    unsigned char *pixels = stbi_load(file_path, &w, &h, &n, STBI_rgb_alpha);
    if (pixels == NULL) {
        fprintf(stderr, "ERROR: could not load file %s: %s\n", 
                file_path, SDL_GetError());
        exit(1);
    }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 rmask = 0xff000000;
    const Uint32 gmask = 0x00ff0000;
    const Uint32 bmask = 0x0000ff00;
    const Uint32 amask = 0x000000ff;
#else 
    const Uint32 rmask = 0x000000ff;
    const Uint32 gmask = 0x0000ff00;
    const Uint32 bmask = 0x00ff0000;
    const Uint32 amask = 0xff000000;
#endif

    int depth = 32;
    int pitch = 4*w;
    
    return scp(SDL_CreateRGBSurfaceFrom((void*)pixels, w, h, depth, pitch, rmask, gmask, bmask, amask));
}

int main(void) {
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *window = 
        scp(SDL_CreateWindow("Zet", 0, 0, 800, 600, 
                SDL_WINDOW_RESIZABLE));

    SDL_Renderer *renderer = 
        scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
    
    SDL_Surface *font_surface = surface_from_file("./assets/charmap-oldschool_white.png");

    SDL_Texture *font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);

    SDL_Rect font_rect = {
        .x = 0,
        .y = 0,
        .w = font_surface->w,
        .h = font_surface->h,
    };

    bool quit = false;
    while(!quit) {
        SDL_Event event = {0};
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                quit = true;
            } break;
            }
        }

        scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
        scc(SDL_RenderClear(renderer));

        SDL_Rect output_rect = {
            .x = 0,
            .y = 0,
            .w = font_surface->w * 5,
            .h = font_surface->h * 5,
        };

        scc(SDL_RenderCopy(renderer, font_texture, &font_rect, &output_rect));

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}
