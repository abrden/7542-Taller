/* 
 * Escribir un algoritmo ANSI C que, sin crear archivos intermedios
 *  altere el archivo a.bin reemplazando la secuencia '-' por '--'
 */

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "a.bin"
#define BUFFER_LEN 2

int main(void) {
	FILE* fp = fopen(FILE_NAME, "rb+");
	char buffer;
	size_t read_pos, write_pos, final_pos;
	
	fseek(fp, 0, SEEK_END);
	final_pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	size_t extension = 0;
	while (ftell(fp) < final_pos)
		if (fgetc(fp) == '-') extension++;
	
	for (int i = 0; i < extension; i++) {
		printf("Writing 'X' in pos %lu\n", ftell(fp));
		fputc('X', fp);
	}
	
	read_pos = final_pos;
	write_pos = ftell(fp);
	
	fseek(fp, read_pos, SEEK_SET);
	
	while (read_pos > 0) {
		printf("Reading from %lu\n", read_pos);
		buffer = fgetc(fp);
		printf("Buffer '%c'\n", buffer);
		read_pos--;
		
		fseek(fp, write_pos, SEEK_SET);
		fputc(buffer, fp);
		printf("Writing '%c' in pos %lu\n", buffer, write_pos);
		write_pos--;
		
		if (buffer == '-') {
			fseek(fp, write_pos, SEEK_SET);
			fputc(buffer, fp);
			printf("Writing '%c' in pos %lu\n", buffer, write_pos);
			write_pos--;
		}
		
		fseek(fp, read_pos, SEEK_SET);
	}
	
	fclose(fp);
	
	return 0;
}
