#include<iostream>
#include<string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/flann/miniflann.hpp"

#define db(a) cout << #a << " = " << a << endl;

class convolucion
{
public:
	convolucion();
	convolucion(std::string);
	convolucion(std::string, int);
	// esta se usará para los test de convolución en serie
	convolucion(int, int, int);
	~convolucion();

	std::string get_img_filename();
	void deteccion_de_bordes();
	cv::Mat get_output();
	cv::Mat get_original();

	void convolucion::show_matrices();
	void run(int);

private:
	static const int pos[3];

	// para trabajar con detección de bordes
	std::string imgfilename;
	cv::Mat img;
	cv::Mat out_img;
	int** Gx;
	int** Gy;
	int** maskA;
	int** maskB;

	// para trabajar con máscaras en general
	int** input;
	int** output;
	int num_rows;
	int num_cols;

	int** mask;
	int mask_size;

	int error_code;
	int load_image(std::string);
	void show_matrix_content(int** &, int, int);
	void init_matrix(int** &, int, int);
	void fill_matrix_with_random_values(int** &,int, int, int);
	void do_thread(int, int, int, int, int);
};