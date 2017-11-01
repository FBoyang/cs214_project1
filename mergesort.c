#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include "mergesort.h"
int compare(char *a, char *b);
int lexcmp(char *a, int alen, char *b, int blen);
int charcmp(char a, char b);
int strbegin(char *str);
int strend(char *str);
int min(int a, int b);
int sort_by_field(char ***table, int num_rows, int num_cols, int field_index)
{
	int *end;
	int low, middle, high;
	int i, j;
	char ***a, ***b, ***tmp;
	int ind;
	/* performs a bottom-up mergesort on the table */
	/* find already sorted regions */
	end = malloc(num_rows * sizeof(*end));
	j = 0;
	for (i = 1; i < num_rows; i++) {
		if (compare(table[i - 1][field_index], table[i][field_index]) > 0) {
			end[j] = i;
			j = i;
		}
	}
	end[j] = num_rows;
	/* begin actual mergesort */
	a = table;
	b = malloc(num_rows * sizeof(*b));
	ind = 0;
	while (end[0] != num_rows) {
		low = ind;
		middle = end[ind];
		high = end[middle];
		i = low;
		j = middle;
		while (i < middle && j < high) {
			if (compare(a[i][field_index], a[j][field_index]) <= 0) {
				b[ind] = a[i];
				ind++;
				i++;
			} else {
				b[ind] = a[j];
				ind++;
				j++;
			}
		}
		while (i < middle) {
			b[ind] = a[i];
			ind++;
			i++;
		}
		while (j < high) {
			b[ind] = a[j];
			ind++;
			j++;
		}
		end[low] = high;
		if (high == num_rows || end[high] == num_rows) {
			/* reset ind to 0, then swap a and b */
			for (; ind < num_rows; ind++)
				b[ind] = a[ind];
			ind = 0;
			tmp = a;
			a = b;
			b = tmp;
		}
	}
	table = a;
	free(end);
	free(b);
	return 0;
}

int compare(char *a, char *b)
{
	double ad, bd;
	bool ac, bc;
	int ab, bb;
	int ae, be;
	int alen, blen;
	bool aempty, bempty;
	char *endptr;
	ab = strbegin(a);
	ae = strend(a);
	bb = strbegin(b);
	be = strend(b);
	alen = ae - ab + 1;
	blen = be - bb + 1;
	aempty = (alen <= 0);
	bempty = (blen <= 0);
	if (aempty && bempty) {
		return 0;
	} else if (aempty) {
		return -1;
	} else if (bempty) {
		return 1;
	} else {
		ac = false;
		bc = false;
		ad = strtod(a + ab, &endptr);
		if (endptr == a + ae + 1)
			ac = true;
		bd = strtod(b + bb, &endptr);
		if (endptr == b + be + 1)
			bc = true;
		if (ac && bc) {
			if (fabs(ad - bd) < 0.0001)
				return 0;
			else if (ad < bd)
				return -1;
			else
				return 1;
		} else if (ac) {
			return -1;
		} else if (bc) {
			return 1;
		} else {
			return lexcmp(a + ab, alen, b+ bb, blen);
		}
	}
}

int lexcmp(char *a, int alen, char *b, int blen)
{
	int i, j;
	int cmp;
	i = 0;
	j = 0;
	while (i < alen && j < blen) {
		while (a[i] < '\0' || isspace(a[i]))
			i++;
		while (b[j] < '\0' || isspace(b[j]))
			j++;
		if (i == alen && j == blen)
			return 0;
		else if (i == alen)
			return -1;
		else if (j == blen)
			return 1;
		else if ((cmp = charcmp(a[i], b[j])))
			return cmp;
		i++;
		j++;
	}
	if (i >= alen && j >= blen)
		return 0;
	else if (i >= alen)
		return -1;
	else
		return 1;
}

int charcmp(char a, char b)
{
	if (isalpha(a) && isalpha(b)) {
		if (toupper(a) == toupper(b)) {
			if (isupper(a) && isupper(b))
				return 0;
			else if (isupper(a))
				return -1;
			else if (isupper(b))
				return 1;
			else
				return 0;
		} else if (toupper(a) < toupper(b)) {
			return -1;
		} else {
			return 1;
		}
	} else if (toupper(a) == toupper(b)) {
		return 0;
	} else if (toupper(a) < toupper(b)) {
		return -1;
	} else {
		return 1;
	}
}

int strbegin(char *str)
{
	int i, len, begin;
	if (str)
		len = strlen(str);
	else
		len = 0;
	begin = len;
	i = 0;
	for (i = 0; i < len; i++) {
		if (str[i] > '\0' && !isspace(str[i])) {
			begin = i;
			break;
		}
	}
	return begin;
}

int strend(char *str)
{
	int i, len, end;
	if (str)
		len = strlen(str);
	else
		len = 0;
	end = -1;
	for (i = len - 1; i >= 0; i--) {
		if (str[i] > '\0' && !isspace(str[i])) {
			end = i;
			break;
		}
	}
	return end;
}

int min(int a, int b)
{
	return a <= b ? a : b;
}
