#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>


int xi[12] = {0};
int yi[12] = {0};

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

void draw_lines(SDL_Surface* image_surface, int** acc_array, int* max) {

    SDL_Window* window = SDL_CreateWindow("Debug", 0, 0, image_surface->w, image_surface->h, SDL_WINDOW_SHOWN);
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
   	
   size_t i = 0; 
   size_t j = 0; 
    int indice = 1;
    double result = 0;
    int indice2 = 1;
    int angle = 0;
    int marge = 5;
    for (int p = 0; p < Rhos * 2; p++) {
        for (int t = 0; t < 180; t++) {
            if ((double)acc_array[p][t] >= ((double)(*max) * 0.3)) {   //0.3
                double angleRad = (t - 90) * M_PI / 180;
                int y0 = (p - Rhos) / sin(angleRad);
                int x0 = 1;  // Starting x-coordinate
		
		int magique = y0 / 58;    //58

                int x1 = width - 1;  // Ending x-coordinate
		int y1 = x1 * - cos(angleRad) / sin(angleRad) + y0;
                
		
		int x2 = width - 1;  // Ending x-coordinate
                int y2 = ((p - Rhos) / sin(angleRad)) + 0.3;
		
		double lineAngleRad2 = atan2(y2 - y0, x2 - x0);
                double lineAngleDeg2 = 90 - (-lineAngleRad2 * 180.0 / M_PI);
 		
		if(lineAngleDeg2 > 90 - marge && lineAngleDeg2 < 90 + marge && y2 > 0 && y2 < width - 200){
				
               		if((j == 0 || (y2 - xi[j - 2] > 100))){
				xi[j+1] = y2;
				j+=3;	
				SDL_SetRenderDrawColor(renderer, 250, 0, 250, 255);
				SDL_RenderDrawLine(renderer,y0, x0, y2, x2);
			}   			
		}

               if((i == 0 && magique > 0 )|| (magique - yi[i] > 20) && magique > 0){
			SDL_SetRenderDrawColor(renderer, 250, 100, 100, 255);
			SDL_RenderDrawLine(renderer,y0, x0, y1, x1);  //draw horizontal line
			yi[i+1] = magique; 
			i+=1;

		}		
			
	
		//printf("Angle of the line: %lf degrees\n", lineAngleDeg);
		indice++;

            }
        }
    }
	
    
   // SDL_RenderDrawLine(renderer,500, 568, 178, 586);
    
    //SDL_RenderDrawLine(renderer,500, 568, 1000, 1175);
    //SDL_RenderDrawLine(renderer,300, 300, 977, 977);
    
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while (1) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                // Free resources before returning
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                return;

    		//return result/indice2;
        }
    }
    
}


unsigned int RhosG;
unsigned int ThetasG;

void freeHough(int** acc_array)
{
	for (int i = 0; i < RhosG *2; i++)
	{
		free(acc_array[i]);
	}
	free(acc_array);
}

int** hough(SDL_Surface* image_surface,int* max) {

    unsigned int width = image_surface->w;
    unsigned int height = image_surface->h;


    RhosG = sqrt(width * width + height * height);
    ThetasG = 180;

    int** acc_array = malloc(RhosG *2* sizeof(unsigned int*));
    for (size_t i = 0; i < RhosG*2; i++) {
        acc_array[i] = malloc(ThetasG * sizeof(unsigned int));
        for (size_t j = 0; j < ThetasG; j++) {
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
                    if (p < RhosG) // interieur ?
                    {
                        acc_array[p+RhosG][t]++;
                        if(*max < acc_array[p+RhosG][t])
                            *max = acc_array[p+RhosG][t];
                    }
                }

            }
        }
    }
    return acc_array;
}

