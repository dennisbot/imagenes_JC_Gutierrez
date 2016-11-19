#include "functions.h"
using namespace std;

void test() {
	char * row = new char[16];
	ifstream in("input\\test.out", ios::in | ios::binary);
	printf("sizeof row %d\n", sizeof(row));
	in.read(row, sizeof(row) * 4);
	in.close();
	RGB_Pixel_t* pixel = (RGB_Pixel_t*)row;
	for (size_t i = 0; i < 6; i++) {
		printf("Group %d\n", i);
		puts("==============================");
		printf("sizeof pixel %d\n", sizeof(pixel));
		printf("G = %d hex = %x\n", pixel->Comp.Green, pixel->Comp.Green);
		printf("R = %d hex = %x\n", pixel->Comp.Red, pixel->Comp.Red);
		printf("B = %d hex = %x\n", pixel->Comp.Blue, pixel->Comp.Blue);
		pixel++;
	}
}

void Ecualizar8bits(string in_filename, string out_filename) {
	//Setup buffers to read in file
	bmp_header_info input_file_header;
	long length;
	char* header;
	char* buffer;
	int header_size = 54;
	ifstream infile;
	ofstream outfile;

	// Cambiar esta cadena para que quepe con la imagen
	infile.open(in_filename, ios::binary | ios::in);

	// obtener el tama�o del archivo:
	infile.seekg(0, ios::end);
	length = infile.tellg();
	infile.seekg(0, ios::beg);
	cout << in_filename << endl;
	cout << length << "\n";
	cout << length - header_size << "\n";

	// reservar memoria:
	header = new char[header_size];

	buffer = new char[length - header_size];

	// leer los datos como si fueran muchos bloques
	infile.read(header, 2);
	infile.read((char*)(&input_file_header), sizeof(input_file_header));
	dennisbot paleta;
	infile.read((char*)&paleta, sizeof(dennisbot));
	puts("PALETA");
	puts("======================================");
	printf("%d - %x\n", paleta.rgbBlue);
	printf("%d - %x\n", paleta.rgbGreen);
	printf("%d - %x\n", paleta.rgbRed);
	printf("%d - %x\n", paleta.rgbReserved);
	puts("======================================");

	infile.read(buffer, length - header_size);
	infile.close();
	puts("se deitene aca");
	int a;
	cin >> a;
	return;
	// hacemos algunas copias del buffer
	char *buffer_copy = new char[length - header_size];
	char *image_ptr_copy = buffer_copy;
	char *image_ptr = buffer;
	char *header_ptr = header;

	// Ecualizaci�n del histograma
	EcualizacionHistograma8bits((RGB_Pixel_t*)image_ptr, input_file_header.width, input_file_header.height, (RGB_Pixel_t*)(image_ptr_copy));


	// Guardar imagen en escala de grises
	outfile.open(out_filename, ios::out | ios::trunc | ios::binary);

	outfile.write(header, 2);
	outfile.write((char*)(&input_file_header), sizeof(input_file_header));
	outfile.write(buffer_copy, length - header_size);
	outfile.close();
	// Clean up
	delete header;
	delete buffer;
	delete buffer_copy;
}

