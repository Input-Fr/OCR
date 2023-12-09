#ifndef ROTATE_H
#define ROTATE_H

//#include "hough.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <SDL2/SDL_image.h>


float draw_lines2(SDL_Surface* image_surface, int** acc_array, int* max);
//int** hough(SDL_Surface* image_surface, int* max);
SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle);

#endif /* ROTATE_H */
