/*
 * Escriba una función ANSI C++ que reciba una lista de elementos T 
 * (std::list) y devuelva otra lista de elementos T sin la existencia de repeticiones.
 */

#include <list>
#include <set>

std::list<T> bye_repeats(std::list<T>& with_repeats) {
	std::set<T> no_repeats(with_repeats.begin(), with_repeats.end());
	std::list<T> result(no_repeats.begin(), no_repeats.end());
	return list;
}
