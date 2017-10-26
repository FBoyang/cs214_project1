#ifndef _SORTER_H
#define _SORTER_H
/*
 * sorts the file with name "infile"
 * by the fields listed in "fieldlist"
 * and outputs the result in the directory "dirname"
 */
int sort_csv(char *infile, char *fieldlist, char *dirname);
#endif
