#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mergesort.h"

int check_header(char *line, char ***header_ptr, char **fields, int *field_indices, int *num_cols, int num_fields);

int read_csv(char ****table_ptr, char ***header_ptr, char *ifname, char **fields, int *field_indices, int *num_rows, int *num_cols_ptr, int num_fields)
{
	char ***table;
	char **header;
	FILE *file;
	int num_cols;
	int row;
	int col;
	int row_capacity;
	char *line;
	size_t n;
	int i;
	int str_len, pstr_len, p_diff;
	char *tokens, *prev_tokens;
	char special_tokens[200];
	if ((file = fopen(ifname, "r")) == NULL) {
		fprintf(stderr, "failed to open file \"%s\"\n", ifname);
		return 1;
	}
	line = NULL;
	n = 0;
	getline(&line, &n, file);
	if (check_header(line, &header, fields, field_indices, &num_cols, num_fields)) {
		/* we don't have to deal with this file */
		fclose(file);
		free(line);
		return 1;
	}
	row_capacity = 1024;
	table = malloc(row_capacity * sizeof(*table));
	/* this part serves to read the record and store it in the table */
	row = 0;
	col = 0;
	while(getline(&line, &n, file) != -1) {
		str_len = strlen(line) - 2;/* exclude '\n' */
		table[row] = malloc(num_cols * sizeof(**table));
		pstr_len = 0;
		col = 0;
		tokens = strtok(line, ",\r\n");
		prev_tokens = line;
		while(tokens) {
			if(prev_tokens[0] == ',')
				pstr_len = -1;
			p_diff = tokens - (prev_tokens + pstr_len);
			if(p_diff <= 1) {
				//treat the data as string
				if(tokens[0] != '"') {
					prev_tokens = tokens;
					pstr_len = strlen(prev_tokens);
					table[row][col] = malloc(strlen(tokens)+2);
					strcpy(table[row][col++], tokens);
				} else {
					//embeded comma
					prev_tokens = tokens;
					strcpy(special_tokens, tokens);
					strcat(special_tokens, ",");
					// make up the lost comma
					tokens = strtok(NULL, "\"");
					strcat(special_tokens, tokens);
					pstr_len = strlen(special_tokens) + 2; // one comma and one quotation mark miss
					//need to be optimized
					table[row][col] = malloc(strlen(special_tokens)+2);
					strcpy(table[row][col++], special_tokens+1);
				}
				tokens = strtok(NULL, ",\r\n");
			} else {
				for(i =1; i < p_diff; i++)
					table[row][col++] = NULL;
				prev_tokens = tokens;
				pstr_len = 0; //since prev_tokens and tokens point to the same position
			}
		}
		/* check whether the last few cell are empty */
		p_diff = line + str_len  - (strlen(prev_tokens) + prev_tokens);
		for (i = 0; i < p_diff; i++) {
			table[row][col] = NULL;
		}
		col = 0;	
		row++;
		if(row >= row_capacity -2) {
			row_capacity *= 2;
			table = realloc(table, row_capacity * sizeof(*table));
		}
	}
	free(line);
	fclose(file);
	*num_rows = row;
	*num_cols_ptr = num_cols;
	*table_ptr = table;
	*header_ptr = header;
	return 0;
}

int check_header(char *line, char ***header_ptr, char **fields, int *field_indices, int *num_cols, int num_fields)
{
	/*
	 * counts the number of columns, then
	 * returns 0 if and only if
	 * all fields we want are in the header
	 */
	int i, j, ret;
	char *ptr;
	char **header;
	int header_capacity;
	for (j = 0; j < num_fields; j++)
		field_indices[j] = -1;
	i = 0;
	header_capacity = 4;
	header = malloc(header_capacity * sizeof(*header));
	for (ptr = strtok(line, ",\r\n"); ptr; ptr = strtok(NULL, ",\r\n")) {
		if (i >= header_capacity) {
			header_capacity *= 2;
			header = realloc(header, header_capacity * sizeof(*header));
		}
		header[i] = malloc((strlen(ptr) + 1) * sizeof(**header));
		strcpy(header[i], ptr);
		for (j = 0; j < num_fields; j++) {
			if (field_indices[j] == -1 && strcmp(fields[j], ptr) == 0)
				field_indices[j] = i;
		}
		i++;
	}
	ret = 0;
	for (j = 0; j < num_fields; j++) {
		if (field_indices[j] == -1)
			ret = 1;
	}
	*header_ptr = header;
	*num_cols = i;
	if (ret) {
		for (i = 0; i < *num_cols; i++)
			free(header[i]);
		free(header);
	}
	return ret;
}