void decoup(char* file){

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
 	
    	for (int i = 1; i < 10; i++) {
    		for (int j = 1; j < 10; j++) {
            		int yy = yi[j];
            		int xx = xi[i];
		
			//taille de la case
            		int width = xi[j + 1] - xi[j];
            		int height = yi[i+1] - yi[i];  
			

			//position de la case
            		SDL_Rect rect;
            		rect.x = xx;        
            		rect.y = yy;
			rect.w = width;
			rect.h = height;
            		// Créer un rectangle découpé
            		SDL_Surface* croppedImage = SDL_CreateRGBSurface(0, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
            		SDL_BlitSurface(image, &rect, croppedImage, NULL);

            		// le chemin du fichier pour l'image découpée
            		char filename[50];
            		snprintf(filename, sizeof(filename), "cases/cropped_%d_%d.bmp", j - 1, i - 1);

            		// sauvegarder les images dans le dossier "cases"
            		SDL_SaveBMP(croppedImage, filename);
            		SDL_FreeSurface(croppedImage);
		}
	}
   
     	SDL_FreeSurface(image);
    	IMG_Quit();
    	SDL_Quit();

}

// Activation functions
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return sigmoid(x) * (1.0 - sigmoid(x));
}

// Neural Network Structure
typedef struct {
    double* weightsInputHidden;
    double* weightsHiddenOutput;
    double* biasesHidden;
    double* biasesOutput;
    double* hiddenLayerValues;
    double* outputLayerValues;
    int* sizes;
} Network;

// Random initialization
double randomNormal() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

// Initialize the neural network
void initNetwork(Network* network, int* sizes) {
    network->sizes = sizes;
    network->hiddenLayerValues = malloc(network->sizes[1]* sizeof(double));
    network->outputLayerValues = malloc(network->sizes[2]* sizeof(double));
    network->weightsInputHidden = malloc(sizes[0] * sizes[1]* sizeof(double));
    network->weightsHiddenOutput = malloc(sizes[1] * sizes[2]* sizeof(double));
    network->biasesHidden = malloc(network->sizes[1]* sizeof(double));
    network->biasesOutput = malloc(network->sizes[2]* sizeof(double));
    FILE *filew1 = fopen("weightsInputHidden.txt", "r");
    FILE *filew2 = fopen("weightsHiddenOutput.txt", "r");
    FILE *fileb1 = fopen("biasesHidden.txt", "r");
    FILE *fileb2 = fopen("biasesOutput.txt", "r");


    for (int i = 0; i < network->sizes[0] * network->sizes[1]; i++) {
        char buffer[1024];
    	while (fgets(buffer, sizeof(buffer), filew1) != NULL) {
        	
        	size_t length = strlen(buffer);
        	if (length > 0 && buffer[length - 1] == '\n') {
            		buffer[length - 1] = '\0';
        	}
        	
        	network->weightsInputHidden[i] = strtod(buffer,NULL);
		i++;

    	}
    }
    for (int i = 0; i < network->sizes[1] * network->sizes[2]; i++) {
        char buffer[1024];
        while (i < (network->sizes[1] * network->sizes[2]) && fgets(buffer, sizeof(buffer), filew2) != NULL) {

                size_t length = strlen(buffer);
                if (length > 0 && buffer[length - 1] == '\n') {
                        buffer[length - 1] = '\0';
                }


                network->weightsHiddenOutput[i] = strtod(buffer,NULL);
                i++;

        }


    }

    for (int i = 0; i < network->sizes[1]; i++) {
	char buffer[1024];
        while (i < network->sizes[1] && fgets(buffer, sizeof(buffer), fileb1) != NULL) {

                size_t length = strlen(buffer);
                if (length > 0 && buffer[length - 1] == '\n') {
                        buffer[length - 1] = '\0';
                }


                network->biasesHidden[i] = strtod(buffer,NULL);
                i++;

        }
    }

    for (int i = 0; i < network->sizes[2]; i++) {
        char buffer[1024];
        while (i < network->sizes[2] && fgets(buffer, sizeof(buffer), fileb1) != NULL) {

                size_t length = strlen(buffer);
                if (length > 0 && buffer[length - 1] == '\n') {
                        buffer[length - 1] = '\0';
                }


                network->biasesOutput[i] = strtod(buffer,NULL);
                i++;

        }
    }

    fclose(filew1);
    fclose(filew2);
    fclose(fileb1);
    fclose(fileb2);


}

// Free memory used by the neural network
void destroyNetwork(Network* network) {
    free(network->weightsInputHidden);
    free(network->weightsHiddenOutput);
    free(network->hiddenLayerValues);
    free(network->outputLayerValues);
    free(network->biasesHidden);
    free(network->biasesOutput);
    free(network->sizes);
    free(network);
}

