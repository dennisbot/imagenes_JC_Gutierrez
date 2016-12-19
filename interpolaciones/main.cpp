#include <iostream>
#include <string>
#include "helper.h"
#include "Color_bot.h"
#include "Matriz_bot.h"


using namespace std;

int main33() {
	cout << "dasfsf" << endl;
	//Color_bot<T_BYTE> 
	//Matriz_bot< T_BYTE> *Matbot = new Matriz_bot< T_BYTE >(2, 3);
	
	Matriz_bot< T_BYTE > *Matbot = new Matriz_bot< T_BYTE >(2, 3);
	Matriz_bot< T_BYTE > matbot(5, 6);
	/*matbot.cols = 12;
	matbot.rows = 11;*/
	/*
		Matriz_botb *Matbot = new Matriz_botb(2, 3);
		Matriz_botb matbot;
	*/
	cout << Matbot << endl;
	cout << matbot << endl;
	
	puts("CAMBIOS CAMBIOS EVERYWHERE");
	return 0;
}