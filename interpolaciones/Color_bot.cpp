#include "Color_bot.h"
template<typename T> Color_bot<T>::Color_bot() { 
	R = (T)0;
	G = (T)0;
	B = (T)0;
	alpha = (T)0;
}
template<typename T> Color_bot<T>::~Color_bot() { puts("se ha llamado al destructor de colorbot"); }
