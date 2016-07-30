#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_NAME "input.txt"

int main(int argc, char** argv) {
	FILE *fp = fopen(FILE_NAME, "rb+");
	
	fseek(fp, 0, SEEK_END);
	long final_pos = ftell(fp);
	
	fseek(fp, 0, SEEK_SET);
	long int reader_pos, writer_pos;
	reader_pos = writer_pos = ftell(fp);

	while (reader_pos < final_pos) {

		if (reader_pos % 168 != 0) {
			fseek(fp, reader_pos, SEEK_SET);
			char c = fgetc(fp);
			fseek(fp, writer_pos, SEEK_SET);
			fputc(c, fp);
			writer_pos++;
		}

		reader_pos++;
	}

	ftruncate(fileno(fp), writer_pos);
	fclose(fp);
	
	return 0;
}
