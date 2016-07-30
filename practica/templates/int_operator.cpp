/*
 * Defina el operador int que trabaje con listas STL de tipos genéricos T. 
 * El operador debe devolver la cantidad de elementos distintos que posee la lista.
 */

#include <list>
#include <set>

std::list<T>::operator int() const {
	std::set<T> no_repeats(this->begin(), this->end());
	return (int) no_repeats.size();
}
