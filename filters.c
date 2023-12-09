//
// Created by Wizen on 01/11/2023.
//
#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define NOMIMAGE "lena.png"



SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface =  IMG_Load(path);
    SDL_Surface* newSurface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    SDL_FreeSurface(surface);
    return newSurface;
}

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    float average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    int errValue = SDL_LockSurface(surface);
    if (errValue)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (size_t i = 0; i < (size_t)len; i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    }
    SDL_UnlockSurface(surface);


}

Uint32 apply_sobel_filter(SDL_Surface* image, int x, int y, int kernel[3][3]) {
    int sum = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            Uint8* pixel = (Uint8*)image->pixels + (y + i) * image->pitch + (x + j) * image->format->BytesPerPixel;
            int gray = (int)pixel[0]; // l'image à été converti en gris avant

            sum += kernel[i + 1][j + 1] * gray;
        }
    }

    if (sum < 0) sum = 0;
    if (sum > 255) sum = 255;

    return SDL_MapRGB(image->format, sum, sum, sum);
}

Uint32 pixel_to_blackandwhite(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint32 color;
    if((r+g+b)/3 > 127)
        color = SDL_MapRGB(format,0,0,0);
    else
        color = SDL_MapRGB(format,255,255,255);
    return color;
}


void surface_to_blackandWhite(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    int errValue = SDL_LockSurface(surface);
    if (errValue)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (size_t i = 0; i < (size_t)len; i++)
    {
        pixels[i] = pixel_to_blackandwhite(pixels[i], format);
    }
    SDL_UnlockSurface(surface);


}


