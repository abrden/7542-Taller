/*
 * Escribir un programa ISO C que procese el archivo palabras.txt sobre 
 * sí mismo. 
 * El proceso consiste en duplicar las palabras que tengan más de 2 
 * consonantes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_NAME "palabras.txt"
#define WORD_BUFF_LEN 20
#define PLACEHOLDER 'X'

void read_word(FILE* fp, char* buffer) {
	char char_buff = fgetc(fp);
	int i;
	for (i = 0; !feof(fp) && char_buff != ' '; i++) {
		buffer[i] = char_buff;
		char_buff = fgetc(fp);
	}
	buffer[i] = '\0';
}

bool double_consonant_word(char* buffer) {
	int consonants = 0;
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] != 'a' && buffer[i] != 'e' && buffer[i] != 'i' 
		&& buffer[i] != 'o' && buffer[i] != 'u' && buffer[i] != 'A' 
		&& buffer[i] != 'E' && buffer[i] != 'I' && buffer[i] != 'O' 
		&& buffer[i] != 'U') consonants++;
	}
	return (consonants > 2);
}

size_t calculate_expansion(FILE* fp) {
	size_t expansion = 0;
	while (!feof(fp)) {
		char buffer[WORD_BUFF_LEN + 1];
		buffer[WORD_BUFF_LEN] = '\0';
		read_word(fp, buffer);
		printf("word: %s\n", buffer);
		if (double_consonant_word(buffer))
			expansion += strnlen(buffer, WORD_BUFF_LEN) + 1;
	}
	return expansion;
}

void expand_file(size_t expansion, FILE* fp) {
	for (int i = 0; i < expansion; i++) {
		printf("writing ph - %d\n", fputc(PLACEHOLDER, fp));
	}
}

void reversed_read_word(FILE* fp, char* buffer) {
	printf("reading reversed word\n");
	char char_buff = fgetc(fp);
	fseek(fp, -2, SEEK_CUR);
	int i;
	for (i = 0; ftell(fp) != 0 && char_buff != ' '; i++) {
		buffer[i] = char_buff;
		printf("char %c in pos %d\n", buffer[i], ftell(fp) + 1);
		char_buff = fgetc(fp);
		fseek(fp, -2, SEEK_CUR);
		
	}
	
	if (ftell(fp) == 0) {
		fseek(fp, 1, SEEK_SET);
		char_buff = fgetc(fp);
		fseek(fp, -2, SEEK_CUR);
		buffer[i] = char_buff;
		char_buff = fgetc(fp);
		i++;
		buffer[i] = char_buff;
		fseek(fp, 0, SEEK_SET);
		i++;
	}
	
	buffer[i] = '\0';
}

void reversed_write_word(FILE* fp, char* buffer) {
	for (int i = 0; buffer[i] != '\0'; i++) {
		fputc(buffer[i], fp);
		fseek(fp, -2, SEEK_CUR);
	}
	fputc(' ', fp);
	fseek(fp, -2, SEEK_CUR);
}

void duplicate_double_consonant_words(FILE* read_fp, FILE* write_fp) {
	while (ftell(read_fp) != 0) {
		char buffer[WORD_BUFF_LEN + 1];
		buffer[WORD_BUFF_LEN] = '\0';
		reversed_read_word(read_fp, buffer);
		printf("word: %s\n", buffer);
		reversed_write_word(write_fp, buffer);
		if (double_consonant_word(buffer))
			reversed_write_word(write_fp, buffer);
	}
}

int main(void) {
	FILE* read_ptr = fopen(FILE_NAME, "r");
	FILE* write_ptr = fopen(FILE_NAME, "r + t");
	fseek(write_ptr, 0, SEEK_END);
	
	size_t expansion = calculate_expansion(read_ptr);
	printf("expansion: %d\n", expansion);
	expand_file(expansion, write_ptr);
	fseek(read_ptr, -1, SEEK_CUR);
	fseek(write_ptr, -1, SEEK_CUR);
	
	printf("read_ptr: %d\n", ftell(read_ptr));
	printf("write_ptr: %d\n", ftell(write_ptr));
	
	duplicate_double_consonant_words(read_ptr, write_ptr);
	
	return 0;
}
