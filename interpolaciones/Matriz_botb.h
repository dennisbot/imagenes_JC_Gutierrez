#include <string>
class Matriz_botb {
public:
	unsigned char *ptr;
	int rows;
	int cols;
	// constructores
	Matriz_botb(void);
	Matriz_botb(int, int);

	// destructor
	~Matriz_botb();
	friend std::ostream& operator<<(std::ostream &, const Matriz_botb &);
	friend std::ostream& operator<<(std::ostream &, const Matriz_botb *);
};