// Softmax activation function
void softmax(double* output, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        output[i] = exp(output[i]);
        sum += output[i];
    }
    for (int i = 0; i < size; i++) {
        output[i] /= sum;
    }
}

// Feedforward pass through the network with softmax activation
void feedForward(Network* network, double input[network->sizes[0]]) {
    int* sizes = network->sizes;
    for (int i = 0; i < network->sizes[1]; i++) {
        network->hiddenLayerValues[i] = 0;
        for (int j = 0; j < network->sizes[0]; j++) {
            network->hiddenLayerValues[i] += input[j] * network->weightsInputHidden[j * sizes[1] + i];
        }
        double bias = network->biasesHidden[i];
        network->hiddenLayerValues[i] = sigmoid(network->hiddenLayerValues[i] + bias);
    }

    for (int i = 0; i < network->sizes[2]; i++) {
        network->outputLayerValues[i] = 0;
        for (int j = 0; j < network->sizes[1]; j++) {
            network->outputLayerValues[i] += network->hiddenLayerValues[j] * network->weightsHiddenOutput[j * sizes[2] + i];
        }
        double bias = network->biasesOutput[i];
        // Softmax activation applied to the output layer
        network->outputLayerValues[i] = sigmoid(network->outputLayerValues[i] + bias);
    }

    // Apply softmax activation to the output layer
    //softmax(network->outputLayerValues, network->sizes[2]);
}


// Cross-entropy loss
double crossEntropyLoss(double* predicted, double* target, int size) {
    double loss = 0.0;
    for (int i = 0; i < size; i++) {
        loss += target[i] * log(predicted[i] + 1e-15);  // Adding a small epsilon to avoid log(0)
    }
    return -loss;
}

void backPropagation(Network* network, double targetOutput[], double learningRate, double* input) {
    int* sizes = network->sizes;

    // Backpropagate the error through the network
    double deltaOutput[sizes[2]];
    for (int i = 0; i < sizes[2]; i++) {
        deltaOutput[i] = targetOutput[i] - network->outputLayerValues[i];
    }

    // Update weights and biases for the output layer
    for (int i = 0; i < sizes[1]; i++) {
        for (int j = 0; j < sizes[2]; j++) {
            network->weightsHiddenOutput[i * sizes[2] + j] += learningRate * deltaOutput[j] * network->hiddenLayerValues[i];
            network->biasesOutput[j] += learningRate * deltaOutput[j];
        }
    }

    // Calculate delta for hidden layer
    double deltaHidden[sizes[1]];
    for (int i = 0; i < sizes[1]; i++) {
        deltaHidden[i] = 0;
        for (int j = 0; j < sizes[2]; j++) {
            deltaHidden[i] += deltaOutput[j] * network->weightsHiddenOutput[i * sizes[2] + j];
        }
        deltaHidden[i] *= sigmoid_derivative(network->hiddenLayerValues[i]);
    }

    // Update weights and biases for the hidden layer
    for (int i = 0; i < sizes[0]; i++) {
        for (int j = 0; j < sizes[1]; j++) {
            network->weightsInputHidden[i * sizes[1] + j] += learningRate * deltaHidden[j] * input[i];
            network->biasesHidden[j] += learningRate * deltaHidden[j];
        }
    }
}

// Train the neural network
void trainNetwork(Network* network, double learningRate, int epochs,double** inputs) {
    double labels[5*9] = {5,6,0,8,4,7,0,0,3,0,9,0,0,0,6,0,0,0,0,8,0,0,0,0,0,0,0,1,0,0,8,0,0,4,0,7,9,0,6,0,2,0,1,8};

    for (int e = 0; e < epochs; e++) {
        int idx = 0;
	for (int i = 0; i < 5; i++) {

	    for (int j = 0; j < 9; j++)
	    {
		if (labels[idx] != 0)
		{
			int outputLabel = (int)labels[idx];
            		double targetOutput[10];
            		for (int ji = 0; ji < 10; ji++) {
                		targetOutput[ji] = (ji == outputLabel) ? 1.0 : 0.0;
            		}

            		feedForward(network, inputs[idx]);

            		double err[10];
            	
	    		for (int jy = 0; jy < 10; jy++) {
                		err[jy] = targetOutput[jy] - network->outputLayerValues[jy];
            		}
		
            		backPropagation(network, err, learningRate, inputs[idx]);
		}
	    	
            	
            	idx++;
	    }

        }
    }

}


