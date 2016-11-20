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

#define db(a) cout << #a << " = " << a << endl;
#define PATH_PROJECT "/Users/dennisbot/Google Drive/maestria/semestre_II/imagenes/code/ProjectosJC/EcualizarBMP/"
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
} palette_t;

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



void Ecualizar1bit(string, string);

void Ecualizar4bits(string, string);

void Ecualizar8bits(string, string);

void Ecualizar(string, string, bool);

void EcualizacionHistograma1bit(unsigned char *InImg, unsigned int Image_Width, unsigned int Image_Height, unsigned char *OutImg);

void EcualizacionHistograma4bits(RGB_Pixel_t*, unsigned int, unsigned int, RGB_Pixel_t*);

void EcualizacionHistograma8bits(unsigned char *, unsigned int, unsigned int, unsigned char *);

void EcualizacionHistograma(RGB_Pixel_t*, unsigned int, unsigned int, RGB_Pixel_t*, bool);

string getWithPath(string);