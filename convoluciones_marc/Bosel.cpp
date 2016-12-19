#include "Bosel.h"



Bosel::Bosel() {};

Bosel::~Bosel() {};

const int Bosel::pos[3] = { -1, 0, 1 };
const int Bosel::Gx[3][3] = {
	{ -1, 0, 1 },
	{ -2, 0, 2 },
	{ -1, 0, 1 }
};
const int Bosel::Gy[3][3] = {
	{ 1, 2, 1 },
	{ 0, 0, 0 },
	{ -1, -2, -1 }
};

void Bosel::binarize(char * filename, int mask[3][3])
{
	IF img(filename);
	II tmp(img.width(), img.height(), 1, 1, 0);

	for (int i = 1; i < img.width() - 1; i++)
		for (int j = 1; j < img.height() - 1; j++){
			float avg = img(i, j, 0) + img(i, j, 1) + img(i, j, 2);
			avg /= 3;
			if (avg < 50) {
				tmp(i, j, 0) = 0;
			}
			else {
				tmp(i, j, 0) = 1;
			}
		}
	tmp.display();

}

void Bosel::convolution(ImgFloat& img, const int mask[3][3], ImgFloat &gradient)
{
	for (int i = 1; i < img.width() - 1; i++)
		for (int j = 1; j < img.height() - 1; j++) {
			float c = 0;
			for (int ii = 0; ii < 3; ii++)
			{ 
				for (int jj = 0; jj < 3; jj++)
				{
					c += img(i + pos[ii], j + pos[jj]) * mask[pos[ii] + 1][pos[jj] + 1];
				}
			}
			gradient(i, j) = c;
		}
}

void Bosel::mergeA(ImgFloat& gradient, ImgFloat& xGradient, ImgFloat& yGradient) {
	for (size_t i = 0; i < gradient.width(); i++)
	{
		for (size_t j = 0; j < gradient.height(); j++)
		{
			gradient(i, j) = abs(xGradient(i, j)) + abs(yGradient(i, j));
		}
	}
}
void Bosel::mergeB(ImgFloat& gradient, ImgFloat& xGradient, ImgFloat& yGradient) {
	for (size_t i = 0; i < gradient.width(); i++)
	{
		for (size_t j = 0; j < gradient.height(); j++)
		{
			
			gradient(i, j) = sqrt(pow(xGradient(i, j), 2) + pow(yGradient(i, j), 2));
		}
	}
}