void Ecualizar(string in_filename, string out_filename, bool is_gray_scale)
{	
	//Setup buffers to read in file
	bmp_header_info input_file_header;
	long length;
	char* header;
	char* buffer;
	int header_size = 54;
	ifstream infile;
	ofstream outfile;

	// Cambiar esta cadena para que quepe con la imagen
	infile.open(in_filename, ios::binary | ios::in);

	// obtener el tama�o del archivo:
	infile.seekg(0, ios::end);
	length = infile.tellg();
	infile.seekg(0, ios::beg);
	cout << in_filename << endl;
	cout << length << "\n";
	cout << length - header_size << "\n";

	// reservar memoria:
	header = new char[header_size];

	buffer = new char[length - header_size];

	// leer los datos como si fueran muchos bloques
	infile.read(header, 2);

	puts("cabecera tipo de archivo:");
	for (size_t i = 0; i < 2; i++)
	{
		printf("0x%x\n", header[i]);
	}

	infile.read((char*)(&input_file_header), sizeof(input_file_header));
	bool show_info = false;
	if (show_info) {
		puts("size of BMP file in bytes (unreliable)");
		printf("0x%x\n", input_file_header.size_of_file);
		puts("reserved, must be zero");
		printf("0x%x\n", input_file_header.reserve);
		puts("offset to start of image data in bytes");
		printf("0x%x\n", input_file_header.offset_of_pixel_data);
		puts("size of BITMAPINFOHEADER structure, must be 40");
		printf("0x%x\n", input_file_header.size_of_header);

		puts("image width in pixels");
		printf("0x%x\n", input_file_header.width);
		puts("image height in pixels");
		printf("0x%x\n", input_file_header.height);

		puts("number of planes in the image, must be 1");
		printf("0x%x\n", input_file_header.num_of_colour_plane);
		puts("number of bits per pixel (1, 4, 8, or 24)");
		printf("0x%x\n", input_file_header.num_of_bit_per_pix);
		puts("compression type (0=none, 1=RLE-8, 2=RLE-4)");
		printf("0x%x\n", input_file_header.compression);
		puts("size of image data in bytes (including padding)");
		printf("0x%x\n", input_file_header.size_of_pix_data);
		puts("horizontal resolution in pixels per meter (unreliable)");
		printf("0x%x\n", input_file_header.h_resolution);
		puts("vertical resolution in pixels per meter (unreliable)");
		printf("0x%x\n", input_file_header.v_resolution);
		puts("number of colors in image, or zero");
		printf("0x%x\n", input_file_header.num_of_colour_in_palette);
		puts("number of important colors, or zero");
		printf("0x%x\n", input_file_header.important_colours);
	}

	infile.read(buffer, length - header_size);
	infile.close();

	// hacemos algunas copias del buffer
	char *buffer_copy = new char[length - header_size];
	char *image_ptr_copy = buffer_copy;
	char *image_ptr = buffer;
	char *header_ptr = header;
	
	// Ecualizaci�n del histograma
	EcualizacionHistograma((RGB_Pixel_t*)image_ptr, input_file_header.width, input_file_header.height, (RGB_Pixel_t*)(image_ptr_copy), is_gray_scale);


	// Guardar imagen en escala de grises
	outfile.open(out_filename, ios::out | ios::trunc | ios::binary);

	outfile.write(header, 2);
	outfile.write((char*)(&input_file_header), sizeof(input_file_header));
	outfile.write(buffer_copy, length - header_size);
	outfile.close();
	// Clean up
	delete header;
	delete buffer;
	delete buffer_copy;
}

//Realizar la ecualizaci�n del Histograma en la imagen
void EcualizacionHistograma(RGB_Pixel_t * InImg, unsigned int Image_Width, unsigned int Image_Height, RGB_Pixel_t * OutImg, bool is_gray_scale)
{
	// Declare variables
	int * a_HistogramR = new int[256];
	int * a_HistogramG = new int[256];
	int * a_HistogramB = new int[256];

	int * a_CumulativeHistogramR = new int[256];
	int * a_CumulativeHistogramG = new int[256];
	int * a_CumulativeHistogramB = new int[256];


	int Image_Size = Image_Width * Image_Height;
	int TempVariableR = 0;
	int TempVariableG = 0;
	int TempVariableB = 0;
	// Crear la imagen y ecualizar los punteros a la imagen 
	RGB_Pixel_t* pImageBuff = InImg;
	RGB_Pixel_t* pHistBuff = OutImg;

	memset(a_HistogramR, 0, sizeof(int) * 256);
	memset(a_HistogramG, 0, sizeof(int) * 256);
	memset(a_HistogramB, 0, sizeof(int) * 256);

	//------------------------------------------------------------------------------//
	//Histograma de ecualizacion
	//------------------------------------------------------------------------------//
	// Build Histogram of image
	for (int i = 0; i < Image_Size; i++)
	{
		a_HistogramR[pImageBuff->Comp.Red]++;
		a_HistogramG[pImageBuff->Comp.Green]++;
		a_HistogramB[pImageBuff->Comp.Blue]++;
		pImageBuff++;
	}
	
	// Construir el histograma acumulativo el cual ser� nuestra tabla de lookup
	a_CumulativeHistogramR[0] = a_HistogramR[0];
	a_CumulativeHistogramG[0] = a_HistogramG[0];
	a_CumulativeHistogramB[0] = a_HistogramB[0];

	ofstream out("out_ecualizado\\histograma.out", ios::out | ios::trunc);

	out << "index 0: " << "R =" << a_HistogramR[0] << " G =" << a_HistogramG[0] << " B =" << a_HistogramB[0] << endl;
	

	for (int i = 1; i < 256; i++) {
		a_CumulativeHistogramR[i] = a_CumulativeHistogramR[i - 1] + a_HistogramR[i];
		a_CumulativeHistogramG[i] = a_CumulativeHistogramG[i - 1] + a_HistogramG[i];
		a_CumulativeHistogramB[i] = a_CumulativeHistogramB[i - 1] + a_HistogramB[i];
		out << "index " << i << ": R =" << a_CumulativeHistogramR[i] << " G =" << a_CumulativeHistogramG[i] << " B =" << a_CumulativeHistogramB[i] << endl;
	}

	out.close();

	//Mapear la imagen original para tener un histograma ecualizado (Nota: las divisiones 
	//no pueden omitirse usando este metodo, este calculo ser� lento)
	pImageBuff = InImg;
	for (int i = 0; i < Image_Size; i++) {
		
		TempVariableR = (int)(a_CumulativeHistogramR[(pImageBuff->Comp.Red)] * 255 * 1. / (Image_Size));
		TempVariableG = (int)(a_CumulativeHistogramG[(pImageBuff->Comp.Green)] * 255 * 1./ (Image_Size));
		TempVariableB = (int)(a_CumulativeHistogramB[(pImageBuff->Comp.Blue)] * 255 * 1. / (Image_Size));
		
		if (is_gray_scale) {
			if (TempVariableR > 255) {
				pHistBuff->Comp.Red = (unsigned char)(255);
				pHistBuff->Comp.Green = (unsigned char)(255);
				pHistBuff->Comp.Blue = (unsigned char)(255);
			}
			else {
				pHistBuff->Comp.Red = (unsigned char)(TempVariableR);
				pHistBuff->Comp.Green = (unsigned char)(TempVariableR);
				pHistBuff->Comp.Blue = (unsigned char)(TempVariableR);
			}
		}
		else {
			// en los 3 canales
			if (TempVariableR > 255) {
				pHistBuff->Comp.Red = (unsigned char)(255);
			}
			else {
				pHistBuff->Comp.Red = (unsigned char)(TempVariableR);
			}

			if (TempVariableG > 255) {
				pHistBuff->Comp.Green = (unsigned char)(255);
			}
			else {
				pHistBuff->Comp.Green = (unsigned char)(TempVariableG);
			}

			if (TempVariableB > 255) {
				pHistBuff->Comp.Blue = (unsigned char)(255);
			}
			else {
				pHistBuff->Comp.Blue = (unsigned char)(TempVariableB);
			}
		}
		
		pHistBuff++;
		pImageBuff++;
	}
	//------------------------------------------------------------------------------//
	//Limpiar antes de salir
	//------------------------------------------------------------------------------//
	delete[] a_HistogramR;
	delete[] a_CumulativeHistogramR;
}

