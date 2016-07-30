/*
 * Utilizando templates defina las clases necesarias para 
 * que el siguiente código sea válido:
 * 
 * Arreglo<int> A(5000);
 * A[0]=123;
 * A[1]=A[0];
 */

#include <stdlib.h>

template < typename T >
class Array {
	private:
		T* _array;
		size_t capacity;
	public:
		Array(size_t capacity);
		~Array();
		T& operator[](size_t index);
};

template < typename T >
Array<T>::Array(size_t capacity) : capacity(capacity) {
	_array = (T*) malloc(capacity * sizeof(T));
}

template < typename T >
Array<T>::~Array() {
	free(_array);
}

template < typename T >
T& Array<T>::operator[](size_t index) {
	return _array[index];
}
