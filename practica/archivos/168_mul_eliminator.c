/* 
 * Escriba una aplicación ANSI C que modifique el archivo a.bin 
 * eliminando los numeros múltiplos de 168.
 */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define FILE_NAME "input.txt"
#define BUFFER_MAX 10

enum states {
	READING,
	READING_NUM
};

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

bool is_168_multiple(long int n) {
	return (n % 168 == 0);
}

void write_alpha(FILE* fp, size_t write_pos, size_t read_pos, char char_buff) {
	printf("write_pos: %lu\n", write_pos);
	printf("read_pos: %lu\n", read_pos);
			
	fseek(fp, write_pos, SEEK_SET);
	printf("Writing '%c' in pos %lu\n", char_buff, ftell(fp));
	fputc(char_buff, fp);
	fseek(fp, read_pos, SEEK_SET);
}

void write_buffer(FILE* fp, size_t write_pos, size_t read_pos, char* buffer) {
	printf("write_pos: %lu\n", write_pos);
	printf("read_pos: %lu\n", read_pos);
			
	fseek(fp, write_pos, SEEK_SET);
	printf("Writing '%s' in pos %lu\n", buffer, ftell(fp));
	fputs(buffer, fp);
	fseek(fp, read_pos, SEEK_SET);
}

void main(void) {
	FILE* fp = fopen(FILE_NAME, "r+");
	
	char buffer[BUFFER_MAX + 1];
	buffer[BUFFER_MAX] = '\0';
	size_t i = 0;
	
	size_t write_pos, read_pos;
	write_pos = read_pos = ftell(fp);
	
	char char_buff;
	int state = READING;
	
	while (!rfeof(fp)) {
		char_buff = fgetc(fp);
		printf("char_buff: %c\n", char_buff);
		read_pos = ftell(fp);
		
		switch (state) {
			case READING:
				if (isdigit(char_buff)) {
					state = READING_NUM;
					buffer[i] = char_buff;
					i++;
				} else {
					write_alpha(fp, write_pos, read_pos, char_buff);
					write_pos++;
				}
				break;
			case READING_NUM:
				if (isdigit(char_buff)) {
					buffer[i] = char_buff;
					i++;
				} else {
					state = READING;
					buffer[i] = '\0';
					if (!is_168_multiple(strtol(buffer, NULL, 10))) {
						write_buffer(fp, write_pos, read_pos, buffer);
						write_pos += i;	
					}
					write_alpha(fp, write_pos, read_pos, char_buff);
					write_pos++;
					i = 0;
				}
				break;
		}	
	}
	
	fclose(fp);
	//truncate(FILE_NAME, read_pos - write_pos);
}
