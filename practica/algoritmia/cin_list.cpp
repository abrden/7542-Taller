/* 
 * Defina el operador>> de forma que cargue, desde stdin, una lista STL 
 * de strings. La carga debe terminar cuando se ingrese una cadena nula.
 */

std::istream& operator>>(std::istream& i, std::list<std::string>& list) {
	std::stringstream ss;
	i >> ss;
	while (s.length() > 0) {
		list.push_back(ss.str());
		ss.str("");
		i >> ss;
	}
	return i;
}
