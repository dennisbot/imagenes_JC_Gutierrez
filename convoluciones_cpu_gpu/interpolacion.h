#include <iostream>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/miniflann.hpp"

using namespace std;

class interpolacion
{
	public:
		interpolacion();
		interpolacion(std::string);
		interpolacion(std::string, int);
		~interpolacion();
		
		void vecinos_cercanos();
		void vecinos_cercanos_1D();
		void bilinear();
		void bicubica();

		std::string get_filename();
		cv::Mat get_img();
		cv::Mat get_bigger_img_vecinos();
		cv::Mat get_bigger_img_bilineal();
		cv::Mat get_bigger_img_bicubica();
	private:
		cv::Mat img;
		cv::Mat bigger_img_vecinos;
		cv::Mat bigger_img_bilineal;
		cv::Mat bigger_img_bicubica;
		std::string filename;
		int scale;
		int error_code;
		
		
		
		int test_loaded_image();
		int test_image(cv::Mat);
		double interpolacion::C(double);
		double interpolacion::P(double);
};

