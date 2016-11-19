#include <iostream>
#include <fstream>
#include <sstream>
#define db(a) cout << #a << " = " << a << endl;
using namespace std;

int main_old() {
	ifstream in;
	
	in.open("toread.out", ios::in | ios::binary);
	int a, b;
	in.read((char*)&a, sizeof(int));
	in.read((char*)&b, sizeof(int));

	in.close();
	db(a);
	db(b);
	return 0;
}