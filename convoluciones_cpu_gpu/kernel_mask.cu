#include <stdio.h>
#include <iostream>
#include <fstream>
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

#define db(a) cout << #a << " = " << a << endl;

#define SIZE 10

#define db(a) cout << #a << " = " << a << endl
#define db2(a, b) cout << #a << " = " << a << " " << #b << " = " << b << endl

using namespace std;

inline bool existe(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

#define CUDA_CALL(cuda_function, ...)  { \
    cudaError_t status = cuda_function(__VA_ARGS__); \
    cudaEnsureSuccess(status, #cuda_function, false, __FILE__, __LINE__); \
}

bool cudaEnsureSuccess(cudaError_t status, const char* status_context_description,
	bool die_on_error, const char* filename, unsigned line_number) {
	if (status_context_description == NULL)
		status_context_description = "";
	if (status == cudaSuccess) {
#if REPORT_CUDA_SUCCESS
		cerr << "Succeeded: " << status_context_description << std::endl << std::flush;
#endif
		return true;
	}
	const char* errorString = cudaGetErrorString(status);
	cerr << "CUDA Error: ";
	if (status_context_description != NULL) {
		cerr << status_context_description << ": ";
	}
	if (errorString != NULL) {
		cerr << errorString;
	}
	else {
		cerr << "(Unknown CUDA status code " << status << ")";
	}
	if (filename != NULL) {

		cerr << filename << ":" << line_number;
	}

	cerr << std::endl;
	if (die_on_error) {
		exit(EXIT_FAILURE);
		// ... or cerr << "FATAL ERROR" << etc. etc.
	}
	return false;
}

__device__ void convolucion(int coordinate, float* d_arr, float* d_result, int width, int len, float* mask, float* dir, float* pos, int lado) {
	float c = 0;
	for (int ii = 0; ii < lado; ii++) {
		for (int jj = 0; jj < lado; jj++) {
			int x = coordinate + width * dir[ii * lado + jj] + pos[jj];
			/*if (x < 12)
				printf("d_arr[x] = %f\n", d_arr[x]);*/
			if (x >= 0 && x < len)
				c += d_arr[x] * mask[ii * lado + jj];
		}
	}
	d_result[coordinate] = c;
	//d_result[coordinate] = d_arr[coordinate];	
}

__global__ void deviceComputeResult(float* d_arr, float* d_result, int width, int len, float* mask, float* dir, float* pos, int lado) {
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if (x * width + y < len)
		convolucion(x * width + y, d_arr, d_result, width, len, mask, dir, pos, lado);
}


void ejecutarGPU(string filename)
{

	float *pmask, *pdir;
	int *ppos, lado;
	
	cv::Mat imagen = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	double ratio = imagen.cols * 1. / imagen.rows;
	cv::Mat imagen_output(imagen.rows, imagen.cols, CV_8UC1);

	dim3 BLOCKS = dim3(32 * 4 + 1, 32 * 4 + 1);
	dim3 THREADS = dim3(32, 32);
	
	for (int sizes = 9; sizes <= 9; sizes++) {
		lado = 2 * sizes + 1;

		ppos = new int[lado];
		pmask = new float[lado * lado];
		pdir = new float[lado * lado];

		int dir_value = -lado / 2;

		for (int idx_pos = 0; idx_pos < lado; idx_pos++) ppos[idx_pos] = dir_value++;

		for (int idx = 0; idx < lado * lado; idx++) pmask[idx] = 1. / (lado * lado);

		dir_value = -lado / 2;

		for (int idx_dir = 0; idx_dir < lado * lado; idx_dir++) {
			if (idx_dir % lado == 0 && idx_dir != 0) dir_value++;
			pdir[idx_dir] = dir_value;
		}

		std::clock_t startt = clock();
		double duration;

		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);

		// depth, numColors, initialize
		int WIDTH = imagen.cols;
		printf("WIDTH = %d\n", WIDTH);

		int HEIGHT = imagen.rows;
		printf("HEIGHT = %d\n", HEIGHT);

		float *arr, *result;
		float *d_arr, *d_result;

		arr = (float*)malloc(WIDTH * HEIGHT * sizeof(float));
		result = (float*)malloc(WIDTH * HEIGHT * sizeof(float));

		cudaMalloc((void**)&d_arr, WIDTH * HEIGHT * sizeof(float));
		cudaMalloc((void**)&d_result, WIDTH * HEIGHT * sizeof(float));

			
		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; j < HEIGHT; j++) {
				arr[i * WIDTH + j] = imagen.at<uchar>(j, i);
				result[i * WIDTH + j] = 0;
			}

		auto the_error = cudaMemcpy(d_arr, arr, WIDTH * HEIGHT * sizeof(float), cudaMemcpyHostToDevice);
		if (the_error != cudaSuccess) {
			fprintf(stderr, "cudaMemcpyHostToDevice arr -> d_arr : cudaMemcpy failed: %s\n",
				cudaGetErrorString(the_error));
			return;
		}
		the_error = cudaMemcpy(d_result, result, WIDTH * HEIGHT * sizeof(float), cudaMemcpyHostToDevice);
		if (the_error != cudaSuccess) {
			fprintf(stderr, "cudaMemcpyHostToDevice result -> d_result : cudaMemcpy failed: %s\n",
				cudaGetErrorString(the_error));
			return;
		}

		float *d_pos, *d_dir, *d_mask;

		cudaMalloc((void**)&d_pos, lado * sizeof(float));
		cudaMalloc((void**)&d_dir, lado * lado * sizeof(float));
		cudaMalloc((void**)&d_mask, lado * lado * sizeof(float));


		cudaMemcpy(d_pos, ppos, lado * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(d_dir, pdir, lado * lado * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(d_mask, pmask, lado * lado * sizeof(float), cudaMemcpyHostToDevice);

		cudaEventRecord(start);

		deviceComputeResult << < BLOCKS, THREADS >> >(d_arr, d_result, WIDTH, WIDTH * HEIGHT, d_mask, d_dir, d_pos, lado);

		std::string error = cudaGetErrorString(cudaPeekAtLastError());

		printf("cudaGetErrorString(cudaPeekAtLastError()) = %s\n", error);
		error = cudaGetErrorString(cudaThreadSynchronize());
		printf("cudaGetErrorString(cudaThreadSynchronize()) = %s\n", error);

		the_error = cudaMemcpy(result, d_result, WIDTH * HEIGHT * sizeof(float), cudaMemcpyDeviceToHost);

		if (the_error != cudaSuccess) {
			fprintf(stderr, "cudaMemcpyDeviceToHost d_result -> result : cudaMemcpy failed: %s\n",
				cudaGetErrorString(the_error));
			return;
		}

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
		printf("imagen con máscara de tamanio %d, cudaEventElapsedTime = %.2f ms\n", lado, milliseconds / 1000);
		
		ofstream out("measurements", ios::out | ios::app);
		out << lado << "\t" << milliseconds / 1000 << endl;
		out.close();

		free(arr);
		free(result);
			
		free(ppos);
		free(pdir);
		free(pmask);

		cudaFree(d_arr);
		cudaFree(d_result);

		cudaFree(d_pos);
		cudaFree(d_dir);
		cudaFree(d_mask);


		cudaEventDestroy(start);
		cudaEventDestroy(stop);

		//acá vamos a realizar una comparación

		//ratio = w / h
		double new_height = 700;
		double new_width = new_height * ratio;

		int fill = 2 - to_string(lado).size();
		cv::imwrite("output/Img_with_kernelSize_" + string(fill > 0 ? fill : 0, '0') + to_string(lado) + "_" + filename, imagen_output);
		/*cvNamedWindow("imagen original", CV_WINDOW_NORMAL);
		cvResizeWindow("imagen original", new_width, new_height);
		cv::imshow("imagen original", imagen);

		cvNamedWindow("imagen con filtro gausiano", CV_WINDOW_NORMAL);
		cvResizeWindow("imagen con filtro gausiano", new_width, new_height);
		cv::imshow("imagen con filtro gausiano", imagen_output);*/
		cv::waitKey(0);
	}
		
}


int main(int argc, char** argv) {
	//ejecutarGPU("lena.jpg");
	ejecutarGPU("4096x4096.jpg");
	
	return 0;
}
