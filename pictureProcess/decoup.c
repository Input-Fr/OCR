#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "hough.h"


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

            		// Créer un rectangle découpé
            		SDL_Surface* croppedImage = SDL_CreateRGBSurface(0, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
            		SDL_BlitSurface(image, &rect, croppedImage, NULL);

            		// le chemin du fichier pour l'image découpée
            		char filename[50];
            		snprintf(filename, sizeof(filename), "cases/cropped_%d_%d.png", j, i);

            		// sauvegarder les images dans le dossier "cases"
            		IMG_SavePNG(croppedImage, filename);
            		SDL_FreeSurface(croppedImage);
        	}
   	}
   
     	SDL_FreeSurface(image);
    	SDL_Quit();
    	IMG_Quit();
}
