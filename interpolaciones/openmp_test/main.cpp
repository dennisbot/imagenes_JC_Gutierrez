#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <omp.h>
#include <ctime>

using namespace std;

int main() {
	omp_set_num_threads(8);
	puts("probando openmp");
	#pragma omp parallel for
	for (int i = 0; i < 64; i++) {
		printf("core #%d = %d\n", omp_get_thread_num(), i);
	}
	puts("este lupulo si pasa");

	return 0;
}