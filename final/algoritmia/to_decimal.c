/*
 * Recibe un numero y una base (entre 2 y 16), imprime el numero en la 
 * base recibida. Contempla errores.
 */

#include <stdio.h>
#include <stdlib.h>

#define VALID_ARGC 3

int main(int argc, char* argv[]) {
	if (argc != VALID_ARGC) return 1;
	
	long int base = strtol(argv[2], NULL, 10);
	if (base < 2 || base > 16) return 2;
	
	long int number = strtol(argv[1], NULL, base);
	printf("%lu", number);
	
	return 0;
}
