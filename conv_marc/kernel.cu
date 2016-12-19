#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Bosel.h"

#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <sys/stat.h>

//#define cimg_use_jpeg
#include "CImg.h"

#define SIZE 10

#define db(a) cout << #a << " = " << a << endl
#define db2(a, b) cout << #a << " = " << a << " " << #b << " = " << b << endl

inline bool existe(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void performCPU(string filename)
{
	std::clock_t start = clock();
	double duration;
	if (existe(filename)) {
		puts("EL ARCHIVO SI EXISTE");
	}
	else {
		puts("EL ARCHIVO NO EXISTE");
	}
	ImgFloat imagen(filename.c_str());
	//ImgFloat imagen("lena30.jpg");
	// depth, numColors, initialize
	ImgFloat xGradient(imagen.width(), imagen.height(), 1, 1, 0);
	ImgFloat yGradient(imagen.width(), imagen.height(), 1, 1, 0);
	ImgFloat gradientA(imagen.width(), imagen.height(), 1, 1, 0);
	ImgFloat gradientB(imagen.width(), imagen.height(), 1, 1, 0);

	//imagen.blur(1.5);

	ImgFloat R = imagen.get_channel(0);

	Bosel b;
	b.convolution(R, b.Gx, xGradient);
	b.convolution(R, b.Gy, yGradient);

	b.mergeA(gradientA, xGradient, yGradient);
	b.mergeB(gradientB, xGradient, yGradient);

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	printf("CPU for image %s takes %.2f seconds\n", filename.c_str(), duration);

	(gradientA, gradientB).display("comparación suma ABSs y SQRT");
	//(xGradient, yGradient, gradient).display("Detección de Bordes");
	cout << duration << endl;
	//gradient.display();
}

__device__ void convolution(int coordinate, float* d_arr, float* gradient, int width, int len, int* mask, int* dir, int* pos)
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
	gradient[coordinate] = c;
}

__global__ void deviceComputeGradient(float* d_arr, float* gradient, int width, int len, int* mask, int* dir, int* pos) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if (x * width + y < len)
		convolution(x * width + y, d_arr, gradient, width, len, mask, dir, pos);
}

__global__ void deviceMerge(float* xGradient, float* yGradient, float* target, int width, int len) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	int index = x * width + y;
	if (index < len)
		target[index] = abs(xGradient[index]) + abs(yGradient[index]);
}

//segunda versión (un sólo método)
__device__ void convolution2(int coordinate, float* d_arr, float* gradient, int width, int len, int* gx_mask, int* gy_mask, int* dir, int* pos)
{
	float cx = 0, cy = 0;
	for (int ii = 0; ii < 3; ii++)
	{
		for (int jj = 0; jj < 3; jj++)
		{
			int x = coordinate + width * dir[ii * 3 + jj] + pos[jj];
			if (x >= 0 && x < len) {
				cx += d_arr[x] * gx_mask[ii * 3 + jj];
				cy += d_arr[x] * gy_mask[ii * 3 + jj];
			}
		}
	}
	gradient[coordinate] = abs(cx) + abs(cy);
}

__global__ void deviceComputeGradient2(float* d_arr, float* gradient, int width, int len, int* gx_mask, int* gy_mask, int* dir, int* pos) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if (x == 0 && y < 10)
		printf("[%d %d]\n", x, y);
	if (x * width + y < len)
		convolution2(x * width + y, d_arr, gradient, width, len, gx_mask, gy_mask, dir, pos);
}