char maxOutput(double* output)
{
	int max = 0;
	for(int i = 0; i<10;i++)
	{
		if (output[i] > 0.7 && output[i]>output[max])
		{
			max = i;
		}
	}
	if (max == 0)
	{
		return '.';
	}
	return '0'+max;
	
}
SDL_Surface* cropSurface(SDL_Surface* originalSurface, int width, int height) {

    SDL_Surface* croppedSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    SDL_Rect sourceRect = {8, 8, width, height};
    SDL_Rect destinationRect = {0, 0, width, height};


    SDL_BlitSurface(originalSurface, &sourceRect, croppedSurface, &destinationRect);

    return croppedSurface;
}
double** processPicsAndSave(Network* network)
{
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return NULL; // Return an error code
    }
    double** inputs = malloc(81*sizeof(double*));	
    int idxCpt = 0;
    for(int i = 0; i < 9; i++)
    {
	for(int j = 0; j < 9; j++)
	{
		
	    char bufferIn[23];
	    snprintf(bufferIn, 22, "cases/cropped_%i_%i.bmp", i,j);
	    // Charger l'image d'origine
	    SDL_Surface* originalSurface = IMG_Load(bufferIn);

	    SDL_Surface* cropped  = cropSurface(originalSurface, originalSurface->w-15, originalSurface->h-15);

	    SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);

	    SDL_BlitScaled(cropped, NULL, resizedSurface, NULL);

	    char bufferOut[27];
            snprintf(bufferOut, 26, "cropCases/cropped_%i_%i.bmp", i,j);

	

	    /* Convert to array */
	    inputs[idxCpt] = malloc(sizeof(double)*28*28);

	    Uint32* pixels = resizedSurface->pixels;
    	    int len = resizedSurface->w * resizedSurface->h;
            SDL_PixelFormat* format = resizedSurface->format;
            int errValue = SDL_LockSurface(resizedSurface);
            if (errValue)
        	errx(EXIT_FAILURE, "%s", SDL_GetError());
	
    	    for (size_t idx = 0; idx < (size_t)len; idx++)
    	    {
        	Uint8 r, g, b;
    		SDL_GetRGB(pixels[idx], format, &r, &g, &b);
    		inputs[idxCpt][idx] = r>0?1:0;
    	    }
    	    SDL_UnlockSurface(resizedSurface);

    	    /* ---------------- */

	    SDL_SaveBMP(resizedSurface, bufferOut);

	    // Libérer la mémoire
	    SDL_FreeSurface(originalSurface);
	    SDL_FreeSurface(cropped);
 	    SDL_FreeSurface(resizedSurface);

 	    feedForward(network,inputs[idxCpt]);

 	    idxCpt++;

	}
    }

    SDL_Quit();
    
    return inputs;
}
void testPics(Network* network,double** inputs)
{
    SDL_Init(SDL_INIT_VIDEO);
	
    int idx = 0;
    for(int i = 0; i < 9; i++)
    {
	for(int j = 0; j < 9; j++)
	{
	
 	
 	    feedForward(network,inputs[idx]);

            printf("output 0 : %f \n",network->outputLayerValues[0]);
            printf("output 1 : %f \n",network->outputLayerValues[1]);
            printf("output 2 : %f \n",network->outputLayerValues[2]);
            printf("output 3 : %f \n",network->outputLayerValues[3]);
            printf("output 4 : %f \n",network->outputLayerValues[4]);
            printf("output 5 : %f \n",network->outputLayerValues[5]);
            printf("output 6 : %f \n",network->outputLayerValues[6]);
            printf("output 7 : %f \n",network->outputLayerValues[7]);
            printf("output 8 : %f \n",network->outputLayerValues[8]);
            printf("output 9 : %f \n",network->outputLayerValues[9]);

	    printf("\n\n");
            idx++;


	}
    }

    SDL_Quit();
}
void printGrid(Network* network, double** testData) {
    FILE *file = fopen("grid_00", "w");
    for (int i = 0; i < 81; i++) {
        double input[28*28];

	for (int j = 0; j < 28*28; j++) {
            input[j] = testData[i][j];
        }
	
        feedForward(network, input);
	char outPut = maxOutput(network->outputLayerValues);
	if ((i) % 3 == 0 )
        	fprintf(file," %c",outPut);
        else
        	fprintf(file,"%c",outPut);
        if (i != 0 && (i+1) % 9 == 0)
                fprintf(file,"\n");
	
    }
    fclose(file);
}

