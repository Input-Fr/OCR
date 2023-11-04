#include "hough.h"
#include <SDL2/SDL.h>//lib image
#include <math.h>      // pour cos et sin



int** hough(SDL_Surface* image_surface,int* max) {

    unsigned int width = image_surface->w;
    unsigned int height = image_surface->h;

    unsigned int Rhos, Thetas;
    Rhos = sqrt(width * width + height * height);
    Thetas = 180;

     int** acc_array = malloc(Rhos *2* sizeof(unsigned int*));
    for (size_t i = 0; i < Rhos*2; i++) {
        acc_array[i] = malloc(Thetas * sizeof(unsigned int));
        for (size_t j = 0; j < Thetas; j++) {
            acc_array[i][j] = 0;
        }
    }

    Uint32 pixel;
    Uint8 r, g, b;
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            pixel = get_pixel(image_surface, x, y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if ((r,g,b) >= (140,140,140)) // blanc ou noir dépend encore à definir plus tard ??
            {
                //p = x*cos(t) + y*sin(t)
                for (int t = 0; t < 180; t++) {
                    double radians = (t -90) * M_PI / 180.0; // conversion en radioan pour la formule ??
                    unsigned int p = y * cos(radians) + x * sin(radians);
                    if (p < Rhos) // interieur ?
                    {
                        acc_array[p+Rhos][t]++;
                        if(*max < acc_array[p+Rhos][t])
                            *max = acc_array[p+Rhos][t];
                    }
                }

            }
        }
    }
    return acc_array;
}






