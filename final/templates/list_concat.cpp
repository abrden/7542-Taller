/*
 * Defina el operador global +, para concatenar 2 listas STL de tipos genérico T.
 */

template < typename T >
std::list<T> operator+(const std::list<T>& left, const std::list<T>& right) {
	std::list<T> result;
	for (std::list<T>::iterator it = left.begin(); it != left.end(); ++it)
		result.push_back(*it);
	for (std::list<T>::iterator it = right.begin(); it != right.end(); ++it)
		result.push_back(*it);
	return result;
}