short sudoku[9][9] =
        {
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},

                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},

                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
        };

short sudoku2[9][9] =
        {
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},

                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},

                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
                {0,0,0, 0,0,0, 0,0,0},
        };

int possible(short n, size_t x, size_t y){
    for (size_t val = 0; val < 9; val+=1){
        //test sur la ligne
        if (sudoku[val][y] == n){
            return 0;
        }
        //test sur la colonne
        if (sudoku[x][val] == n){
            return 0;
        }
    }
    for (size_t Y = 0; Y < 3; Y += 1){
//calcul de la position de Ro et Co les premières valeurs
//dans le petit carré trois*trois dans laquelle
        //la valeur n se trouve
        size_t Ro = 6;
        size_t Co = 6;
        if(x < 3)
            Ro = 0;
        else
        if(x < 6)
            Ro = 3;
        if(y < 3)
            Co = 0;
        else
        if(y < 6)
            Co = 3;

        for(size_t X = 0; X < 3; X+=1){
            //test sur le carre de trois
            if (sudoku[Ro+Y][Co+X] == n){
                return 0;
            }
        }
    }
    return 1;
}



int solve(){
    for(size_t i = 0; i < 9; i++){       //parcour toutes les cases du sudoku
        for(size_t j = 0; j < 9; j++){

            if(sudoku[i][j] == 0){
                for(short num = 1; num <= 9; num++){
                    if (possible(num,i,j)){       //test de la case avec le chiffre num
                        sudoku[i][j] = num;
                        if(solve()){
                            return 1; //appel recursif, si le sudoku est resolu
                            //alors le num est bon sinon le num n'est pas bon
                        }
                        sudoku[i][j] = 0; //la valeur ne marche pas donc
                        // on reniatilise a zero et on continue avec les autres nums
                    }
                }
                return 0;
            }

        }
    }
    return 1;
}


void read2r(const char *file) {
    FILE *fichier = fopen(file, "r");
if (fichier == NULL) {
    perror("Error opening file");
    return;
}
    int caractere;
    size_t i = 0;
    size_t j = 0;
    while ((caractere = fgetc(fichier)) != EOF) {
        if(i == 9){
            i = -1;
            j+=1;
        }
        if(caractere != ' '){
            if(caractere >= '1' && caractere <= '9'){
		sudoku2[j][i] = caractere - 48;
                sudoku[j][i] = caractere - 48;    //on converti les valeurs char en int
            }
            i++;
        }
    }
    fclose(fichier);
}

void writeT(char *file){
    char message[] = ".result";
    char resultat[20];
    strcpy(resultat, file);
    strcat(resultat, message);     //creer un fichier avec le nom du fichier original + .result
    FILE *nouveauFichier = fopen(resultat, "w");
    if (nouveauFichier == NULL) {
        return;
    }

    for (size_t row = 0; row < 9; row++)
    {
        for (size_t col = 0; col < 9; col++)
        {
            if(col % 3 == 0 && col != 0){
                fprintf(nouveauFichier," ");
            }
            fprintf(nouveauFichier,"%d", sudoku[row][col]); //print les valeurs
        }
        if((row + 1) % 3 == 0){
            fprintf(nouveauFichier,"\n");
        }
        if(row!= 8)
            fprintf(nouveauFichier,"\n");
    }
    fclose(nouveauFichier);
}

