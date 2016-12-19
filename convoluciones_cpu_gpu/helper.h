#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/miniflann.hpp"

uchar getRealPosition(int i,int ii,int j,int jj,cv::Mat img, int scale) {
	size_t temp = i * img.cols * scale * scale + j * scale + ii + jj * scale * img.cols;
	
	if (temp < img.rows * img.cols * scale * scale && temp >= 0) 
		return temp; 
	else
	{
		printf("POSICION DESBORDE : %d\n", temp);
		printf("=========");
	}
	//cvShowManyImages("dasfsf", 2, img, bigger_img);
	return 0;
};