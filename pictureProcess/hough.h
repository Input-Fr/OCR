#ifndef HOUGH_H
#define HOUGH_H
#include <SDL2/SDL.h>

// Function prototypes
int** hough(SDL_Surface* image_surface, int* max);
Uint32 get_pixel(SDL_Surface* surface, int x, int y);
void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void draw_lines(SDL_Surface* image_surface, int** acc_array, int* max, int* y, int* x);
static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
void hough_transform(SDL_Surface* surface);
SDL_Surface* detectGrid(SDL_Surface* image);
SDL_Surface* performEdgeDetection(SDL_Surface* image, int max);
void calculate_gradients(SDL_Surface* input_surface, int* gradient_x, int* gradient_y);
unsigned char* apply_canny_edge_detection(SDL_Surface* image_surface);
void SDL_CannyEdge(SDL_Surface* input_surface, unsigned char* output_image);
SDL_Surface* create_surface_from_data(unsigned char* data, int width, int height);


void decoup(char* file,int *X, int *Y);




#endif

