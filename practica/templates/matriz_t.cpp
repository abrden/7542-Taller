/*
 * Haciendo uso de STL declare una clase MATRIZ, de forma que las siguientes operaciones sean válidas:
 * 
 * MiMatriz[1][2] = <valor>
 * X = MiMatriz[1][2]
 */

#include <stdlib.h>
#include <vector>

template < typename T >
class Matrix {
	private:
		std::vector<std::vector<T>> _matrix;
	public:
		Matrix(size_t col_capacity, size_t row_capacity);
		~Matrix();
		std::vector<T>& operator[](size_t index);
};

template < typename T >
Matrix<T>::Matrix(size_t col_capacity, size_t row_capacity) : 
	_matrix(col_capacity, vector<T>(row_capacity)) {}

template < typename T >
Matrix<T>::~Matrix() {}

template < typename T >
std::vector<T>& Matrix<T>::operator[](size_t index) {
	return _matrix[index];
}
