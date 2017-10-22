#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"sorter.h"
void column_count(char *str, int *column){
	//FILE *file = fopen(file_name, "r");
	*column = 0;
	//fgets(str, 1024, file);
	char* token = strtok(str, ",");
	while(token)
	{	
		if(token[0] != '"')
		{
			(*column)++;
			token = strtok(NULL, ",");
		}
		else
		{	

			token = strtok(NULL, "\"");
		}
			
	}
/*	while(fgets(str, 1024, file))
	{
		(*row)++;
	}
	(*row)++;
	//fclose(file);
*/
}


void matrix_free(int row, int column, struct record **matrix){
	int i;
	int j;
	for (i = 0; i < column; i++)
	{
		for (j = 0; j < row; j++)
		{
			/*if(i == 2)
			{
				printf("row num is %d\n", j);
			}
			*/
			
			free(matrix[i][j].string);
			//only need to free the string
			//because only string in the struct mallocate
		}
		free(matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
	matrix = NULL;
}
/* matrix_enlarge function dynamically allocate the matrix
 */
void matrix_enlarge(int row, int column, struct record **matrix){
	int i = 0;
	for (i = 0; i < column; i++)
	{
		matrix[i] = realloc((matrix)[i], row*sizeof(struct record));
	}
}









int main(int argc, char* argv[]){

//	char *name = "movie_metadata.csv";
//	FILE *file = fopen("movie_metadata.csv", "r");

	if (argc < 3) {
		fputs("please give a field by which to sort\n", stderr);
		return 0;
	}
	 int row_num = 5000;
	 feature_num = 0;
	//counter( &row_num, &feature_num);
	FILE *file = stdin;
	int ini_size = 1024;	
	char *str = NULL;
	size_t n = 0;
	getline(&str, &n, file);




	
	char *str_temp = malloc(ini_size);
	memcpy(str_temp, str, strlen(str) + 1);
	
	column_count(str_temp ,&feature_num);

	
	record_table = malloc(sizeof(struct record*)*feature_num);
	
	//int i;
	//for(i = 0; i< feature_num; i++)
	//{
	//	record_table[i] = malloc(sizeof(struct record)*row_num);
	//	}

	matrix_enlarge(row_num, feature_num, record_table);
	feature_name = malloc(sizeof(char*)*feature_num);
				
	row_counter = 0;
	int column_counter = 0;

	char* tokens = strtok(str, "\r\n,");
	
	char* prev_tokens;
	
	int str_len = strlen(str) - 2;
	
	int pstr_len = 0;
	column_counter = 0;
	prev_tokens = str;
	

	while(tokens)
	{	
		//record_table[column_counter] = malloc(sizeof(struct record)*(row_num+1));
		if(prev_tokens[0] == ',')
		{
			pstr_len = -1;
		}

		int p_diff = tokens - (prev_tokens + pstr_len);
		if(p_diff <= 1)
		{
			if(tokens[0] != '"')
			{	
				prev_tokens = tokens;		
				feature_name[column_counter] = malloc(sizeof(char)*(strlen(tokens) + 1));
				strcpy(feature_name[column_counter++], tokens);
				pstr_len = strlen(prev_tokens);
	
		 	}

			else
			{
				//embeded comma
				prev_tokens = tokens;
				char special_tokens[200];
				strcpy(special_tokens, tokens);
				tokens = strtok(NULL, "\"");
				strcat(special_tokens, tokens);
				pstr_len = strlen(special_tokens) + 2;
				feature_name[column_counter] = malloc(sizeof(char)*(strlen(tokens) + 1));
				strcpy(feature_name[column_counter++], special_tokens+1);
			}
				
				tokens = strtok(NULL, ",\r\n");
			
		}

		else
		{
		//contain empty cell
			fprintf(stderr, "no field name in column %d\n", column_counter);
		}
	}
		
	/*
 * 	this part serves to read the record and store them into the record struct
 */
	ini_size = 1024;
	while(getline(&str, &n, file) != -1)
	{
	
		//printf("%s\n", tokens);
			str_len = strlen(str) - 2;//exclude the \n
			int pstr_len = 0;
			column_counter = 0;		
			tokens = strtok(str, ",\r\n");
			prev_tokens = str;

			
			while(tokens)
			{
				//pstr_len = strlen(prev_tokens);
				if(prev_tokens[0] == ',')
				{
					pstr_len = -1;
				}

	
				int p_diff = tokens - (prev_tokens + pstr_len);
				if(p_diff <= 1)
				{
		
						//treat the data as string
					
						if(tokens[0] != '"')
						{
							prev_tokens = tokens;
							pstr_len = strlen(prev_tokens);
							record_table[column_counter][row_counter].string = malloc(strlen(tokens)+2);
							strcpy(record_table[column_counter++][row_counter].string, tokens);
						}
						else
						{
							//embeded comma
							prev_tokens = tokens;
							char special_tokens[200];
							strcpy(special_tokens, tokens);
							strcat(special_tokens, ",");
							// make up the lost comma
							tokens = strtok(NULL, "\"");
							strcat(special_tokens, tokens);
							pstr_len = strlen(special_tokens) + 2; // one comma and one quotation mark miss
							//need to be optimized
							record_table[column_counter][row_counter].string = malloc(strlen(special_tokens)+2);
							strcpy(record_table[column_counter++][row_counter].string, special_tokens+1);
						}
							tokens = strtok(NULL, ",\r\n");
				 
				}
				else
				{
					int i;
					for(i =1; i < p_diff; i++)
					{
						record_table[column_counter++][row_counter].string = NULL;
					}
					
						
					prev_tokens = tokens;
					pstr_len = 0; //since prev_tokens and tokens point to the same possition
				}
				
					
				 
		
			
			}
			/*
 * 			check whether the last few cell are empty
 */
			
			int p_diff = str + str_len  - (strlen(prev_tokens) + prev_tokens);
			if(p_diff > 0)
			{
				int i;
				for (i = 0; i < p_diff; i++)
				{
					record_table[column_counter][row_counter].string = NULL;
				}
			}
		column_counter = 0;	
		row_counter++;
		if(row_counter >= row_num -2){
			row_num = row_num + 1000;
			matrix_enlarge(row_num, feature_num, record_table);
		}
		/*if((row_counter -1) % 100 == 0 && record_table[0][row_counter-1].string !=NULL)
		{
			printf("%s\n", record_table[0][row_counter-1].string);
			printf("%d\n", row_counter-1);
		}
		*/
					
	}
	

	//printf("row number is %d\n", row_counter);
	//printf("column number is %d\n", feature_num);

/*	free function
 */
	//printf(argc[2])
	sort_by_field(argv[2]);
	print_table();

	
	free(str);
	str = NULL;
	matrix_free(row_counter, feature_num, record_table);
	int i;
	for(i = 0; i < feature_num; i++)
	{
		free(feature_name[i]);
		feature_name[i] = NULL;
	}
	free(feature_name);	
	free(str_temp);
	feature_name = NULL;
	return 0;
}
