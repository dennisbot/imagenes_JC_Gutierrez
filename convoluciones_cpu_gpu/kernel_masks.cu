#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>

#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <sys/stat.h>
//#include "math_functions.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/miniflann.hpp"

//#define cimg_use_jpeg


#define SIZE 10

#define db(a) cout << #a << " = " << a << endl
#define db2(a, b) cout << #a << " = " << a << " " << #b << " = " << b << endl

using namespace std;

inline bool existe(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

__device__ void convolution(int coordinate, float* d_arr, float* d_result, int width, int len, int* mask, int* dir, int* pos)
{
	float c = 0;
	for (int ii = 0; ii < 3; ii++)
	{
		for (int jj = 0; jj < 3; jj++)
		{
			int x = coordinate + width * dir[ii * 3 + jj] + pos[jj];
			if (x >= 0 && x < len)
				c += d_arr[x] * mask[ii * 3 + jj];
		}
	}
	/*printf("abs(cx) + abs(cy) = %f\n", abs(cx) + abs(cy));
	printf("sqrt(pow(cx, 2) + pow(cy, 2)) = %f\n", sqrt(pow(cx, 2.f) + pow(cy, 2.f)));*/
	d_result[coordinate] = c;
}

__global__ void deviceComputeGaussian(float* d_arr, float* d_result, int width, int len, int* mask, int* dir, int* pos) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if (x == 0 && y < 10)
		printf("[%d %d]\n", x, y);
	if (x * width + y < len)
		convolution(x * width + y, d_arr, d_result, width, len, mask, dir, pos);
}


void performGPU(string filename)
{
	cv::Mat imagen = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	cv::Mat imagen_output(imagen.rows, imagen.cols, CV_8UC1);

	std::clock_t startt = clock();
	double duration;
	

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	// depth, numColors, initialize
	int WIDTH = imagen.cols;
	int HEIGHT = imagen.rows;
	float *arr, *result;
	float *d_arr, *d_result;

	arr = (float*)malloc(WIDTH * HEIGHT * sizeof(float));
	result = (float*)malloc(WIDTH * HEIGHT * sizeof(float));
	

	cudaMalloc((void**)&d_arr, WIDTH * HEIGHT * sizeof(float));
	cudaMalloc((void**)&d_result, WIDTH * HEIGHT * sizeof(float));

	int pos[3] = { -1, 0, 1 };
	int dir[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int mask[9] = {
		1. / 9, 1. / 9, 1. / 9,
		1. / 9, 1. / 9, 1. / 9,
		1. / 9, 1. / 9, 1. / 9
	};

	int* d_pos, *d_dir, *d_mask;

	cudaMalloc((void**)&d_pos, 3 * sizeof(int));
	cudaMalloc((void**)&d_dir, 9 * sizeof(int));
	cudaMalloc((void**)&d_mask, 9 * sizeof(int));
	

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			arr[i * WIDTH + j] = imagen.at<uchar>(j, i);
			result[i * WIDTH + j] = 0;
		}
	}

	cudaMemcpy(d_arr, arr, WIDTH * HEIGHT * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_result, result, WIDTH * HEIGHT * sizeof(float), cudaMemcpyHostToDevice);

	dim3 BLOCKS = dim3(1024, 1024);
	dim3 THREADS = dim3(4, 4);

	/*dim3 BLOCKS(2, 2);
	dim3 THREADS(2, 2);*/

	

	cudaMemcpy(d_pos, pos, 3 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_dir, dir, 9 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_mask, mask, 9 * sizeof(float), cudaMemcpyHostToDevice);


	cudaEventRecord(start);
	deviceComputeGaussian << < BLOCKS, THREADS >> > (d_arr, d_result, WIDTH, WIDTH * HEIGHT, d_mask, d_dir, d_pos);
	cudaMemcpy(result, d_result, WIDTH * HEIGHT * sizeof(float), cudaMemcpyDeviceToHost);
	cudaEventRecord(stop);

	cudaEventSynchronize(stop);

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			imagen_output.at<uchar>(j, i) = result[i * WIDTH + j];
		}
	}

	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	duration = (std::clock() - startt) / (double)CLOCKS_PER_SEC;

	printf("GPU for image %s takes %.2f seconds\n", filename.c_str(), duration);
	printf("cudaEventElapsedTime = %.2f ms\n", milliseconds);

	free(arr);


	cudaFree(d_arr);
	cudaFree(d_result);

	cudaFree(d_pos);
	cudaFree(d_dir);
	cudaFree(d_result);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	//acá mostramos la imagen resultante
	cv::imshow("Desenfoque Gaussiano", imagen_output);
	cv::waitKey(0);
}


int main(int argc, char** argv) {
	puts("hola :)");
	performGPU("lena.jpg");
	return 0;
}
