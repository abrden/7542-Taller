/* 
 * Escriba una aplicación ANSI C++ que tome dos strings por línea de 
 * comandos e imprima el primero sin la segunda ocurrencia (si existe) 
 * del segundo.
 */
 
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	std::string string(argv[1]);
	std::string substring(argv[2]);
	
	std::size_t found = string.find(substring);
	
	if (found != std::string::npos) {
		found = string.find(substring, found + 1);
		
		if (found != std::string::npos) {
			string.erase(found, substring.length());
			std::cout << "Result: " << string << std::endl;
		} else {
			std::cout << "Second substring not found in string" << std::endl;
		}
		
	} else 
		std::cout << "Substring not found in string" << std::endl;
	
	return 0;
}
