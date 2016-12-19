#ifndef MATRIZ_BOT_CPP
#define MATRIZ_BOT_CPP

#include "helper.h"
#include "Matriz_bot.h"

template<typename T> Matriz_bot<T>::Matriz_bot(void) { }

template<typename T> Matriz_bot<T>::Matriz_bot(int rows, int cols) { 
	this->rows = rows; this->cols = cols; ptr = new T[this->rows * this->cols];
}
template<typename T> Matriz_bot<T>::~Matriz_bot() { }
// std out para matriz como tipo
template<typename T>
ostream& operator << (ostream& out, const Matriz_bot<T> & mat) {
	out << "saludos :)" << std::endl;
	out << "mat.rows : " << mat.rows << std::endl;
	out << "mat.cols : " << mat.cols << std::endl;
	return out;
}

// std out para matriz como puntero
template<typename T>
ostream& operator << (ostream& out, const Matriz_bot<T> *mat) {
	out << "saludos :)" << std::endl;
	out << "mat.rows : " << mat->rows << std::endl;
	out << "mat.cols : " << mat->cols << std::endl;
	return out;
}

#endif