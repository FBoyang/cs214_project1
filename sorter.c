#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "mergesort.h"

int get_fields(char *fieldlist, char ***fields_ptr, int *num_fields_ptr);
int header_index(char *str);
char *ofname(char *infile, char *fieldlist);
char *ifname(char *inpath, char *infile);
void free_matrix(char ***matrix, int num_rows, int num_cols);
void free_header(char **header, int num_cols);

int sort_csv(char *infile, char *fieldlist, char *in_path, char *out_path)
{
	char ***table;
	char **header;
	char *fieldcopy;
	char **fields;
	int num_rows, num_cols, num_fields;
	char *outfile;
	char *infilepath;
	int i;
	int *field_indices;
	if (strlen(infile) < 4 || strcmp(infile + strlen(infile) - 4, ".csv") ||
		/* wrong file extension */
			strstr(infile, "-sorted-")) {
		/* already sorted */
		return 1;
	}
	fieldcopy = malloc((strlen(fieldlist) + 1) * sizeof(char));
	strcpy(fieldcopy, fieldlist);
	get_fields(fieldcopy, &fields, &num_fields);
	field_indices = malloc(num_fields * sizeof(*field_indices));
	infilepath = ifname(in_path, infile);
	if (read_csv(&table, &header, infilepath, fields, field_indices, &num_rows, &num_cols, num_fields)) {
		free(fieldcopy);
		free(fields);
		free(field_indices);
		free(infilepath);
		return 1;
	}
	for (i = num_fields - 1; i >= 0; i--)
		sort_by_field(table, num_rows, num_cols, field_indices[i]);
	outfile = ofname(infile, fieldlist);
	if (out_path)
		chdir(out_path);
	print_table(table, header, num_rows, num_cols, outfile);
	free_matrix(table, num_rows, num_cols);
	free_header(header, num_cols);
	free(fields);
	free(fieldcopy);
	free(infilepath);
	free(outfile);
	return 0;
}

int get_fields(char *fieldlist, char ***fields_ptr, int *num_fields_ptr)
{
	char **fields;
	int num_fields;
	char *ptr;
	int field_capacity;
	num_fields = 0;
	field_capacity = 4;
	fields = malloc(field_capacity * sizeof(**fields));
	for (ptr = strtok(fieldlist, ","); ptr; ptr = strtok(NULL, ",")) {
		if (num_fields == field_capacity) {
			field_capacity *= 2;
			fields = realloc(*fields, field_capacity * sizeof(*fields));
		}
		fields[num_fields++] = ptr;
	}
	*fields_ptr = fields;
	*num_fields_ptr = num_fields;
	return 0;
}

char *ofname(char *infile, char *fieldlist)
{
	int baselen;
	char *outfile;
	baselen = strlen(infile) - 4;
	outfile = malloc((strlen(infile) + strlen(fieldlist) + 9) * sizeof(char));
	strncpy(outfile, infile, baselen);
	outfile[baselen] = '\0';
	strcat(outfile, "-sorted-");
	strcat(outfile, fieldlist);
	strcat(outfile, ".csv");
	return outfile;
}

char *ifname(char *inpath, char *infile)
{
	char *infilepath;
	infilepath = malloc((strlen(inpath) + strlen(infile) + 2) * sizeof(char));
	strcpy(infilepath, inpath);
	strcat(infilepath, "/");
	strcat(infilepath, infile);
	return infilepath;
}

void free_matrix(char ***matrix, int num_rows, int num_cols)
{
	int i, j;
	for (i = 0; i < num_rows; i++) {
		for (j = 0; j < num_cols; j++)
			free(matrix[i][j]);
		free(matrix[i]);
	}
	free(matrix);
}

void free_header(char **header, int num_cols)
{
	int i;
	for (i = 0; i < num_cols; i++)
		free(header[i]);
	free(header);
}
