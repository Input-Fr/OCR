//
// Created by Wizen on 30/10/2023.
//
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

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

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}





SDL_Surface* rotate90(SDL_Surface* image) {
    int width = image->w;
    int height = image->h;

    SDL_Surface* temp_img = SDL_CreateRGBSurface(0, height, width, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
    int test = 0;
    for (int x = 0; x < width; x++) {
        int dest_x = height - 1;

        for (int y = 0; y < height; y++) {
            //printf("(%i,%i)\n",dest_x,x);
            Uint32 source_pix = get_pixel(image,x,y);
            put_pixel(temp_img,dest_x,x,source_pix);
            dest_x--;
            test++;


        }
    }
    printf("good");
    return temp_img;
}


SDL_Surface* SDL_RotationCentralN(SDL_Surface* origine, float angle)
{
    SDL_Surface* destination;
    int i;
    int j;
    Uint32 couleur;
    int mx, my, mxdest, mydest;
    int bx, by;
    float angle_radian;
    float tcos;
    float tsin;
    double largeurdest;
    double hauteurdest;
    //détermine la valeur en radian de l'angle
    angle_radian = -angle * M_PI / 180.0;
    //pour éviter pleins d'appel, on stocke les valeurs*/
    tcos = cos(angle_radian);
    tsin = sin(angle_radian);
    //calcul de la taille de l'image de destination
    largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
    hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),
    //alloue la mémoire à l'espace de destination, attention, la surface est de même taille
    destination = SDL_CreateRGBSurface(SDL_SWSURFACE, largeurdest, hauteurdest, origine->format->BitsPerPixel,
                                       origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);
    //on vérifie que la mémoire a été allouée
    if(destination==NULL)
        return NULL;

    //calcul du centre des images
    mxdest = destination->w/2.;
    mydest = destination->h/2.;
    mx = origine->w/2.;
    my = origine->h/2.;

    for(j=0;j<destination->h;j++)
        for(i=0;i<destination->w;i++)
        {
            // on détermine la valeur de pixel qui correspond le mieux pour la position i,j de la surface de destination

            //on détermine la meilleure position sur la surface d'origine en appliquant une matrice de rotation inverse


            bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
            by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));
            // on vérifie que l'on ne sort pas des bords
            if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
            {
                couleur = get_pixel(origine, bx, by);
                put_pixel(destination, i, j, couleur);
            }
        }

    return destination;
}


int main(int argc, char* argv[])
{
   


    SDL_Surface* image = NULL;
    
    if (argc == 2)
    {
      image = IMG_Load(argv[1]);
    }
    else
    {
    	return 1;
    }

    if (image != NULL) {
     // Rotate the image by 45 degrees

        //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
        //SDL_Rect srcRect = {0, 0, image->w, image->h}; // Use entire image as source
        //SDL_Rect dstRect = {100, 100, image->w, image->h}; // Position and size of destination

        //double angle = 90.0; // Rotate by 45 degrees
        //SDL_Surface* rotatedSurface = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
        //SDL_FillRect(rotatedSurface, NULL, SDL_MapRGB(rotatedSurface->format, 255, 255, 255));

        // Render with rotation
        //SDL_RenderClear(renderer);
        //SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, angle, NULL, SDL_FLIP_NONE);
        //SDL_RenderReadPixels(renderer, NULL, rotatedSurface->format->format, rotatedSurface->pixels, rotatedSurface->pitch);
        //SDL_Surface* screenshot = SDL_CreateRGBSurface(0, 800, 600, 32, 0, 0, 0, 0);
        //SDL_RenderReadPixels(renderer, NULL, screenshot->format->format, screenshot->pixels, screenshot->pitch);
        //SDL_SaveBMP(screenshot, "rotated_image.bmp");
       // SDL_FreeSurface(screenshot);


        printf("1");
        // Do something with the rotated image (e.g., display it or save it)
        //SDL_SaveBMP(new_image, "rotated_image.bmp");
        SDL_Surface* new_image2 = SDL_RotationCentralN(image,13);
        SDL_SaveBMP(new_image2, "rotated_image.bmp");

        printf("2");
        //SDL_FreeSurface(rotatedSurface);
        //SDL_DestroyTexture(texture);

        //SDL_FreeSurface(new_image);
        printf("3");
        SDL_FreeSurface(image);
        printf("4");

    }
    else
    {
        printf("Error loading image: %s\n", SDL_GetError());
    }

    SDL_Quit();
    printf("5");
    return 0;
}
