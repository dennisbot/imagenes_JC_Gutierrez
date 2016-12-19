#ifndef MATRIZ_BOT_H
#define MATRIZ_BOT_H


#include <iostream>
#include <string>

using std::ostream;

template<typename T>
class Matriz_bot {
	public :
		T *ptr;
		int rows;
		int cols;
		// constructores
		Matriz_bot(void);
		Matriz_bot(int, int);
		
		// destructor
		~Matriz_bot();
		template<typename TT>
		friend ostream& operator << (ostream &, const Matriz_bot<TT> &);
		template<typename TT>
		friend ostream& operator << (ostream& out, const Matriz_bot<TT> *);

};

#include "Matriz_bot.cpp"
#endif