//Realizar la ecualizaci�n del Histograma en la imagen
void EcualizacionHistograma8bits(RGB_Pixel_t * InImg, unsigned int Image_Width, unsigned int Image_Height, RGB_Pixel_t * OutImg)
{
	// Declaramos la variable
	int * a_Histogram = new int[256];

	int * a_CumulativeHistogram = new int[256];


	int Image_Size = Image_Width * Image_Height;
	int TempVariableR = 0;
	
	// Crear la imagen y ecualizar los punteros a la imagen 
	RGB_Pixel_t* pImageBuff = InImg;
	RGB_Pixel_t* pHistBuff = OutImg;

	memset(a_Histogram, 0, sizeof(int) * 256);

	//------------------------------------------------------------------------------//
	//Equalise Histogram
	//------------------------------------------------------------------------------//
	// Build Histogram of image
	for (int i = 0; i < Image_Size; i++)
	{
		a_Histogram[pImageBuff->Comp.Red]++;
		pImageBuff++;
	}

	// Construir el histograma acumulativo el cual ser� nuestra tabla de lookup
	a_CumulativeHistogram[0] = a_Histogram[0];

	

	for (int i = 1; i < 256; i++) {
		a_CumulativeHistogram[i] = a_CumulativeHistogram[i - 1] + a_Histogram[i];
	
	}

	//Mapear la imagen original para tener un histograma ecualizado (Nota: las divisiones 
	//no pueden omitirse usando este metodo, este calculo ser� lento)
	pImageBuff = InImg;
	for (int i = 0; i < Image_Size; i++) {

		TempVariableR = (int)(a_CumulativeHistogram[(pImageBuff->Comp.Red)] * 255 * 1. / (Image_Size));
		
		
		if (TempVariableR > 255) {
			pHistBuff->Comp.Red = (unsigned char)(255);
			pHistBuff->Comp.Green = (unsigned char)(255);
			pHistBuff->Comp.Blue = (unsigned char)(255);
		}
		else {
			pHistBuff->Comp.Red = (unsigned char)(TempVariableR);
			pHistBuff->Comp.Green = (unsigned char)(TempVariableR);
			pHistBuff->Comp.Blue = (unsigned char)(TempVariableR);
		}
	

		pHistBuff++;
		pImageBuff++;
	}
	//------------------------------------------------------------------------------//
	//Clean-up Before Exiting
	//------------------------------------------------------------------------------//
	delete[] a_Histogram;
	delete[] a_CumulativeHistogram;
}