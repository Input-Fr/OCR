#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <err.h>

// Noyaux de Sobel en x et en y
extern int sobel_kernel_x[3][3];
extern int sobel_kernel_y[3][3];

SDL_Surface* load_image(const char* path);

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);

void surface_to_grayscale(SDL_Surface* surface);

Uint32 apply_sobel_filter(SDL_Surface* image, int x, int y, int kernel[3][3]);
void surface_to_blackandWhite(SDL_Surface* surface);

#endif /* IMAGE_PROCESSING_H */
