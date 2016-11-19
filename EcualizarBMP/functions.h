#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <time.h>
using namespace std;
//------------------------------------------------------------------------------//
//Algunas estructuras
//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//
// definimos la estructura que contendrá el header
//------------------------------------------------------------------------------//
struct bmp_header_info
{
	unsigned size_of_file; // unsigned = unsigned int is 4 bytes
	unsigned reserve;
	unsigned offset_of_pixel_data;
	unsigned size_of_header;
	unsigned width;
	unsigned height;
	unsigned short num_of_colour_plane; // unsigned short is 2 bytes
	unsigned short num_of_bit_per_pix;
	unsigned compression;
	unsigned size_of_pix_data;
	unsigned h_resolution;
	unsigned v_resolution;
	unsigned num_of_colour_in_palette;
	unsigned important_colours;
};

typedef struct {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} dennisbot;

//NOTAR que el bitmap va GRB (BGR for little endian)

typedef struct {
	unsigned char Green;
	unsigned char Red;
	unsigned char Blue;
} Colours;

typedef union {
	unsigned char Val[3];
	Colours Comp;
} RGB_Pixel_t;

void test();

void EcualizacionHistograma8bits(RGB_Pixel_t*, unsigned int, unsigned int, RGB_Pixel_t*);

void Ecualizar8bits(string, string);

void Ecualizar(string, string, bool);

void EcualizacionHistograma(RGB_Pixel_t*, unsigned int, unsigned int, RGB_Pixel_t*, bool);