#include "functions.h"
#include "CImg.h"

using namespace std;
using namespace cimg_library;

#define db(a) cout << #a << " = " << a << endl;
#define db2(a,b) cout << #a << " = " << a << " " << #b << " = " << b << endl;

void test_read_header_create_bmp() {
	ifstream in;
	in.open("input\\header.bmp", ios::binary | ios::in);

	int length = 0;

	char *signature_header = new char[2]; //signature, must be 4D42 hex
	printf("size of signature_header %d\n", sizeof(signature_header));

	bmp_header_info input_file_header;
	in.read(signature_header, 2);

	in.read((char*)&input_file_header, sizeof(input_file_header));

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
	//printf("0x%x\n", input_file_header.num_of_colour_plane);
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
	in.close();

	printf("size of FILE antes: %d\n", input_file_header.size_of_file);
	input_file_header.size_of_file = 20 * 20 * 3 + 54;
	printf("size of FILE despues: %d\n", input_file_header.size_of_file);


	printf("number of bits per pixel: %d\n", input_file_header.num_of_bit_per_pix);
	input_file_header.num_of_bit_per_pix = 24;
	printf("number of bits per pixel: %d\n", input_file_header.num_of_bit_per_pix);

	input_file_header.width = 20;
	input_file_header.height = 20;

	ofstream out;
	out.open("input\\out.bmp", ios::out | ios::trunc | ios::binary);
	out.write(signature_header, 2);
	out.write((char*)&input_file_header, sizeof(input_file_header));

	unsigned char G = 0; //G
	unsigned char R = 255; //R
	unsigned char B = 0; //B

	for (size_t i = 0; i < 20 * 20 * 3; i++)
	{
		out.write((char*)&G, sizeof(G));
		out.write((char*)&R, sizeof(R));
		out.write((char*)&B, sizeof(B));
	}
	out.close();
	std::cout << "se ha creado la imagen con exito" << std::endl;
}

void readWriteFile(string infilename, string outfilename) {
	
	ifstream in;
	in.open(infilename, ios::binary | ios::in);
	
	db(infilename);

	char* signature = new char[2];
	bmp_header_info header;
	int header_length = 54;
	
	
	in.seekg(0, ios::end);
	int filesize = in.tellg();
	in.seekg(0, ios::beg);
	

	in.read(signature, 2);
	in.read((char*)&header, sizeof(bmp_header_info));

	/*db(filesize);
	db(header.num_of_bit_per_pix);
	db(header.offset_of_pixel_data);*/

	char *buffer = new char[filesize - header.offset_of_pixel_data];
	
	int palette_size;
	char* palette;

	if (header.num_of_bit_per_pix != 24) {
		// we need to read more
		palette_size = header.offset_of_pixel_data - header_length;
		//db(palette_size);
		palette = new char[palette_size];
		in.read(palette, palette_size);
	}
	else {
		puts("la profundidad es de 24 bits, no es necesario leer la paletas");
	}
	in.read(buffer, filesize - header.offset_of_pixel_data);
	in.close();

	//printf("tam: %d\n", header.size_of_file);
	

	//return;
	// write (escribimos)

	ofstream out;
	
	/*printf("filesize: %d\n", filesize);
	printf("offset : %d\n", header.offset_of_pixel_data);
	printf("a escribir adicionalmente : %d\n", filesize - header.offset_of_pixel_data);*/
	out.open(outfilename, ios::binary | ios::out);
	out.write(signature, 2);
	out.write((char*)&header, sizeof(bmp_header_info));
	//printf("tam header: %d\n", sizeof(bmp_header_info));
	if (header.num_of_bit_per_pix != 24) {
		// we need to read more
		out.write(palette, palette_size);
	}
	else {
		puts("es imagen de profundidad 24, no es necesario escribir la paleta de colores");
	}
	out.write(buffer, filesize - header.offset_of_pixel_data);
	out.close();
	puts("DONE!");


}

int main() {
	/*test();
	return 0;*/
	/*test_read_header_create_bmp();
	return 0;*/

	do {
		int opcion = 0, opcion_depth;
		string in_name, out_name;
		CImg<float> in_img, out_img;

		system("cls");
		puts("PROGRAMA PARA MANIPULAR MAPA DE BITS BMP");
		puts("========================================");
		puts("1.- leer/escribir mapa de bits bmp");
		puts("2.- ecualizacion");
		puts("3.- salir");
		puts("========================================");
		puts("ingrese opci�n:");
		scanf("%d", &opcion);
		vector< pair<string, string> > imagenes;
		
		switch (opcion)
		{
		case 1:

			imagenes.push_back(make_pair("input\\lena_24bits.bmp", "output\\lena_24bits_out.bmp"));
			imagenes.push_back(make_pair("input\\lena_8bits.bmp", "output\\lena_8bits_out.bmp"));
			imagenes.push_back(make_pair("input\\lena_4bits.bmp", "output\\lena_4bits_out.bmp"));
			imagenes.push_back(make_pair("input\\lena_1bit.bmp", "output\\lena_1bit_out.bmp"));

			for (size_t i = 0; i < imagenes.size(); i++) {
				readWriteFile(imagenes[i].first, imagenes[i].second);
				CImg<float> inp(imagenes[i].first.c_str()), out(imagenes[i].second.c_str());
				(inp, out).display();
			}

			break;
		case 2:
			//puts("ecualizaci�n / histograma");
			system("cls");
			puts("elige la profundidad de la imagen a ecualizar");
			puts("========================================");
			puts("1.- 1 bit");
			puts("2.- 4 bits");
			puts("3.- 8 bits");
			puts("4.- 24 bits");
			puts("========================================");
			puts("ingrese opci�n:");

			scanf("%d", &opcion_depth);

			switch (opcion_depth)
			{
				case 1: break;
				case 2: break;
				case 3: 
					in_name = "input\\lena512.bmp", out_name = "out_ecualizado\\lena__.bmp";
					Ecualizar8bits(in_name, out_name);
					break;
				case 4:
					puts("�Desea a escala de grises o 3 canales?");
					puts("======================================");
					puts("1.- Escala de grises");
					puts("2.- 3 canales RGB");
					puts("========================================");
					puts("ingrese opci�n:");

					int opcion_canales;
					scanf("%d", &opcion_canales);
					in_name = "input\\lena_24bits.bmp", out_name = "out_ecualizado\\lena_24bits_out.bmp";
				
					Ecualizar(in_name, out_name, opcion_canales == 1);

					in_img.load(in_name.c_str());
					out_img.load(out_name.c_str());
					
					(in_img, out_img).display();

					break;
				default:
					break;
			}

		default:
			break;
		}
		if (opcion == 3) break;
	} while (true);

	return 0;
}