void performGPU(string filename)
{
	ImgFloat imagen(filename.c_str());

	std::clock_t startt = clock();
	double duration;
	ImgFloat result(imagen.width(), imagen.height(), 1, 1, 0);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	// depth, numColors, initialize
	int WIDTH = imagen.width();
	int HEIGHT = imagen.height();
	float *arr, *gradient;
	float *d_arr, *d_gradient;

	arr = (float*)malloc(WIDTH * HEIGHT * sizeof(float));
	gradient = (float*)malloc(WIDTH * HEIGHT * sizeof(float));

	cudaMalloc((void**)&d_arr, WIDTH * HEIGHT * sizeof(float));
	cudaMalloc((void**)&d_gradient, WIDTH * HEIGHT * sizeof(float));

	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++) {
			arr[i * WIDTH + j] = imagen(i, j);
			gradient[i * WIDTH + j] = 0;
		}

	cudaMemcpy(d_arr, arr, WIDTH * HEIGHT * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_gradient, gradient, WIDTH * HEIGHT * sizeof(float), cudaMemcpyHostToDevice);

	dim3 BLOCKS = dim3(1024, 1024);
	dim3 THREADS = dim3(4, 4);

	/*dim3 BLOCKS(2, 2);
	dim3 THREADS(2, 2);*/

	int pos[3] = { -1, 0, 1 };
	int dir[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int Gx[9] = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};
	int Gy[9] = {
		1, 2, 1,
		0, 0, 0,
		-1, -2, -1
	};
	int* d_pos, *d_dir, *d_Gx, *d_Gy;

	cudaMalloc((void**)&d_pos, 3 * sizeof(int));
	cudaMalloc((void**)&d_dir, 9 * sizeof(int));
	cudaMalloc((void**)&d_Gx, 9 * sizeof(int));
	cudaMalloc((void**)&d_Gy, 9 * sizeof(int));

	cudaMemcpy(d_pos, pos, 3 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_dir, dir, 9 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_Gx, Gx, 9 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_Gy, Gy, 9 * sizeof(float), cudaMemcpyHostToDevice);


	cudaEventRecord(start);
	deviceComputeGradient2 << < BLOCKS, THREADS >> > (d_arr, d_gradient, WIDTH, WIDTH * HEIGHT, d_Gx, d_Gy, d_dir, d_pos);
	cudaMemcpy(gradient, d_gradient, WIDTH * HEIGHT * sizeof(float), cudaMemcpyDeviceToHost);
	cudaEventRecord(stop);

	cudaEventSynchronize(stop);

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			result(i, j) = gradient[i * WIDTH + j];
		}
	}


	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	duration = (std::clock() - startt) / (double)CLOCKS_PER_SEC;

	printf("GPU for image %s takes %.2f seconds\n", filename.c_str(), duration);
	printf("cudaEventElapsedTime = %.2f ms\n", milliseconds);

	free(arr);


	cudaFree(d_arr);
	cudaFree(d_gradient);

	cudaFree(d_pos);
	cudaFree(d_dir);
	cudaFree(d_Gx);
	cudaFree(d_Gy);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	//acá si quieres ponemos en xgradient, ygradient
	(result).display("HOLA MUNDO CUDA");
}


int main(int argc, char** argv) {
	CImg<unsigned char> image("lena_grises.bmp"), visu(500, 400, 1, 3, 0);
	const unsigned char red[] = { 255, 0, 0 }, green[] = { 0, 255, 0 }, blue[] = { 0, 0, 255 };
	image.blur(2.5);
	CImgDisplay main_disp(image, "Click a point"), draw_disp(visu, "Intensity profile");
	while (!main_disp.is_closed() && !draw_disp.is_closed()) {
		main_disp.wait();
		if (main_disp.button() && main_disp.mouse_y() >= 0) {
			const int y = main_disp.mouse_y();
			visu.fill(0).draw_graph(image.get_crop(0, y, 0, 0, image.width() - 1, y, 0, 0), red, 1, 1, 0, 255, 0);
			visu.draw_graph(image.get_crop(0, y, 0, 1, image.width() - 1, y, 0, 1), green, 1, 1, 0, 255, 0);
			visu.draw_graph(image.get_crop(0, y, 0, 2, image.width() - 1, y, 0, 2), blue, 1, 1, 0, 255, 0).display(draw_disp);
		}
	}
	return 0;


	performCPU("lena.jpg");
	return 0;
	for (int i = 1; i < 2; i++)
	{
		int len = 4;
		int baseSize = 1024;
		for (int j = 1; j <= len; j++)
		{
			ostringstream stream;
			stream << (baseSize * j);
			if (i == 0)
				performCPU(stream.str() + "x" + stream.str() + ".jpg");
			else
				performGPU(stream.str() + "x" + stream.str() + ".jpg");
		}
	}

	return 0;
}
