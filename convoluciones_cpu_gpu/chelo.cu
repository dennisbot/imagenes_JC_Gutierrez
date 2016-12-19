//#include "opencv2/highgui/highgui.hpp"
//#include <cstdio>
//#include <time.h>
//#include <sstream>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//#define MAX_THREADS_BY_BLOCK 1024
//#define DIM_BLOCK_X 32
//#define DIM_BLOCK_Y 32
//
//__device__ int cuGPos(int y, int x, int cuCols) {
//	return y * cuCols + x;
//}
//
//__global__ void cudaConvolutionImage(int *cuPoRows, int *cuPoCols, int *cuInImage, int *cuResImage, int* sizeKer, float* kernel){
//	int threadIdGlob = threadIdx.x + blockIdx.x * blockDim.x;
//
//	int cuRows = *cuPoRows;
//	int cuCols = *cuPoCols;
//
//	if (threadIdGlob < cuRows * cuCols) {
//		int y = threadIdGlob / cuCols;
//		int x = threadIdGlob % cuCols;
//		float pixel = 0;
//		for (int i = (*sizeKer) / -2; i <= (*sizeKer) / 2; i++) {
//			for (int j = (*sizeKer) / -2; j <= (*sizeKer) / 2; j++) {
//				if (y + j >= 0 && x + i >= 0) {
//					pixel += cuInImage[cuGPos(y + j, x + i, cuCols)] * kernel[(j + (*sizeKer / 2)*(*sizeKer) + (i + (*sizeKer / 2)))];
//				}
//			}
//		}
//
//		pixel = pixel > 255 ? 255 : pixel;
//		pixel = pixel < 0 ? 0 : pixel;
//		cuResImage[cuGPos(y, x, cuCols)] = pixel;
//		if (cuResImage[cuGPos(y, x, cuCols)] == 0 && x == 0 && y == 0)
//			printf("es igual de cero\n");
//	}
//}
//
//Mat generateConvolutionCUDAGrayImage(Mat inMatIn, float** kernel, int siKe, string nameFile) {
//	Mat inMatImage = inMatIn.clone();
//	float time = 0.0;
//	int nRows = inMatImage.rows;
//	int nCols = inMatImage.cols;
//
//	cudaEvent_t start, stop;
//	cudaEventCreate(&start);
//	cudaEventCreate(&stop);
//
//	int* inImage = new int[nRows * nCols];
//	float* kerArray = new float[siKe * siKe];
//
//	for (int x = 0; x < nCols; x++)
//		for (int y = 0; y < nRows; y++)
//			inImage[y*nCols + x] = 0;
//
//	// split in RedGreenBlue channels
//	for (int y = 0; y < nRows; y++){
//		for (int x = 0; x < nCols; x++){
//			inImage[y * nCols + x] = inMatImage.at<uchar>(y, x);
//		}
//	}
//
//	//gen array1d of kernel
//	for (int i = 0; i < siKe; i++)
//		for (int j = 0; j< siKe; j++){
//			kerArray[i*siKe + j] = kernel[i][j];
//		}
//
//	int *cuPoRows, *cuPoCols, *cuN, *cuInImage, *cuResImage;
//	float* cuKernel;
//
//
//	cudaEventRecord(start, 0);
//	cudaMalloc((void**)&cuPoRows, sizeof(int));
//	cudaMalloc((void**)&cuPoCols, sizeof(int));
//	cudaMalloc((void**)&cuN, sizeof(int));
//	cudaMalloc((void**)&cuKernel, siKe * siKe * sizeof(float));
//	cudaMalloc((void**)&cuInImage, nCols * nRows * sizeof(int));
//	cudaMalloc((void**)&cuResImage, nCols * nRows * sizeof(int));
//
//	cudaMemcpy(cuPoRows, &nRows, sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuPoCols, &nCols, sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuN, &siKe, sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuKernel, kerArray, siKe * siKe * sizeof(float), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuInImage, inImage, nCols * nRows * sizeof(int), cudaMemcpyHostToDevice);
//
//	int N = nRows * nCols;
//	dim3 blockDim(MAX_THREADS_BY_BLOCK, 1, 1);
//	dim3 gridDim((N + MAX_THREADS_BY_BLOCK - 1) / MAX_THREADS_BY_BLOCK, 1, 1);
//
//	cudaConvolutionImage << <gridDim, blockDim >> >(cuPoRows, cuPoCols, cuInImage, cuResImage, cuN, cuKernel);
//
//	cudaMemcpy(inImage, cuResImage, nRows * nCols * sizeof(int), cudaMemcpyDeviceToHost);
//
//	cudaFree(cuPoRows);
//	cudaFree(cuPoCols);
//	cudaFree(cuN);
//	cudaFree(cuKernel);
//	cudaFree(cuInImage);
//	cudaFree(cuResImage);
//
//	cudaEventRecord(stop, 0);
//	cudaEventSynchronize(stop);
//	cudaEventElapsedTime(&time, start, stop);
//	cudaEventDestroy(start);
//	cudaEventDestroy(stop);
//
//	for (int y = 0; y < nRows; y++)
//		for (int x = 0; x < nCols; x++)
//			inMatImage.at<uchar>(y, x) = inImage[y*nCols + x];
//
//	printf("%f \t", time / 1000.0);
//	return inMatImage;
//}
//
//Mat generateConvolutionCUDARGBImage(Mat inMatImage, float** kernel, int siKe, string nameFile){
//	float time = 0.0;
//	int nRows = inMatImage.rows;
//	int nCols = inMatImage.cols;
//
//	cudaEvent_t start, stop;
//	cudaEventCreate(&start);
//	cudaEventCreate(&stop);
//
//	int** inImage = new int*[3];
//	float* kerArray = new float[siKe * siKe];
//	for (int i = 0; i < 3; i++)
//		inImage[i] = new int[nRows * nCols];
//
//	Mat bgr[3];
//	split(inMatImage, bgr);
//
//	// split in RedGreenBlue channels
//	for (int y = 0; y < nRows; y++){
//		for (int x = 0; x < nCols; x++){
//			inImage[0][y * nCols + x] = bgr[0].at<uchar>(y, x);
//			inImage[1][y * nCols + x] = bgr[1].at<uchar>(y, x);
//			inImage[2][y * nCols + x] = bgr[2].at<uchar>(y, x);
//		}
//	}
//
//	//gen array1d of kernel
//	for (int i = 0; i < siKe; i++)
//		for (int j = 0; j< siKe; j++)
//			kerArray[i*siKe + j] = kernel[i][j];
//
//	int *cuPoRows, *cuPoCols, *cuN;
//	int *cuInImageR, *cuInImageG, *cuInImageB;
//	int *cuResImageR, *cuResImageG, *cuResImageB;
//	float *cuKernel;
//
//	cudaEventRecord(start, 0);
//	cudaMalloc((void**)&cuPoRows, sizeof(int));
//	cudaMalloc((void**)&cuPoCols, sizeof(int));
//	cudaMalloc((void**)&cuN, sizeof(int));
//	cudaMalloc((void**)&cuKernel, siKe * siKe * sizeof(float));
//	cudaMalloc((void**)&cuInImageR, nCols * nRows * sizeof(int));
//	cudaMalloc((void**)&cuInImageG, nCols * nRows * sizeof(int));
//	cudaMalloc((void**)&cuInImageB, nCols * nRows * sizeof(int));
//	cudaMalloc((void**)&cuResImageR, nCols * nRows * sizeof(int));
//	cudaMalloc((void**)&cuResImageG, nCols * nRows * sizeof(int));
//	cudaMalloc((void**)&cuResImageB, nCols * nRows * sizeof(int));
//
//	cudaMemcpy(cuPoRows, &nRows, sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuPoCols, &nCols, sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuN, &siKe, sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuKernel, kerArray, siKe * siKe * sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuInImageB, inImage[0], nCols * nRows * sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuInImageG, inImage[1], nCols * nRows * sizeof(int), cudaMemcpyHostToDevice);
//	cudaMemcpy(cuInImageR, inImage[2], nCols * nRows * sizeof(int), cudaMemcpyHostToDevice);
//
//	int N = nRows * nCols;
//	int nBloq = (N + MAX_THREADS_BY_BLOCK - 1) / MAX_THREADS_BY_BLOCK;
//
//	dim3 blockDim(DIM_BLOCK_X, DIM_BLOCK_Y, 1);
//	dim3 gridDim(nBloq, 1, 1);
//
//	cudaConvolutionImage << <gridDim, blockDim >> >(cuPoRows, cuPoCols, cuInImageB, cuResImageB, cuN, cuKernel);
//	cudaConvolutionImage << <gridDim, blockDim >> >(cuPoRows, cuPoCols, cuInImageG, cuResImageG, cuN, cuKernel);
//	cudaConvolutionImage << <gridDim, blockDim >> >(cuPoRows, cuPoCols, cuInImageR, cuResImageR, cuN, cuKernel);
//
//	cudaMemcpy(cuInImageB, inImage[0], nRows * nCols * sizeof(int), cudaMemcpyDeviceToHost);
//	cudaMemcpy(cuInImageG, inImage[1], nRows * nCols * sizeof(int), cudaMemcpyDeviceToHost);
//	cudaMemcpy(cuInImageR, inImage[2], nRows * nCols * sizeof(int), cudaMemcpyDeviceToHost);
//
//	cudaFree(cuPoRows);
//	cudaFree(cuPoCols);
//	cudaFree(cuN);
//	cudaFree(cuKernel);
//	cudaFree(cuInImageB);
//	cudaFree(cuInImageG);
//	cudaFree(cuInImageR);
//	cudaFree(cuResImageB);
//	cudaFree(cuResImageG);
//	cudaFree(cuResImageR);
//
//	cudaEventRecord(stop, 0);
//	cudaEventSynchronize(stop);
//	cudaEventElapsedTime(&time, start, stop);
//	cudaEventDestroy(start);
//	cudaEventDestroy(stop);
//
//	for (int y = 0; y < nRows; y++)
//		for (int x = 0; x < nCols; x++){
//			bgr[0].at<uchar>(y, x) = inImage[0][y*nCols + x];
//			bgr[1].at<uchar>(y, x) = inImage[0][y*nCols + x];
//			bgr[2].at<uchar>(y, x) = inImage[0][y*nCols + x];
//		}
//	merge(bgr, 3, inMatImage);
//	printf("Convolución de CUDA RGB Image:%s Mascara: %d  Tiempo %f\n", nameFile.c_str(), siKe, time / 1000.0);
//	return inMatImage;
//}
//
//string intToString(int n){
//	ostringstream ss;
//	ss << n;
//	string a = n <= 9 ? "0" : "";
//	return a + ss.str();
//}
//
//
//int main111() {
//	string path = "D://temp//convolution//";
//	string imagens[15] = { "gordo.jpg"};
//	string dirOut[15] = { "tekken"};
//
//	//	string imagens[2] = {"wallPaper08.jpeg"};
//	//	string dirOut[1] = {"wallPaper08"};
//
//	int arrayKernel[13] = { 3, 5, 9, 13, 19, 25, 31, 39, 47, 57, 67, 75, 85 };
//	int nImages = 15, nKernels = 13;
//
//	for (int i = 0; i < nImages; i++) {
//		Mat inMatGrayImage = imread(path + imagens[i], CV_LOAD_IMAGE_GRAYSCALE);
//		printf("%s,%dx%d,%ld \n", imagens[i].c_str(), inMatGrayImage.rows, inMatGrayImage.cols, (long)inMatGrayImage.cols * (long)inMatGrayImage.rows);
//		imwrite(path + dirOut[i] + "/grayScale" + imagens[i], inMatGrayImage);
//		for (int j = 0; j < nKernels; j++) {
//			float **kernel = new float*[arrayKernel[j]];
//			for (int k = 0; k < arrayKernel[j]; k++) {
//				kernel[k] = new float[arrayKernel[j]];
//				for (int l = 0; l < arrayKernel[j]; l++)
//					kernel[k][l] = 1.0 / (arrayKernel[j] * arrayKernel[j]);
//			}
//			Mat outMatImage = generateConvolutionCUDAGrayImage(inMatGrayImage, kernel, arrayKernel[j], imagens[i]);
//			string fullpath = path + dirOut[i] + "//grayCuda_" + intToString(arrayKernel[j]) + "_" + imagens[i];
//			cout << "kernel : " << arrayKernel[j] << " - fullpath : " << fullpath << endl;
//			imwrite(fullpath, outMatImage);
//		}
//		printf("\n");
//	}
//
//	return 0;
//}
