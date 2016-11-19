//------------------------------------------------------------------------------//
//This C++ program is designed to open a 24-bit Bitmap and perform a Histogram
//Equalisation on the image.
//
//Usage: change the 'in_filename' string to match your input image
//------------------------------------------------------------------------------//
#include "functions.h"

#define db(a) cout << #a << " = " << a << endl;
#define db2(a,b) cout << #a << " = " << a << " " << #b << " = " << b << endl;

using namespace std;

int main_prueba() {

	bmp_header_info input_file_header;
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

int main22()
{
	//db(sizeof(bmp_header_info));
	//return 0;
	string base_path = "C:\\Users\\dennisbot\\Pictures\\linux\\";
	string filename_in = "lena24.bmp";
	string filename_out = "lena24_out.bmp";
	string full_path_in = base_path + filename_in;
	string full_path_out = base_path + filename_out;
	//Setup buffers to read in file
	bmp_header_info input_file_header;
	long length;
	char* header;
	char* buffer;
	int header_size = 54;
	ifstream infile;
	ofstream outfile;
	
	// Cambiar esta cadena para que quepe con la imagen
	string in_filename(full_path_in);
	infile.open(in_filename, ios::binary | ios::in);
	
	// obtener el tamaño del archivo:
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

	infile.read(buffer, length - header_size);
	infile.close();
	
	// hacemos algunas copias del buffer
	char *buffer_copy = new char[length - header_size + 3];
	char *image_ptr_copy = buffer_copy;
	char *image_ptr = buffer;
	char *header_ptr = header;
	cout << "Image Height: " << input_file_header.height << "\n";
	cout << "Image Width : " << input_file_header.width << "\n";
	
	// Ecualización del histograma
	EcualizacionHistograma((RGB_Pixel_t*)image_ptr, input_file_header.width, input_file_header.height, (RGB_Pixel_t*)(image_ptr_copy + 2));
	
	
	// Guardar imagen en escala de grises
	outfile.open(full_path_out, ios::out | ios::trunc | ios::binary);

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