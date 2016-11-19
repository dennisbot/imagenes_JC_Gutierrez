#include <iostream>
#include <fstream>

using namespace std;

struct testa {
	int a;
	char b;
	short c;
};

ostream &operator << (ostream &output, const testa &t) {
	output << "a = " << t.a << endl;
	output << "b = " << t.b << endl;
	output << "c = " << t.c << endl;
	cout << "==============================" << endl;
	return output;
}

int main_test() {
	
	ofstream out;
	out.open("test.out", ios::binary | ios::out);
	testa ti = { 12, 'c', 10 };
	/*ti.a = 12;
	ti.b = 'c';
	ti.c = 10;
	*/
	
	out.write((char*)&ti, sizeof(ti));
	
	/*out.write((char*)&ti.a, sizeof(ti.a));
	out.write((char*)&ti.b, sizeof(ti.b));
	out.write((char*)&ti.c, sizeof(ti.c));*/

	out.close();

	ifstream in;
	testa t;
	in.open("test.out", ios::binary | ios::in);
	
	//in.read((char*)&t, sizeof(t));
	
	in.read((char*)&t.a, sizeof(t.a));
	in.read((char*)&t.b, sizeof(t.b));
	in.read((char*)&t.c, sizeof(t.c));

	in.close();

	cout << "el contenido de t es :" << endl;
	cout << t << endl;

	puts("se escribio exitosamente");
	return 0;
}