#include "convolucion.h"
#include <thread>

const int convolucion::pos[3] = { -1, 0, 1 };

convolucion::convolucion() {}

convolucion::convolucion(std::string imgfilename) {
	load_image(imgfilename);
}
convolucion::convolucion(std::string imgfilename, int mask_size) : convolucion(imgfilename) {
	this->mask_size = mask_size;
}

convolucion::convolucion(int num_rows, int num_cols, int mask_size) {
	this->num_rows = num_rows;
	this->num_cols = num_cols;
	this->mask_size = mask_size;
	this->init_matrix(input, num_rows, num_cols);
	this->init_matrix(output, num_rows, num_cols);
	this->init_matrix(mask, mask_size, mask_size);
	this->fill_matrix_with_random_values(input, 255, num_rows, num_cols);
	//this->fill_matrix_with_random_values(output, num_rows, num_rows);
	this->fill_matrix_with_random_values(mask, 2, mask_size, mask_size);
}

void convolucion::do_thread(int idx,int offset,int global_height,int global_width, int n_threads) {
	int val = 0;
	int local_width = global_width / n_threads;
	int start_idx = idx * local_width;
	
	int right_width_limit = start_idx + local_width < global_width ? start_idx + local_width : global_width;
	
	for (int i = 0; i < global_height; i++) {
		for (int j = start_idx; j < right_width_limit; j++) {
			val = 0;
			for (int u = -offset; u <= offset; u++) {
				for (int v = -offset; v <= offset; v++) {
					if (i + u >= 0 && i + u < global_height && j + v >= 0 && j + v < global_width)
					val += input[i + u][j + v] * 1. * mask[u + offset][v + offset];
				}
			}
			output[i][j] = val;
		}
	}
}

void convolucion::run(int num_threads) {
	if (num_cols == 0 || mask_size == 0) {
		printf("ERROR: Las dimensiones no son válidas ...");
		return;
	}
	int offset = mask_size / 2;
	std::thread* tt = new std::thread[num_threads];
	int i;
	for (i = 0; i < num_threads; i++) {
		tt[i] = std::thread(&convolucion::do_thread, this, i, offset, num_rows, num_cols, num_threads);
	}
	if (num_cols % num_threads != 0) {
		do_thread(i, offset, num_rows, num_cols, num_threads);
	}
	for (int i = 0; i < num_threads; i++) {
		tt[i].join();
	}
	num_threads++;
	printf("num_threads = %d\n", num_threads);
}

void convolucion::init_matrix(int** &mask, int rows, int cols) {
	mask = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		//reservamos memoria
		mask[i] = (int*)malloc(cols * sizeof(int));
		//inicializamos en cero
		memset(mask[i], 0, cols * sizeof(int));
	}
}

void convolucion::fill_matrix_with_random_values(int** &mask, int max_value, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mask[i][j] = rand() % max_value;
			//mask[i][j] = 1;
		}
	}
}

void convolucion::show_matrix_content(int** & mask, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << mask[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void convolucion::show_matrices() {
	puts("INPUT MATRIX");
	puts("============");
	show_matrix_content(this->input, this->num_rows, this->num_cols);

	puts("MASK");
	puts("====");
	show_matrix_content(this->mask, this->mask_size, this->mask_size);

	puts("OUTPUT MATRIX");
	puts("=============");
	show_matrix_content(this->output, this->num_rows, this->num_cols);
	puts("=====================================================================================");
}

void convolucion::deteccion_de_bordes() {
	init_matrix(this->maskA, 3, 3);
	init_matrix(this->maskB, 3, 3);

	init_matrix(Gx, img.rows, img.cols);
	init_matrix(Gy, img.rows, img.cols);

	maskA[0][0] = -1; maskA[0][1] = 0; maskA[0][2] = 1;
	maskA[1][0] = -2; maskA[1][1] = 0; maskA[1][2] = 2;
	maskA[2][0] = -1; maskA[2][1] = 0; maskA[2][2] = 1;

	maskB[0][0] = -1; maskB[0][1] = -2; maskB[0][2] = -1;
	maskB[1][0] = 0; maskB[1][1] = 0; maskB[1][2] = 0;
	maskB[2][0] = 1; maskB[2][1] = 2; maskB[2][2] = 1;

	/*std::cout << "se llama a display matrix content" << std::endl;
	puts("MATRIX mask");
	display_matrix_content(this->mask, mask_size, mask_size);
	puts("MATRIX A");
	display_matrix_content(this->maskA, mask_size, mask_size);
	puts("MATRIX B");
	display_matrix_content(this->maskB, mask_size, mask_size);*/

	for (int i = 1; i < img.rows - 1; i++) {
		for (int j = 1; j < img.cols - 1; j++) {
			Gx[i][j] = 0;
			Gy[i][j] = 0;
			for (int u = 0; u < 3; u++) {
				for (int v = 0; v < 3; v++) {
					Gx[i][j] += img.at<uchar>(i + pos[u], j + pos[v]) * 1. * maskA[u][v];
					Gy[i][j] += img.at<uchar>(i + pos[u], j + pos[v]) * 1. * maskB[u][v];
				}
			}

			double val = sqrt(pow(Gx[i][j], 2) + pow(Gy[i][j], 2));
			//val = abs(Gx[i][j]) + abs(Gy[i][j]);

			//printf("val = %f\n", val);

			out_img.at<uchar>(i, j) = (uchar)val;
		}
	}
}


int convolucion::load_image(std::string imgfilename) {
	this->imgfilename = imgfilename;
	img = cv::imread(imgfilename, cv::IMREAD_GRAYSCALE); // the newer cvLoadImage alternative, MATLAB-style function
	cv::Mat o(img.rows, img.cols, CV_8UC1);
	out_img = o;
	printf("width = %d , height = %d\n", img.cols, img.rows);
	if (img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", imgfilename);
		error_code = -1;
	}
	if (!img.data) // check if the image has been loaded properly
	{
		fprintf(stderr, "The image has not been loaded properly %s\n", imgfilename);
		error_code = -1;
	}
	return error_code;
}
convolucion::~convolucion() {
	if (img.cols != 0) {
		for (int j = 0; j < img.cols; j++) {
			free(Gx[j]);
			free(Gy[j]);
		}
		free(Gx);
		free(Gy);
		for (int j = 0; j < 3; j++) {
			free(maskA[j]);
		}
		for (int j = 0; j < 3; j++) {
			free(maskB[j]);
		}
	}
	// para trabajar con máscaras en general
	input;
	for (int j = 0; j < num_cols; j++) {
		free(input[j]);
		free(output[j]);
	}
	free(input);
	free(output);
	for (int j = 0; j < mask_size; j++) {
		free(mask[j]);
	}
	free(mask);
	puts("entro a free");
}
cv::Mat convolucion::get_original() {
	return img;
}
cv::Mat convolucion::get_output() {
	return out_img;
}
