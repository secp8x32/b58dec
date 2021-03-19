#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "base58/libbase58.h"


char* ltrim(char* str, const char* seps)
{
	size_t totrim;
	if (seps == NULL) {
		seps = "\t\n\v\f\r ";
	}
	totrim = strspn(str, seps);
	if (totrim > 0) {
		size_t len = strlen(str);
		if (totrim == len) {
			str[0] = '\0';
		}
		else {
			memmove(str, str + totrim, len + 1 - totrim);
		}
	}
	return str;
}

char* rtrim(char* str, const char* seps)
{
	int i;
	if (seps == NULL) {
		seps = "\t\n\v\f\r ";
	}
	i = strlen(str) - 1;
	while (i >= 0 && strchr(seps, str[i]) != NULL) {
		str[i] = '\0';
		i--;
	}
	return str;
}

char* trim(char* str, const char* seps)
{
	return ltrim(rtrim(str, seps), seps);
}


void convert(const char* in_filename, const char* out_filename)
{
	FILE* fd1 = NULL;
	FILE* fdo = NULL;
	char* aux = NULL;
	char* hextemp = NULL;

	fd1 = fopen(in_filename, "rb");
	fdo = fopen(out_filename, "wb");

	if (fd1 == NULL || fdo == NULL)
		exit(EXIT_FAILURE);

	aux = malloc(1000);
	size_t len2 = 25;
	uint8_t addrbin[25];
	uint64_t failed = 0;
	uint64_t count = 0;
	while (!feof(fd1)) {
		memset(aux, '\0', 1000);
		hextemp = fgets(aux, 1000, fd1);
		if (hextemp == aux && aux[0] != '\0') {
			trim(aux, " \t\n\r");
			int len = strlen(aux);
			len2 = 25;
			memset(addrbin, 0, 25);
			if (b58tobin(addrbin, &len2, aux, len)) {
				fwrite(addrbin + 1, 1, 20, fdo);
				count++;
			}
			else {
				failed++;
			}
		}
	}
	printf("completed: %llu\n", count);
	printf("failed: %llu\n", failed);
	fclose(fd1);
	fclose(fdo);
	free(aux);
}


int main(int argc, const char* argv[])
{
	if (argc != 3) {
		printf("Error: wrong args\n");
		printf("Usage: %s in_file out_file\n", argv[0]);
		exit(1);
	}
	printf("\n");
	convert(argv[1], argv[2]);

	exit(EXIT_SUCCESS);
}
