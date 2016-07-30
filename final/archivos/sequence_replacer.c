/* 
 * Escriba un programa ANSI C que procese el archivo datos.bin. 
 * El procesamiento consiste en reemplazar las secuencias de bytes 
 * 0x2F, 0x31, 0x3A por 0x31, 0x3A.
 * Adicionalmente, las búsquedas deben reiniciarse 3 bytes antes del
 *  último byte reemplazado. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define FILE_NAME "datos.bin"
#define BUFFER_LEN 3

void go_back(size_t final_pos, FILE* fp);
void process(size_t read_pos, size_t write_pos, size_t final_pos, FILE* fp);

bool is_sequence(char buffer[]) {
	char sequence[] = { 0x2F, 0x31, 0x3A };
	for (int i = 0; i < BUFFER_LEN; i++) 
		if (sequence[i] != buffer[i])
			return false;
	return true;
}

void replace_sequence(size_t write_pos, FILE* fp) {
	char replacement[] = { 0x31, 0x3A, 0x40 };
	size_t current_pos = ftell(fp);
	fseek(fp, write_pos, SEEK_SET);
	fwrite(replacement, sizeof(char), BUFFER_LEN, fp);
	fseek(fp, current_pos, SEEK_SET);
}

int process(size_t read_pos, size_t write_pos, size_t final_pos, FILE* fp) {
	char buffer[BUFFER_LEN + 1];
	buffer[BUFFER_LEN] = '\0';
	
	int go_back_counter = 0;
	
	while (read_pos < final_pos) {
		printf("Reading from %lu\n", read_pos);
		fread(buffer, sizeof(char), BUFFER_LEN, fp);
		printf("Buffer '%s'\n", buffer);
		read_pos++;
		
		if (is_sequence(buffer)) {
			replace_sequence(write_pos, fp);
			go_back_counter += go_back(read_pos, fp);
			write_pos += 2; //- go_back_counter + 1;  
			// read_pos skips the sequence
			read_pos += 2;
			fseek(fp, read_pos, SEEK_SET);
			
		} else {
			// rewrite first read byte
			fseek(fp, write_pos, SEEK_SET);
			printf("Writing '%c' in pos %lu\n", buffer[0], write_pos);
			fwrite(&buffer[0], sizeof(char), 1, fp);
			fseek(fp, read_pos, SEEK_SET);
			write_pos++;
		}
	}
	return go_back_counter;
}

int go_back(size_t final_pos, FILE* fp) {
	printf("\nRewind\n");
	fseek(fp, -4, SEEK_CUR);
	return process(ftell(fp), ftell(fp), final_pos, fp) + 1;
}

int main(void) {
	FILE* fp = fopen(FILE_NAME, "rb+");
	
	size_t read_pos, write_pos, final_pos;
	fseek(fp, 0, SEEK_END);
	final_pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	read_pos = write_pos = ftell(fp);
	
	process(read_pos, write_pos, final_pos, fp);
	
	//ftruncate(fileno(fp), write_pos);
	fclose(fp);
	
	return 0;
}
