/* 
 * Escribir un programa en C que, sin crear archivos intermedios, procese
 * el archivo binario "mayores.dat". El procesamiento consiste en leer
 * grupo de 3 enteros sin signo de 32 bits desde el archivo y escribir,
 * en su lugar, el mayor de los tres.
 */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define FILE_NAME "mayores.dat"
#define BUFFER_LEN 3

unsigned int best_of_3(unsigned int buffer[]) {
	unsigned int best = buffer[0];
	
	for (int i = 0; i < BUFFER_LEN; i++)
		if (buffer[i] > best) best = buffer[i];
			
	return best;
}

int main(void) {
	FILE *fp = fopen(FILE_NAME, "rb+");
	
	unsigned int buffer[BUFFER_LEN];
	unsigned int best;
	
	size_t reader_pos, writer_pos, final_pos;
	reader_pos = writer_pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	final_pos = ftell(fp);
	fseek(fp, reader_pos, SEEK_SET);
	
	while (reader_pos < final_pos) {
		fread(buffer, sizeof(unsigned int), BUFFER_LEN, fp);
		reader_pos = ftell(fp);
		
		best = best_of_3(buffer);
		
		fseek(fp, writer_pos, SEEK_SET);
		fwrite(&best, sizeof(unsigned int), 1, fp);
		writer_pos = ftell(fp);
		
		fseek(fp, reader_pos, SEEK_SET);
	}
	
	ftruncate(fileno(fp), writer_pos);
	fclose(fp);
	
	return 0;
}

