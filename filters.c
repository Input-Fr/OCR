#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <err.h>

// Noyaux de Sobel en x et en y
int sobel_kernel_x[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
};

int sobel_kernel_y[3][3] = {
        { -1, -2, -1 },
        { 0, 0, 0 },
        { 1, 2, 1 }
};

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
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

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            Uint8* pixel = (Uint8*)image->pixels + (y + i) * image->pitch + (x + j) * image->format->BytesPerPixel;
            int gray = (int)pixel[0]; // l'image à été converti en gris avant

            sum += kernel[i + 1][j + 1] * gray;
        }
    }

    if (sum < 0)
    {
        sum = 0;
    }
    if (sum > 255)
    {
        sum = 255;
    }

    return SDL_MapRGB(image->format, sum, sum, sum);
}


int main(int argc, char * argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    if (argc != 2)
    {
        printf("Usage : ./filter <imageName>");
        return 1;
    }
    SDL_Surface* image = load_image(argv[1]);
    if (!image) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        return 1;
    }

    surface_to_grayscale(image);

    // Créer une surface pour l'image filtrée
    SDL_Surface* filtered_image = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);

    // Appliquer le filtre de Sobel
    for (int y = 1; y < image->h - 1; y++) {
        for (int x = 1; x < image->w - 1; x++) {

            Uint32 new_pixel_x = apply_sobel_filter(image, x, y, sobel_kernel_x);
            Uint32 new_pixel_y = apply_sobel_filter(image, x, y, sobel_kernel_y);

            // Combiner les résultats de Sobel en x et en y
            Uint8* p_filtered = (Uint8*)filtered_image->pixels + y * filtered_image->pitch + x * filtered_image->format->BytesPerPixel;
            Uint8* p_x = (Uint8*)&new_pixel_x;
            Uint8* p_y = (Uint8*)&new_pixel_y;

            for (int c = 0; c < filtered_image->format->BytesPerPixel; c++) {
                p_filtered[c] = (Uint8)sqrt(pow(p_x[c], 2) + pow(p_y[c], 2));
            }

        }
    }



    SDL_SaveBMP(filtered_image,"out.bmp");

    SDL_FreeSurface(image);
    SDL_FreeSurface(filtered_image);
    SDL_Quit();

    return 0;
}
