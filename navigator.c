#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<getopt.h>
#include<unistd.h>
#include"sorter.h"
#include<libgen.h>
#include<sys/wait.h>

/*
* navigation function serves as the function to recursively traverse the directroies,
* first argument name is the start directory from which we begin to search. The function
* first traverse the content in the current directory, if not a directory, check the
* validity of the csv file format and the number of field name inside of the file, otherwise
* go to the next directory and do the same procedure again.
* if no sub directory in the current directory, stop fork.
*/


int navigation(char *name, char *output_path, FILE *ptr, int indent, char* dir_name) {
	DIR *dir;
	pid_t t_pid[256];
	int dir_index[256];
	
	struct dirent *entry;
	int status;
	int process_num = 0;
	int counter = 0;
	pid_t pid = 0;
	pid_t ppid = 0;


	int i;
	for (i = 0; i < 256; i++) {
		t_pid[i] = 0;
		dir_index[i ] = 0;
	}
	char *filelist[256];



	if (!(dir = opendir(name))) {
		printf("\ndirectory name is %s\n", name);
		fprintf(stderr, "no such directory exist\n");
		exit(0);
	}
	else {


		while (entry = readdir(dir)) {

			if (entry->d_type == DT_REG) {
				if (strstr(entry->d_name, ".csv")) {
					pid = fork();
					counter++;
					if (pid == 0) {

						//check the csv file 

						sort_csv(entry -> d_name, field_name, name, output_path); 

						_exit(1);
					}
					else if (pid > 0) {
						ppid = getpid();
						filelist[counter - 1] = malloc(strlen(entry -> d_name) + 2);
						strcpy(filelist[counter - 1], entry -> d_name);
						//fprintf(ptr, "parent %d generates child %d in path %s to process file: %s\n", ppid, pid, name, entry->d_name);
						//fflush(ptr);
						t_pid[counter - 1] = pid;
						continue;
					}

					else {
						//error case
					}


				}
			}


			if (entry->d_type == DT_DIR) {
				if (entry->d_name[0] != '.') {
					pid = fork();
					counter++;
					t_pid[counter - 1] = pid;
					if (pid != 0) {
						ppid = getpid();
						//fprintf(ptr, "parent %d generates child %d in path %s to process directory: %s\n", ppid, pid, name, entry->d_name);
						//fflush(ptr);
						filelist[counter - 1 ] = malloc(strlen(entry -> d_name) + 2);
						strcpy(filelist[counter - 1], entry -> d_name); 
						t_pid[counter - 1] = pid;
						dir_index[counter -1] = 1;
						continue;
					}
					if (pid == 0) {
						char file[1024];
						strcpy(file, entry->d_name);
						strcat(name, "/");
						strcat(name, file);
						//printf("output path is %s\n", name);
						indent ++;
						process_num = navigation(name, output_path, ptr, indent, entry -> d_name);
						//printf("path %s has process_num %d\n", output_path, process_num);

						_exit(process_num);
					
					}
				}
			}
		}
	}

	int c_process_num = 0;
	if (pid > 0) {
		int i;
		
		if(indent >= 1){
			for (i = 0; i < 30* (indent-1) + 27 ; i ++){
				fprintf(ptr, " ");
			}
			//fflush(ptr);
			fprintf(ptr, "---");
			fprintf(ptr, "|");
			//fflush(ptr);
		}		
		indent = indent + 1;
		fprintf(ptr, "%s: pid is %d --|\n",dir_name, getpid());
		//fflush(ptr);	
		for (i = 0; i < counter; i++) {
			waitpid(t_pid[i], &status, 0);
			fprintf(stdout, "%d,", t_pid[i]);
			fflush(stdout);
			c_process_num = WEXITSTATUS(status);
			int m;
			if(dir_index[i] == 0){
				for( m = 0; m < c_process_num; m++){
					int j;
					// get the indent
					for( j = 0; j < 30 * (indent - 1) + 27; j ++)
						fprintf(ptr, " ");
					//fflush(ptr);
					fprintf(ptr, "---");
					fprintf(ptr, "|");
					//fflush(ptr);
					//leave a line for a child
					if(m == 0){
						fprintf(ptr, "%s: pid is %d\n", filelist[i], t_pid[i]);
						free(filelist[i]);
					}
							
					else	{fprintf(ptr, "\n");}	
						//fflush(ptr);}			
				}
				
				fprintf(ptr, "\n");
			}
			process_num += c_process_num;
		}
		fflush(ptr);
	}
	process_num = process_num + 1; //the parent process
	pid_t c_pid = getpid();
	closedir(dir);
	if (c_pid != ini_pid) {
		//printf("process number is %d\n", process_num);
		return process_num;
	}

	else {
		//printf("process number is %d\n", process_num);
		return process_num;
	}

	
}

int main(int argc, char** argv) {
	int status;
	int i;
	int c = 0;
	int cflag = 0;
	int oflag = 0;
	int dflag = 0;
	struct dirent *entry;
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
	char *output_path = NULL;
	getcwd(input_path, 1024);
	while ((c = getopt(argc, argv, "c: d: o:")) != -1) {
		switch (c)
		{
		case 'c':
			cflag = 1;
			field_name = optarg;
			break;
		case 'd':
			dflag = 1;
			if (optarg == NULL) {
				fprintf(stderr, "no start path\n");
				exit(1);
			}
			strcpy(input_path, optarg);
			break;
		case 'o':
			oflag = 1;
			if (optarg == NULL) {
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
	if (cflag == 0 || argc < 3) {
		fprintf(stderr, "wrong argument\n");
		exit(1);
	}
	process_num = 0;
	// use default path	
	FILE *ptr;
	int indent = 0;
	ptr = fopen("pid_infor.txt", "w");
	char path[1024];
	getcwd(path, 1024);
	char *file_name = basename(path);
	ini_pid = getpid();
	printf("Initial PID: %d\n", ini_pid);
	fprintf(stdout, "PIDS of all child processes:");
	fflush(stdout);
	//printf("parent is %s\n", entry -> d_name);
	printf("output path is %s\n", output_path);
	process_num = navigation(input_path, output_path, ptr, indent, file_name);
	fprintf(stdout, "\n");
	fflush(stdout);
	
	printf("Total number of processes: %d\n", process_num);
	fflush(stdout);
	fclose(ptr);

	return 0;
}
