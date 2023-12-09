#ifndef NETWORK_H
#define NETWORK_H

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

// Function declarations
void initNetwork(Network* network, int* sizes);
void destroyNetwork(Network* network);
double randomNormal();
void feedForward(Network* network, double input[]);
void softmax(double* output, int size);
double crossEntropyLoss(double* predicted, double* target, int size);
void backPropagation(Network* network, double targetOutput[], double learningRate, double* input);
void trainNetwork(Network* network, double learningRate, int epochs, double** inputs);
char maxOutput(double* output);
SDL_Surface* cropSurface(SDL_Surface* originalSurface, int width, int height);
double** processPicsAndSave(Network* network);
void testPics(Network* network, double** inputs);
void printGrid(Network* network, double** testData);
void draw_lines(SDL_Surface* image_surface, int** acc_array, int* max);
void read2r(const char *file);
void writeT(char *file);
void image();
void decoup(char* file);
int solve();
int** hough(SDL_Surface* image_surface, int* max);
Uint32 get_pixel(SDL_Surface* surface, int x, int y);
void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

#endif // NETWORK_H
