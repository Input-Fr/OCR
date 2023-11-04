#include "hough.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>

Uint32 get_pixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}

void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            p[0] = (pixel & 0xFF);
            p[1] = (pixel >> 8) & 0xFF;
            p[2] = (pixel >> 16) & 0xFF;
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void draw_lines(SDL_Surface* image_surface, int** acc_array, int* max, int* y, int* x) {
    SDL_Window* window = SDL_CreateWindow("Debug",0,0,image_surface->w,image_surface->h,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_Rect texture_rect;
    texture_rect.x = 0; //the x coordinate
    texture_rect.y = 0; //the y coordinate
    texture_rect.w = image_surface->w; //the width of the texture
    texture_rect.h = image_surface->h; //the height of the texture
    SDL_RenderClear(renderer); //clears the renderer
    SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
    SDL_RenderPresent(renderer); //updates the renderer


    unsigned int width = image_surface->w;
    unsigned int height = image_surface->h;
    unsigned int Rhos, Thetas;
    Rhos = sqrt(width * width + height * height);
    Thetas = 180;
    int indice = 1;
    for (int p = 0; p < Rhos*2; p++) {
        for ( int t = 0; t < Thetas; t++) {
            if ((double)acc_array[p][t] >= ((double)*max*0.3)) {
               double angleRad = (t-90) * M_PI/180;
                int x0 = 0;
                int y0 = (p-Rhos)/ sin(angleRad);
                int x1 = width-1;
                int y1 = x1  *  -cos(angleRad)/sin(angleRad) + y0;
                SDL_SetRenderDrawColor(renderer,255,0,0,255);

                    x[indice-1]=(p-Rhos);
                    y[indice -1]=(p-Rhos);

                SDL_RenderDrawLine(renderer,p-Rhos,1,p-Rhos,height);
                SDL_RenderDrawLine(renderer,1,p-Rhos,width,p-Rhos);
               indice++;
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT : return;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}




