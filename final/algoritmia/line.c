/*
 * Escriba un programa ANSI C que tome por línea de comandos 3 parámetros: 
 * A, B e Yi, e imprima por stdout el valor de x que cumple Yi = Ax + B. 
 * Valide todos los errores posibles originados en los valores ingresados.
 */

#include <stdlib.h>
#include <stdio.h>

#define PARAM_LEN 4

int main(int argc, char* argv[]) {
	if (argc != PARAM_LEN) return 1;
	
	long int a = strtol(argv[1], NULL, 10);
	long int b = strtol(argv[2], NULL, 10);
	long int y = strtol(argv[3], NULL, 10);
	
	if (!a || !b || !y) return 1;
	
	double x = (y - b) / (double) a;
	
	printf("%d = %dx + %d, with x = %f\n", y, a, b, x);
	
	return 0;
}
