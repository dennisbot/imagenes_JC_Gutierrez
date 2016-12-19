#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "helper.h"
#include "interpolacion.h"
#include "convolucion.h"

#include <thread>
#include <mutex>

#define db(a) cout << #a << " = " << a << endl;
#define db2(a,b) cout << #a << "= " << a << " " << #b << " = " << b << endl;

using namespace std;

struct profiler
{
	std::string name;
	std::chrono::high_resolution_clock::time_point p;
	profiler(std::string const &n) : name(n), p(std::chrono::high_resolution_clock::now()) { }

	~profiler() {
		using dura = std::chrono::duration<double>;
		auto d = std::chrono::high_resolution_clock::now() - p;
		std::cout << name << ": " << std::chrono::duration_cast<dura>(d).count() << std::endl;
	}
};

#define PROFILE_BLOCK(pbn) profiler _pfinstance(pbn)

int main123() {
	string filename = "rostro.jpg";
	int scale = 10;

	do {
		cout << "PROGRAMA PARA INTERPOLAR UNA IMAGEN" << endl;
		cout << "===================================" << endl;
		cout << "1.- Interpolar" << endl;
		cout << "2.- convolución" << endl;
		cout << "3.- test convolución CPU" << endl;
		cout << "4.- test convolución GPU" << endl;
		cout << "5.- Salir" << endl;
		
		int switch_on;
		cin >> switch_on;

		switch (switch_on) {
			case 1:  {
					cout << "indique el nivel de zoom (2 - 40):" << endl;
					cin >> scale;
					cout << "el scale ingresado es:" << scale << endl;
					interpolacion* interpolar  = new interpolacion(filename, scale);
					interpolar->vecinos_cercanos();
					interpolar->bilinear();
					interpolar->bicubica();
					//cv::namedWindow("mi ventana para mostrar imagenes", cv::WINDOW_AUTOSIZE);
					cv::imshow("Original", interpolar->get_img());
					cv::imshow("Int.Vecinos", interpolar->get_bigger_img_vecinos());
					cv::imshow("Int.bilineal", interpolar->get_bigger_img_bilineal());
					cv::imshow("Int.bicubica", interpolar->get_bigger_img_bicubica());
					cv::waitKey(0);
					delete interpolar;
					break;
				}
			case 2: {
					convolucion* c = new convolucion("lena_grises.bmp", 3);
					c->deteccion_de_bordes();
					cv::imshow("Original", c->get_original());
					cv::imshow("Con Detección de bordes", c->get_output());
					cv::waitKey(0);
					delete c;
					break;
				}
			case 3: {
				for (int input_size = 0; input_size < 4; input_size++) {
					auto input_wh = 4000 + input_size * 2000;
					for (int mask_size = 1; mask_size <= 7; mask_size++) {
						auto mask_wh = 2 * mask_size + 1;
						for (int num_threads = 1; num_threads < 24; num_threads++) {
							std::cout << "start : " << mask_size << std::endl;
							auto start = std::chrono::high_resolution_clock::now();
							// rows, cols, num_threads
							convolucion c(input_wh, input_wh, mask_wh);
							c.run(num_threads);
							auto diff = std::chrono::high_resolution_clock::now() - start;
							using dura = std::chrono::duration<double>;
							auto tiempo = std::chrono::duration_cast<dura>(diff).count();
							std::cout << "end time" << ": " << tiempo << std::endl;
							std::cout << "para MATRIX_INPUT = " << input_wh << ", para MASK = " << mask_wh << " y NUM_THREADS = " << num_threads << endl;
							std::cout << "============================================================" << endl;
							ofstream out("D:\\temp\\__to_plot_" + std::to_string(input_wh) + "x" + std::to_string(input_wh) + ".txt", ios::app);
							out << mask_wh << "\t" << num_threads << "\t" << tiempo << endl;
							out.close();
						}
						cout << "______________________________________________________________________" << endl;
						cout << "______________________________________________________________________" << endl;
					}
				}
				/*puts("ANTES:");
				c.show_matrices();
				c.run(3);
				puts("DESPUES:");
				c.show_matrices();*/
				break;
			}
			default: break;
		}
		if (switch_on == 4) break;

	} while (true);
	
	return 0;
}
