/*
 * Imagine la clase COMPLEJO con dos atributos (T Im; y T Re;), 
 * donde T puede ser float, int, etc. Defina el operator<< que escriba 
 * un objeto COMPLEJO<T> por STDOUT con el formato “(RE=rrrr – IM=iiii)”.
*/

#include <iostream>

template < typename T >
std::ostream& operator<<(std::ostream& o, const Complex<T>& complex) {
	T re = complex.re();
	T im = complex.im();
	o << "(RE=" << re << " - IM=" << im << ")" << std::endl;
	return o;
}
