/*
 * Escriba un programa ISO C que reciba por linea de comando una UNICA cadena
 * de la forma "HHHH x HHHH" donde "x" es una operacion (+, -, *, /) y
 * "HHHH" es un numero hexadecimal de 4 simbolos.
 * El programa debe imprimir el resultado de la operacion aritmetica indicada en formato
 * decimal.
 * Por ejemplo: calc.exe "000A + 0002" => 12
 * 
 * No se pidio validar nada en particular
 */

#include <stdio.h>
#include <stdlib.h>

long int operate(char operator, long int op1, long int op2);

long int operate(char operator, long int op1, long int op2) {
	long int result;
	switch (operator) {
		case '+':
			result = op1 + op2;
			break;
		case '-':
			result = op1 - op2;
			break;
		case '*':
			result = op1 * op2;
			break;
		case '/':
			result = op1 / op2;
			break;
	}
	return result;
}

int main(int argc, char* argv[]) {
	char* arg_end;
	long int op1 = strtol(argv[1], &arg_end, 16);
	char operand = *(arg_end + 1);
	long int op2 = strtol(arg_end + 3, NULL, 16);
	long int result = operate(operand, op1, op2);
	printf("%lu\n", result);
	return 0;
}
