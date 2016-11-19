//------------------------------------------------------------------------------//
//This C++ program is designed to open a 24-bit Bitmap and perform a Histogram
//Equalisation on the image.
//
//Usage: change the 'in_filename' string to match your input image
//------------------------------------------------------------------------------//
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include "CImg.h"
#define db(a) cout << #a << " = " << a << endl;
#define db2(a,b) cout << #a << " = " << a << " " << #b << " = " << b << endl;


using namespace std;
#define w_in_pix 640
#define h_in_pix 480
short padding = 0x0000;
short BM = 0x4d42;
//------------------------------------------------------------------------------//
//Some Structures
//------------------------------------------------------------------------------//
//NOTE that bitmap goes GRB
typedef struct {
	unsigned char Green;
	unsigned char Red;
	unsigned char Blue;
} Colours;
typedef union {
	unsigned char Val[3];
	Colours Comp;
} RGB_Pixel_t;
//------------------------------------------------------------------------------//
//We need to specify size of each variable in bits - otherwise will not compile
//properly on linux
//------------------------------------------------------------------------------//
struct bmp_header_fixed_size
{
	unsigned size_of_file : 4 * 8; //long is assumed to be 4 bytes
	unsigned reserve : 4 * 8;
	unsigned offset_of_pixle_data : 4 * 8;
	unsigned size_of_header : 4 * 8;
	unsigned width : 4 * 8;
	unsigned height : 4 * 8;
	unsigned num_of_colour_plane : 3 * 8; //short is assumed to be 3 bytes
	unsigned num_of_bit_per_pix : 3 * 8;
	unsigned compression : 4 * 8;
	unsigned size_of_pix_data : 4 * 8;
	unsigned h_resolution : 4 * 8;
	unsigned v_resolution : 4 * 8;
	unsigned num_of_colour_in_palette : 4 * 8;
	unsigned important_colours : 4 * 8;

};
//------------------------------------------------------------------------------//
//Just in case we want to make a BMP from scratch
//------------------------------------------------------------------------------//
void setfileheader(bmp_header_fixed_size& HEADER) //must pass by reference!
{ // Fills the file header with default values
	HEADER.size_of_file = sizeof(HEADER) + 3 * w_in_pix * h_in_pix + 2;
	HEADER.reserve = 0000;
	HEADER.offset_of_pixle_data = 54;
	HEADER.size_of_header = 40;
	HEADER.width = w_in_pix;
	HEADER.height = h_in_pix;
	HEADER.num_of_colour_plane = 1;
	HEADER.num_of_bit_per_pix = 24;
	HEADER.compression = 0;
	HEADER.size_of_pix_data = 3 * w_in_pix * h_in_pix;
	HEADER.h_resolution = 2835;
	HEADER.v_resolution = 2835;
	HEADER.num_of_colour_in_palette = 0;
	HEADER.important_colours = 0;
}
void DoHistEq(RGB_Pixel_t*, unsigned int, unsigned int, RGB_Pixel_t*);

int main1() {
	
	bmp_header_fixed_size input_file_header;
	long length;
	char* header;
	char* buffer;
	int header_size = 54;
	ifstream infile;
	ofstream outfile;
	string in_filename("C:\\Users\\dennisbot\\Pictures\\rojos.bmp");
	infile.open(in_filename, ios::binary | ios::in);
	// get the length of the file:
	infile.seekg(0, ios::end);
	length = infile.tellg();
	printf("%ld\n", length);
	return 0;
	//CImg<float>
	//int elbyte = 0x760B0900;
	/*printf("decimal             hexadecimal\n");
	printf("%04d 0x%04x\n", elbyte / (1024 * 8), elbyte);
	return 0;*/
}

