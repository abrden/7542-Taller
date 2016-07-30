/*
 * Procesa un archivo (sobre si mismo) eliminando las secuencias DDCCCC
 * siendo DD dos numeros iguales y CCCC cuatro caracteres iguales.
 */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

#define FILE_NAME "input.txt"
#define SEQUENCE_SIZE 6
#define ACCOUNT_THE_END_OF_STR 1

size_t fsize(FILE* fp) {
	long pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, pos, SEEK_SET);
	return size;
}

bool rfeof(FILE* fp) { // aka real feof
	return ftell(fp) == fsize(fp);
}

bool is_sequence(char* buffer) {
	return (isdigit(buffer[0]) && buffer[0] == buffer[1] && 
	isalpha(buffer[2]) && buffer[2] == buffer[3] && buffer[2] == buffer[4]
	&& buffer[2] == buffer[5] && buffer[3] == buffer[4] && buffer[3] == buffer[5]
	&& buffer[4] == buffer[5]);
}

void main(void) {
	FILE* fp = fopen(FILE_NAME, "r+");
	
	char buffer[SEQUENCE_SIZE + 1];
	buffer[SEQUENCE_SIZE] = '\0';
	
	size_t write_pos, read_pos;
	write_pos = read_pos = ftell(fp);
	
	while (!rfeof(fp)) {
		fgets(buffer, SEQUENCE_SIZE + ACCOUNT_THE_END_OF_STR, fp);
		printf("Buffer: %s\n", buffer);
		
		if (is_sequence(buffer)) {
			// read_pos skips the sequence
			read_pos = ftell(fp);
		} else {
			read_pos++;
			
			printf("write_pos: %d\n", write_pos);
			printf("read_pos: %d\n", read_pos);
			
			fseek(fp, write_pos, SEEK_SET);
			printf("Writing '%c' in pos %d\n", buffer[0], ftell(fp));
			fputc(buffer[0], fp);
			fseek(fp, read_pos, SEEK_SET);
			
			write_pos++;
		}
	}
	
	fclose(fp);
	//truncate(FILE_NAME, read_pos - write_pos);
}
