/*
 * Declare una función (con templates) Sumar<T> que reciba un arreglo de
 * elementos T y devuelva la sumatoria de esos elementos (otro elemento T).
 */
 
#include <stdlib.h>
 
template < typename T >
T Sumar(T array[], size_t len) {
	T result;
	for (size_t i = 0; i < len; ++i)
		result += array[i];
	return result;
}
