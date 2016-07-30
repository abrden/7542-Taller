/* 
 * Escribir un algoritmo ANSI C que, sin crear archivos intermedios
 * altere el archivo a.txt reemplazando la secuencia '//' por '*'
 * Excepto si se encuentra entre parentesis.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define FILE_NAME "a.txt"
#define BUFFER_LEN 2
#define SEQUENCE "//"
#define REPLACEMENT '*'

enum states { REGULAR, PARENTHESIS };

bool is_sequence(char buffer[]) {
	char sequence[] = SEQUENCE;
	for (int i = 0; i < BUFFER_LEN; i++) 
		if (sequence[i] != buffer[i])
			return false;
	return true;
}

void write_byte(char byte, size_t read_pos, size_t write_pos, FILE* fp) {
	fseek(fp, write_pos, SEEK_SET);
	printf("Writing '%c' in pos %lu\n", byte, write_pos);
	fwrite(&byte, sizeof(char), 1, fp);
	fseek(fp, read_pos, SEEK_SET);
}

void process(size_t read_pos, size_t write_pos, size_t final_pos, FILE* fp) {
	char buffer[BUFFER_LEN + 1];
	buffer[BUFFER_LEN] = '\0';
	
	int state = REGULAR;
	
	while (read_pos < final_pos) {
		printf("Reading from %lu\n", read_pos);
		fread(buffer, sizeof(char), BUFFER_LEN, fp);
		printf("Buffer '%s'\n", buffer);
		read_pos++;
		
		if (buffer[0] == '(') state = PARENTHESIS;
		else if (buffer[0] == ')') state = REGULAR;
		
		if (is_sequence(buffer) && state == REGULAR) {
			// skip the sequence
			read_pos = ftell(fp);
			write_byte(REPLACEMENT, read_pos, write_pos, fp);
			write_pos++;
			
		} else {
			// rewrite first read byte
			write_byte(buffer[0], read_pos, write_pos, fp);
			write_pos++;
		}
	}
	
	truncate(FILE_NAME, sizeof(char) * write_pos);
}

int main(void) {
	FILE* fp = fopen(FILE_NAME, "rb+");
	
	size_t read_pos, write_pos, final_pos;
	fseek(fp, 0, SEEK_END);
	final_pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	read_pos = write_pos = ftell(fp);
	
	process(read_pos, write_pos, final_pos, fp);
	
	fclose(fp);
	
	return 0;
}
