//#include "hough.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>      // pour cos et sin
#include <err.h>
#include <SDL2/SDL_image.h>
#include "neuralNet.h"

/*Uint32 get_pixel(SDL_Surface* surface, int x, int y) {
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
}*/

float draw_lines2(SDL_Surface* image_surface, int** acc_array, int* max) {

    //SDL_Window* window = SDL_CreateWindow("Debug", 0, 0, image_surface->w, image_surface->h, SDL_WINDOW_SHOWN);
    //SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    //SDL_Rect texture_rect;
    //texture_rect.x = 0; //the x coordinate
    //texture_rect.y = 0; //the y coordinate
    //texture_rect.w = image_surface->w; //the width of the texture
    //texture_rect.h = image_surface->h; //the height of the texture
    //SDL_RenderClear(renderer); //clears the renderer
    //SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
    //SDL_RenderPresent(renderer); //updates the renderer
    unsigned int width = image_surface->w;
    unsigned int height = image_surface->h;
    unsigned int Rhos, Thetas;
    Rhos = sqrt(width * width + height * height);
    Thetas = 180;
    int indice = 1;
    double result = 0;
    int indice2 = 1;
    int angle = 0;
    int marge = 20;
    for (int p = 0; p < Rhos * 2; p++) {
        for (int t = 0; t < 180; t++) {
            if ((double)acc_array[p][t] >= ((double)(*max) * 0.3)) {
                double angleRad = (t - 90) * M_PI / 180;
                int y0 = (p - Rhos) / sin(angleRad);
                int x0 = 1;  // Starting x-coordinate

    //            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                int x1 = width - 1;  // Ending x-coordinate
		int y1 = x1 * - cos(angleRad) / sin(angleRad) + y0;
                
		double lineAngleRad = atan2(y1 - y0, x1 - x0);
                double lineAngleDeg = 90 - (-lineAngleRad * 180.0 / M_PI);
	
		if(lineAngleDeg < 90){
			
      //          	SDL_RenderDrawLine(renderer,y0, x0, y1, x1);
			result += lineAngleDeg;
			indice2 += 1;
		} 
		//printf("Angle of the line: %lf degrees\n", lineAngleDeg);
		indice++;

            }
        }
    }
    printf("----     %lf\n", result/(indice2));
	
     //SDL_RenderDrawLine(renderer,0, 0, 500, 500);
    //SDL_RenderPresent(renderer);

    //SDL_Event event;
    //while (1) {
      //  SDL_WaitEvent(&event);
        //switch (event.type) {
          //  case SDL_QUIT:
                // Free resources before returning
            //    SDL_DestroyTexture(texture);
              //  SDL_DestroyRenderer(renderer);
                //SDL_DestroyWindow(window);
                //return;

    		return result/indice2;
        //}
    //}
    
}




/*int** hough(SDL_Surface* image_surface,int* max) {

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
}*/



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

/*
int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    if (argc != 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return 1;
    }
    SDL_Surface *image_surface = IMG_Load(argv[1]);
    if (image_surface == NULL) {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    int max = 0;
    int** res1 = hough(image_surface,&max);

    float angle = -draw_lines2(image_surface,res1,&max);
    
    //sauvegarder les lignes traces
    
    SDL_SaveBMP(image_surface, "result.bmp");
	
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
  
        SDL_Surface* new_image2 = SDL_RotationCentralN(image,angle);
        SDL_SaveBMP(new_image2, "rotated_imageresult.bmp");

        SDL_FreeSurface(image);

    }
    else
    {
        printf("Error loading image: %s\n", SDL_GetError());
    }


    SDL_Quit();
    return 0;

}*/
