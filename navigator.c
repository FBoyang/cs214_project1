#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<getopt.h>
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


int navigation(char *name, char *output_path, FILE *ptr){
	DIR *dir;
	pid_t t_pid[256];
	struct dirent *entry;
	int status;
	int process_num =0;
	int counter = 0;
	pid_t pid = 0; 
	pid_t ppid = 0;


	int i ;
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
													
						//sort_csv(entry -> d_name, field_name, NULL, output_path); 
							
					_exit(1);
					}
					else if(pid > 0){
						ppid = getpid();
						fprintf(ptr, "parent %d generates child %d in path %s to process file: %s\n", ppid, pid, name, entry -> d_name);
						fflush(ptr);
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
						ppid = getpid();
						fprintf(ptr, "parent %d generates child %d in path %s to process directory: %s\n", ppid, pid, name, entry -> d_name);
						fflush(ptr);
						t_pid[counter-1] = pid;
						continue;
					}
					if (pid == 0){
						char file[1024];
						strcpy(file, entry -> d_name);
						strcat(name, "/");
						strcat(name, file);
						//printf("output path is %s\n", name);
						process_num = navigation(name, output_path, ptr);
						//printf("path %s has process_num %d\n", output_path, process_num);

						_exit(process_num);
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
	
	}	
	process_num = process_num + 1; //the parent process
	pid_t c_pid = getpid();	
	
	if(c_pid != ini_pid){
		//printf("process number is %d\n", process_num);
		return process_num;
	}
	
	else{
		//printf("process number is %d\n", process_num);
		return process_num;
	}
	
	
}

int main(int argc, char** argv){
	int status;
	int i;
	int c = 0;
	int cflag = 0;
	int oflag = 0;
	int dflag = 0;

	/*for(i = 1; i < argc; i++){
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
	}*/
	char input_path[1024];
	char *output_path;
	getcwd( input_path, 1024);		
	while((c = getopt (argc, argv, "c: d: o:")) != -1){
		switch (c)
		{
		case 'c':
			cflag = 1;
			field_name = optarg;
			break;
		case 'd':
			dflag = 1;
			if (optarg == NULL){
				fprintf(stderr, "no start path\n");
				exit(1);
			}
			strcpy(input_path, optarg);
			break;
		case 'o':
			oflag = 1;
			if (optarg == NULL){
				fprintf(stderr, "no output path\n");
				exit(1);
			}
			output_path = optarg;
			break;
		case '?':
			fprintf(stderr, "Unknown option\n");
			exit(1);
			break;
		}
	}	 
	if(cflag == 0 || argc < 3){
		fprintf(stderr, "wrong argument\n");
		exit(1);
	}
	process_num = 0;
		// use default path	
	FILE *ptr;
	
	ptr = fopen("pid_infor.txt", "w");	
	
	ini_pid = getpid();
	printf("Initial PID: %d\n", ini_pid);
	fprintf(stdout,"PIDS of all child processes:");
	fflush(stdout);	
	process_num = navigation(input_path, output_path, ptr);
	fprintf(stdout, "\n");
	fflush(stdout);
	
	printf("Total number of processes: %d\n", process_num);	
	fflush(stdout);
	

	return 0;
}