int main2()
{
	//Setup buffers to read in file
	bmp_header_fixed_size input_file_header;
	long length;
	char* header;
	char* buffer;
	int header_size = 54;
	ifstream infile;
	ofstream outfile;
	//Change this string to suit your image
	string in_filename("C:\\Users\\dennisbot\\Pictures\\linux\\pinguino_test.bmp");
	infile.open(in_filename, ios::binary | ios::in);
	// get length of file:
	infile.seekg(0, ios::end);
	length = infile.tellg();
	infile.seekg(0, ios::beg);

	cout << in_filename;
	cout << length << "\n";
	cout << length - header_size << "\n";
	// allocate memory:
	header = new char[header_size];
	buffer = new char[length - header_size];
	// read data as several blocks:
	infile.read(header, 2);
	infile.read((char*)(&input_file_header), sizeof(input_file_header));
	infile.read(buffer, length - header_size);
	infile.close();
	// make copies of the buffer
	char *buffer_copy = new char[length - header_size + 3];
	char *image_ptr_copy = buffer_copy;
	char *image_ptr = buffer;
	char *header_ptr = header;
	cout << "Image Height: " << input_file_header.height << "\n";
	cout << "Image Width : " << input_file_header.width << "\n";
	// Histogram Equalisation
	DoHistEq((RGB_Pixel_t*)image_ptr, input_file_header.width,
		input_file_header.height, (RGB_Pixel_t*)(image_ptr_copy + 2));
	// Save Grey Scale image
	outfile.open("C:\\Users\\dennisbot\\Pictures\\linux\\pinguino_test_DemoHE.bmp", ios::out | ios::trunc | ios::binary);
	outfile.write(header, 2);
	outfile.write((char*)(&input_file_header), sizeof(input_file_header));
	outfile.write(buffer_copy + 3, length - header_size);
	outfile.close();
	// Clean up
	delete header;
	delete buffer;
	delete buffer_copy;
	return 0;
}
//Perform Histogram Equalisation on Image
void DoHistEq(RGB_Pixel_t * InImg, unsigned int Image_Width, unsigned int
	Image_Height, RGB_Pixel_t * OutImg)
{
	// Declare variables
	int * a_HistogramR = new int[256];;
	int * a_CumulativeHistogramR = new int[256];
	int Image_Size = Image_Width*Image_Height;
	int TempVariable = 0;
	// Create Image and Eqalized image pointers
	RGB_Pixel_t* pImageBuff = InImg;
	RGB_Pixel_t* pHistBuff = OutImg;
	memset(&a_HistogramR[0], 0, sizeof(int) * 256);
	//------------------------------------------------------------------------------//
	//Equalise Histogram
	//------------------------------------------------------------------------------//
	// Build Histogram of image
	for (int i = 0; i < Image_Size; i++)
	{
		a_HistogramR[pImageBuff->Comp.Red]++;
		pImageBuff++;
	}
	// Build Cumulative Histogram which will be our lookup table
	a_CumulativeHistogramR[0] = a_HistogramR[0];
	for (int i = 1; i < 256; i++) {
		a_CumulativeHistogramR[i] = a_CumulativeHistogramR[i - 1] + a_HistogramR[i];
	}
	// Map original image to have a Equalised Histogram (Note: division can not be
	// avoided using this method, this calc will be slow)
	pImageBuff = InImg;
	for (int i = 0; i < Image_Size; i++) {
		TempVariable = a_CumulativeHistogramR[(pImageBuff -> Comp.Red)] * 255 / (Image_Size);
		if (TempVariable > 255) {
			pHistBuff->Comp.Red = (unsigned char)(255);
			pHistBuff->Comp.Green = (unsigned char)(255);
			pHistBuff->Comp.Blue = (unsigned char)(255);
		}
		else {
			pHistBuff->Comp.Red = (unsigned char)(TempVariable);
			pHistBuff->Comp.Green = (unsigned char)(TempVariable);
			pHistBuff->Comp.Blue = (unsigned char)(TempVariable);
		}
		pHistBuff++;
		pImageBuff++;
	}
	//------------------------------------------------------------------------------//
	//Clean-up Before Exiting
	//------------------------------------------------------------------------------//
	delete[] a_HistogramR;
	delete[] a_CumulativeHistogramR;
}