#include "hough.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>      // pour cos et sin
#include <err.h>
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


void decoup(char* file,int *X, int *Y){

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        	fprintf(stderr, "Échec de l'initialisation de SDL : %s\n", SDL_GetError());
        	return;
    	}

    	if (IMG_Init(IMG_INIT_PNG) == 0) {
        	fprintf(stderr, "Échec de l'initialisation de SDL_image : %s\n", IMG_GetError());
        	SDL_Quit();
        	return;
    	}

    	SDL_Surface* image = IMG_Load(file);
    	if (image == NULL) {
        	fprintf(stderr, "Échec du chargement de l'image : %s\n", IMG_GetError());
        	SDL_Quit();
        	IMG_Quit();
        	return;
   	 }


        int w = image->w;
    	int h = image->h;
 	
    	for (int i = 0; i < 9; i++) {
        	for (int j = 0; j < 9; j++) {
            		int x = X[j];
            		int y = Y[i];

			//taille de la case
            		int width = X[i + 1] - X[i];
            		int height = Y[i + 1] - Y[i];

			//position de la case
            		SDL_Rect rect;
            		rect.x = x;        
            		rect.y = y;
			rect.w = width;
			rect.h = height;
            		// Créer un rectangle découpé
            		SDL_Surface* croppedImage = SDL_CreateRGBSurface(0, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
            		SDL_BlitSurface(image, &rect, croppedImage, NULL);

            		// le chemin du fichier pour l'image découpée
            		char filename[50];
            		snprintf(filename, sizeof(filename), "cases/cropped_%d_%d.bmp", j, i);

            		// sauvegarder les images dans le dossier "cases"
            		SDL_SaveBMP(croppedImage, filename);
            		SDL_FreeSurface(croppedImage);
        	}
   	}
   
     	SDL_FreeSurface(image);
    	SDL_Quit();
    	IMG_Quit();
}






int main(int argc, char *argv[]) {

    int x[100] = {0};
    int y[100] = {0};

    int X[10];
    int Y[10];
    /*
    char *command[] = {"mkdir", "cases", NULL};

    if (execvp(command[0], command) == -1) {
        perror("Erreur lors de l'exécution de mkdir");
        exit(EXIT_FAILURE);
    }
    */

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

    //tracer les lignes et stocker les positions dans les tableaux x et y;
    draw_lines(image_surface,res1,&max,x,y);
    
    //sauvegarder les lignes traces
    SDL_SaveBMP(image_surface, "result.bmp");



//regler le cas des doublons
 	int j = 0;
	int k = 0;

   for (int i = 0; i < 99; i++) {
        if(x[i+1] - x[i] > 20 || (x[i+1] == 0 && x[i] != 0)){
                X[j] = x[i];
		printf("x : %i\n",X[j]);
		j++;
        }
        if(y[i+1] - y[i] > 20 || (y[i+1] == 0 && y[i] != 0)){
                Y[k] = y[i];
		printf("y : %i\n",Y[k]);
                k++;
        }

    }

//exception pour la derniere ligne
    int diff = X[1] - X[0];
    X[9] = X[8] + diff;

    int diff2 = Y[1] - Y[0];
    Y[9] = Y[8] + diff2;
    
    
    //decouper les cases 
    decoup(argv[1],X,Y);


    SDL_Quit();
    return 0;

}

