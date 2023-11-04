#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "hough.h"
#include <err.h>

//faire deux tableaux de tailles 100 pour eviter les depassements
 
int x[100] = {0};
int y[100] = {0};

//stocker les positions des extremites des 9 cases

int X[10];
int Y[10];



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

    //tracer les lignes et stocker les positions dans les tableaux x et y;
    draw_lines(image_surface,res1,&max,x,y);
    
    //sauvegarder les lignes traces
    SDL_SaveBMP(image_surface, "result.bmp");




//regler le cas des doublons
 	int j = 0;
	int k = 0;

   for (int i = 0; i < 100; i++) {
        if(x[i+1] - x[i] > 20 || (x[i+1] == 0 && x[i] != 0)){
                X[j] = x[i];
                j++;
        }
        if(y[i+1] - y[i] > 20 || (y[i+1] == 0 && y[i] != 0)){
                Y[k] = y[i];
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

