#include "CImg.h"
#include <vector>

using namespace std;
using namespace cimg_library;

typedef CImg<float> IF;
typedef CImg<int> II;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef CImg<float> ImgFloat;


class Bosel {
public:

	Bosel();
	~Bosel();

	static const int pos[3];

	static const int Gx[3][3];

	static const int Gy[3][3];

	void displayAll();
	void binarize(char * filename, int mask[3][3]);
	void convolution(ImgFloat& img, const int mask[3][3], ImgFloat &gradient);
	void mergeA(ImgFloat& gradient, ImgFloat& xGradient, ImgFloat& yGradient);
	void mergeB(ImgFloat& gradient, ImgFloat& xGradient, ImgFloat& yGradient);
};