#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"sorter.h"


	
/*
 * navigation function serves as the function to recursively traverse the directroies, 
 * first argument name is the start directory from which we begin to search. The function
 * first traverse the content in the current directory, if not a directory, check the 
 * validity of the csv file format and the number of field name inside of the file, otherwise
 * go to the next directory and do the same procedure again.
 * if no sub directory in the current directory, stop fork.
 */


int navigation(char *name, char *output_path){
	DIR *dir;
	pid_t t_pid[256];
	struct dirent *entry;
	int status;
	int counter = 0;
	pid_t pid = 0; 


		int i;
		for (i = 0; i < 32; i++){
			t_pid[i] = 0;
		}

		
	
	
		if( !(dir = opendir(name) )){
			printf("\ndirectory name is %s\n", name);
			fprintf(stderr, "no such directory exist\n");
			exit(0);
		}
		

		else{
		

			while( entry = readdir(dir) ){
			
				if(entry -> d_type == DT_REG){	
					if(strstr(entry -> d_name, ".csv")){
						pid = fork();
						counter++;
						if(pid == 0){
						
						//check the csv file 
							char outfile[128];
							strncpy(outfile, entry -> d_name, strlen(entry -> d_name) - 4); //remove the .csv postfix

							strcat(outfile, "-sorted");						
							int i;
							for (i = 0; i < sort_num; i++){
								strcat(outfile, "-");
								strcat(outfile, sort_list[i]);
							}
							strcat(outfile, ".csv");
							
							//sort_csv(entry -> d_name, sort_list, outfile, output_path); 
							
							_exit(1);
						}
						else if(pid > 0){
							t_pid[counter -1] = pid;
							continue;
						}
	
						else{
							//error case
						}	
	
						
					}
				}
				

				if (entry -> d_type == DT_DIR){
					if(entry -> d_name[0] != '.'){
						pid = fork();
						counter++;
						t_pid[counter - 1] = pid;
						if(pid != 0){
							t_pid[counter-1] = pid;
							continue;
						}
						if (pid == 0){
							char file[1024];
							strcpy(file, entry -> d_name);
							strcat(name, "/");
							strcat(name, file);
							navigation(name, output_path);

							_exit(1);
						}


					}
				}

			}
						

		}

		
	if( pid > 0){
		int i;
		for (i = 0; i < counter; i++){
			waitpid( t_pid[i], &status, 0);
			fprintf(stdout, "%d,", t_pid[i]);
			fflush(stdout);
			process_num += WEXITSTATUS(status);
		}
		process_num = process_num + 1; //the parent process
	}
	pid_t c_pid = getpid();	
	
	if(c_pid != ini_pid){
		_exit(process_num);
	}
	
	else
		return process_num;
	
	
}

int main(int argc, char** argv){
	int status;
	int i;
	int c = 0;
	int d = 0;
	int o = 0;

	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-c") == 0){
			c= i;
		}
		else if(strcmp(argv[i], "-d") == 0){
			d = i;
		}
		else if(strcmp(argv[i], "-o") == 0){
			o = i;
		}
		else{
			continue;
		}
	}
	
	for( i = c + 1; i < argc; i++){
		if((strcmp(argv[i], "-d") == 0)||(strcmp(argv[i], "-o") == 0)){
			break;
		}
		else{
			strcpy(sort_list[i -c -1], argv[i]);
		}
	}
	sort_num = i - c - 1;		
	process_num = 0;
	char input_path[1024];
	if(d == 0){
		// use default path	
		getcwd( input_path, 1024);		
	}
	else{
		strcpy(input_path, argv[d+1]);
	}
	pid_t pid; 
	//initialize the process number
	pid = fork();
	if(pid == 0){
		ini_pid = getpid();
		printf("Initial PID: %d\n", ini_pid);
		fprintf(stdout,"PIDS of all child processes:");
		fflush(stdout);
		if(o == 0){
			// output to the default directory
			process_num = navigation(input_path, NULL);
		}
		else{
			process_num = navigation(input_path, argv[o+1]);
		}
		printf("\n");
		_exit(process_num);
	}
	else{
		waitpid(pid, &status, 0);
		process_num = WEXITSTATUS(status);
	}

	printf("Total number of processes: %d\n", process_num);	
	

	return 0;
}




