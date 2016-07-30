/*
 * Definir la rutina ORDEN de forma que ordene los elementos recibidos 
 * de la siguiente manera: Primero los impares y luego los pares, y 
 * dentro de esa clasificación, por orden descendente. 
 * Ejemplo: Si recibe los elementos 1,5,7,2,9,4,3 debe devolver 9,7,5,3,1,4,2
 * 
 * std:list<T> ORDEN(std:list<T> &Lista);
 * Nota: Asuma que el tipo T posee el operador < y el método EsImpar.
 */

#include <list>

std:list<T> ORDEN(std:list<T>& Lista) {
	std::list<T> pares;
	std::list<T> impares;
	T aux;
	for (int i = 0; i < Lista.size(); i++) {
		aux = Lista.back();
		if (aux.EsImpar()) impar.push_back(aux);
		else par.push_back(aux);
		Lista.pop_back();
	}
	impar.sort();
	par.sort();
	for (std::list<T>::iterator it = impar.rbegin(); it != impar.rend(); ++it)
		Lista.push_back(*it);
	for (std::list<T>::iterator it = par.rbegin(); it != par.rend(); ++it)
		Lista.push_back(*it);
	return Lista;
}
