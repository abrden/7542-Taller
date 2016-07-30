/*
 * Escriba una aplicación ANSI C++ que tome dos strings por línea de 
 * comandos e imprima un tercer string resultado de intercalar las letras
 * de ambos string de acuerdo a su precedencia alfabética.
 */
 
#include <string>
#include <iostream>

std::string interleaved(std::string a, std::string b) {
	std::string r;
	for (unsigned int i = 0; i < a.size(); ++i) {
		r.append(a.substr(i, 1));
		r.append(b.substr(i, 1));
	}
	return r;
}

int main(int argc, char* argv[]) {
	std::string a(argv[1]);
	std::string b(argv[2]);
	std::string result;
	if (a < b) {
		result = interleaved(a, b);
	} else {
		result = interleaved(b, a);
	}
	std::cout << result << std::endl;
	return 0;
}