void image(){
   // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    // Load the background image
    SDL_Surface* background = SDL_LoadBMP("image_.bmp");
    if (!background) {
        printf("Error loading background image: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Load the overlay image

   
    const char* imageFiles[] = {"0.bmp","1v.bmp", "2v.bmp", "3v.bmp","4v.bmp", "5v.bmp", "6v.bmp","7v.bmp", "8v.bmp", "9v.bmp"};
    const char* imageFiles2[] = {"0.bmp","1.bmp", "2.bmp", "3.bmp","4.bmp", "5.bmp", "6.bmp","7.bmp", "8.bmp", "9.bmp"};

    // Array of coordinates for each image
    int x[9] = {13,120,230,343,450,560,670,780,890};
    int y[9] = {13,120,230,343,450,560,670,780,890};
	   
    // Loop to load and overlay each image
    for (int i = 0; i < 9; ++i) {
        // Load the image
	    for (int j = 0; j < 9; j++) {

		    SDL_Surface* image;
		    //printf("%hhi \n",sudoku[i][j]);
		    //printf("%i   ",sudoku2[i][j]);
		    if(sudoku2[j][i] == 0){

			    image = SDL_LoadBMP(imageFiles[sudoku[j][i]]);
			    // Rectangle source (entire image)
			    SDL_Rect srcrect = {0, 0, image->w, image->h};

			    // Rectangle destination (where to paste the image)
			    SDL_Rect dstrect = {x[i], y[j], 0, 0};

			    // Overlay the image on the background
			    SDL_BlitSurface(image, &srcrect, background, &dstrect);

			    // Free the loaded image surface
			    SDL_FreeSurface(image);
		    }else{

			    image = SDL_LoadBMP(imageFiles2[sudoku[j][i]]);

			    // Rectangle source (entire image)
			    SDL_Rect srcrect = {0, 0, image->w, image->h};

			    // Rectangle destination (where to paste the image)
			    SDL_Rect dstrect = {x[i], y[j], 0, 0};

			    // Overlay the image on the background
			    SDL_BlitSurface(image, &srcrect, background, &dstrect);

			    // Free the loaded image surface
			    SDL_FreeSurface(image);
		    }

		    if (!image) {
			    printf("Error loading image %d: %s\n", i + 1, SDL_GetError());
			    SDL_FreeSurface(background);
			    SDL_Quit();
			    return;
		    }
	    }
    }
    // Save the result
    if (SDL_SaveBMP(background, "result.bmp") != 0) {
        printf("Error saving the resulting image: %s\n", SDL_GetError());
    }

    // Free the background surface
    SDL_FreeSurface(background);

    // Quit SDL
    SDL_Quit();

}

/*int main(int argc, char *argv[]) {

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

    draw_lines(image_surface,res1,&max);
 
    for(size_t i = 0; i < 12; i+=1){
	    printf("magique y: %i \n",y[i]);
    }

    printf("\n");
   
    size_t p = 1; 
	for(size_t k = 0; k < 4; k+=1){
		if(p < 8){
		xi[p+1] = xi[p] + ((x[p+3] - x[p]) / 3);
		x[p+2] = x[p+1] + ((x[p+3] - x[p]) / 3);
		}
		p+=3;
	 }
    for(size_t i = 0; i < 12; i+=1){
	    printf("magique2 x: %i \n",x[i]);
    }
	
    decoup(argv[1]);
  
    SDL_Quit();
    
    int sizes[] = {28*28, 128, 10};

    Network* network = malloc(sizeof(Network));
    initNetwork(network, sizes);

    double** inputs = processPicsAndSave(network);



    //trainNetwork(network, learningRate, epochs,inputs);

    printGrid(network, inputs);
    //testPics(network,inputs);
    //testNetwork(network, trainingData, labels);

    for (int i = 0; i < 81; i++)
    {
    	free(inputs[i]);
    }

    free(inputs);

    destroyNetwork(network);
    
    
    read2r("grid_00");  	//lire le fichier avec la grile la pour remplir dans la matrice

   	//sauvegarde de la grille dans un nouveau fichier

	//image();
    
 int a = solve();  	//on resoud la grille

    if(!a){     		//on verifie que la grille a ete resolu
        printf("sudoku non valide");
        return 1;
    }
    
	image();
	
    writeT("grid_00");  
    
    return 0;
}*/
