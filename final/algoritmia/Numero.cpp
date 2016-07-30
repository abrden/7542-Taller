#include <iostream>

#include "Numero.h"

Numero::Numero() : _numero(100) {}

std::ostream& operator<<(std::ostream& o, const Numero& n) {
	o << n._numero;
	return o; 
}

int main(void) {
	Numero n;
	std::cout << n;
	return 0;
}
