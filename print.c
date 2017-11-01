#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mergesort.h"
int escaped(char *str);
int print_table(char ***table, char **header, int num_rows, int num_cols, char *ofname)
{
	int i, j;
	char *str;
	FILE *outfile;
	if ((outfile = fopen(ofname, "w")) == NULL) {
		fprintf(stderr, "failed to open file \"%s\"\n", ofname);
		return 1;
	}
	/* print the sorted csv file */
	for (i = 0; i < num_cols; i++) {
		fputs(header[i], outfile);
		if (i == num_cols - 1)
			break;
		fputc(',', outfile);
	}
	fputs("\r\n", outfile);
	for (i = 0; i < num_rows; i++) {
		for (j = 0; j < num_cols; j++) {
			str = table[i][j];
			if (str) {
				if (escaped(str))
					fprintf(outfile, "\"%s\"", str);
				else
					fputs(str, outfile);
			}
			if (j == num_cols - 1)
				break;
			fputc(',', outfile);
		}
		fputs("\r\n", outfile);
	}
	return 0;
}

int escaped(char *str)
{
	return index(str, ',') || index(str, '"') || index(str, '\r') || index(str, '\n');
}
