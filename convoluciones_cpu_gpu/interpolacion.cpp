#include "interpolacion.h"

interpolacion::interpolacion() {}

interpolacion::interpolacion(std::string filename) {
	cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE); // the newer cvLoadImage alternative, MATLAB-style function
	test_loaded_image();
}

interpolacion::interpolacion(std::string filename, int scale) {
	this->filename = filename;
	this->scale = scale;
	//printf("valor de grayscale %d\n", cv::IMREAD_GRAYSCALE);
	this->img = cv::imread(filename, cv::IMREAD_GRAYSCALE); // the newer cvLoadImage alternative, MATLAB-style function
	printf("width = %d , height = %d\n", img.cols, img.rows);
	test_loaded_image();
}

int interpolacion::test_loaded_image() {
	if (img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", this->filename);
		error_code = -1;
	}
	if (!img.data) // check if the image has been loaded properly
	{
		fprintf(stderr, "The image has not been loaded properly %s\n", this->filename);
		error_code = -1;
	}
	return error_code;
};

int interpolacion::test_image(cv::Mat cur_img) {
	int error_code = 0;
	if (cur_img.empty())
	{
		fprintf(stderr, "Can not load image\n");
		error_code = -1;
	}
	if (!cur_img.data) // check if the image has been loaded properly
	{
		fprintf(stderr, "The image has not been loaded properly\n");
		error_code = -1;
	}
	return error_code;
};

interpolacion::~interpolacion() {}

void interpolacion::vecinos_cercanos_1D() {
	/*uchar* bigger_img_c = (uchar*)bigger_img.data;
	uchar* img_c = (uchar*)img.data;
	int max_bigger_value = bigger_img.rows * bigger_img.cols;*/
	//printf("max_bigger_value = %d\n", max_bigger_value);

	/*for (int i = 0; i < img.rows; i++) {
	for (int j = 0; j < img.cols; j++) {
	printf("i = %d ; j = %d\n", i, j);
	if (j == 0)
	printf("block i = %d, total_sum = %d\n", i, i * img.cols * scale * scale);
	printf("value = %d\n", i * img.cols * scale * scale + j * scale);
	bigger_img_c[i * img.cols * scale * scale + j * scale] = img_c[i * img.cols + j];
	int times = 0;
	for (int ii = - scale / 2; ii < scale / 2; ii++) {
	for (int jj = -scale / 2; jj < scale / 2; jj++) {
	times++;
	bigger_img_c[getRealPosition(i, ii, j, jj, img, scale)] = img_c[i * img.cols + j];
	}
	}
	if (i + 1 == img.rows && j + 1 == img.cols)
	db(times);
	}
	}*/
}

void interpolacion::vecinos_cercanos() {
	cv::Mat var_bigger_img_vecinos(img.rows * scale, img.cols * scale, CV_8UC1, 192);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			var_bigger_img_vecinos.at<uchar>(i * scale, j * scale) = img.at<uchar>(i, j);
			for (int ii = -scale / 2; ii < scale / 2; ii++) {
				for (int jj = -scale / 2; jj < scale / 2; jj++) {
					if (i * scale + ii >= 0 && i * scale + ii < img.rows * scale &&
						j * scale + jj >= 0 && j * scale + jj < img.cols * scale) {
						var_bigger_img_vecinos.at<uchar>(i * scale + ii, j * scale + jj) = img.at<uchar>(i, j);
					}
				}
			}
		}
	}
	cout << "completó la de vecinos cercanos" << endl;
	this->bigger_img_vecinos = var_bigger_img_vecinos;
}

void interpolacion::bilinear() {
	float value = 0;
	cv::Mat var_bigger_img_bilineal(img.rows * scale, img.cols * scale, CV_8UC1, 192);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			var_bigger_img_bilineal.at<uchar>(i * scale, j * scale) = img.at<uchar>(i, j);
			for (int step_a = 0; step_a < scale; step_a++) {
				for (int step_b = 0; step_b < scale; step_b++) {
					if (i + 1 < img.rows && j + 1 < img.cols && i * scale + step_b < var_bigger_img_bilineal.rows &&
						j * scale + step_a < var_bigger_img_bilineal.cols) {
						value = ((scale - step_a) * (scale - step_b) * img.at<uchar>(i, j) +
							step_a * (scale - step_b) * img.at<uchar>(i, j + 1) +
							(scale - step_a) * step_b * img.at<uchar>(i + 1, j) +
							step_a * step_b * img.at<uchar>(i + 1, j + 1)) * 1. / (scale * scale);
						var_bigger_img_bilineal.at<uchar>(i * scale + step_b, j * scale + step_a) = (uchar)value;
					}
				}
			}
		}
	}
	cout << "completó la bilineal" << endl;
	this->bigger_img_bilineal = var_bigger_img_bilineal;
}

void interpolacion::bicubica() {
	cv::Mat var_bigger_bicubica(img.rows * scale, img.cols * scale, CV_8UC1, 192);
	double value = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			var_bigger_bicubica.at<uchar>(i * scale, j * scale) = img.at<uchar>(i, j);
			for (int step_a = 0; step_a < scale; step_a++) {
				for (int step_b = 0; step_b < scale; step_b++) {
					float p_value = 0;
					for (int n = -1; n <= 2; n++) {
						for (int m = -1; m <= 2; m++) {
							if (i + m >= 0 && i + m < img.rows && j + n < img.cols && j + n >= 0)
								p_value += img.at<uchar>(i + m, j + n) * P(n - step_a * 1. / scale) * P(step_b * 1. / scale - m);
						}
					}
					var_bigger_bicubica.at<uchar>(i * scale + step_b, j * scale + step_a) = p_value;
				}
			}
		}
	}
	cout << "completó la bicubica" << endl;
	bigger_img_bicubica = var_bigger_bicubica;
}


std::string interpolacion::get_filename() {
	return filename;
}
double interpolacion::C(double k) {
	return k < 0 ? 0 : k;
}
double interpolacion::P(double k) {
	return (1. / 6) * (pow(C(k + 2), 3) - 4 * pow(C(k + 1), 3) + 6 * pow(C(k), 3) - 4 * pow(C(k - 1), 3));
}

cv::Mat interpolacion::get_img() {
	int error_code = test_image(this->img);
	if (error_code != -1)
		return this->img;
	throw ("ocurrió un error cargando la imagen");
}

cv::Mat interpolacion::get_bigger_img_vecinos() {
	int error_code = test_image(this->bigger_img_vecinos);
	if (error_code != -1)
		return this->bigger_img_vecinos;
	throw ("ocurrió un error cargando la imagen");
}

cv::Mat interpolacion::get_bigger_img_bilineal() {
	int error_code = test_image(this->bigger_img_bilineal);
	if (error_code != -1)
		return this->bigger_img_bilineal;
	throw ("ocurrió un error cargando la imagen");
}

cv::Mat interpolacion::get_bigger_img_bicubica() {
	int error_code = test_image(this->bigger_img_bicubica);
	if (error_code != -1)
		return this->bigger_img_bicubica;
	throw ("ocurrió un error cargando la imagen");
}