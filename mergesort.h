#ifndef _MERGESORT_H
#define _MERGESORT_H

int read_csv(char ****table, char ***header, char *ifname, char **field_strings, int *field_indices, int *num_rows, int *num_cols, int num_fields);
int sort_by_field(char ***table, int num_rows, int num_cols, int field_index);
int print_table(char ***table, char **header, int num_rows, int num_cols, char *ofname);
#endif
