#include "helper.h"
#include "Matriz_botb.h"
Matriz_botb::Matriz_botb(void) { }
Matriz_botb::~Matriz_botb() { puts("se llamó al destructor de la clase"); }
Matriz_botb::Matriz_botb(int rows, int cols) { this->rows = rows; this->cols = cols; ptr = new unsigned char[rows * cols]; }
// std out para matriz como tipo
std::ostream& operator << (std::ostream& out, const Matriz_botb & mat) {
	out << "saludos :)" << std::endl;
	out << "A ADSF ASF DAS FDS FDFAF " << std::endl;
	out << "mat.rows : " << mat.rows << std::endl;
	out << "mat.cols : " << mat.cols << std::endl;
	return out;
}
// std out para matriz como puntero
std::ostream& operator << (std::ostream& out, const Matriz_botb *mat) {
	out << "saludos :)" << std::endl;
	out << "mat.rows : " << mat->rows << std::endl;
	out << "mat.cols : " << mat->cols << std::endl;
	return out;
}
