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
void navigation(char *name, pid_t pid){
	DIR *dir;
	struct dirent *entry;
	if(pid == -1){
		fprintf(stderr, "fail to fork\n");
		_exit(1);
	}

	else{
		if( pid != 0){
			waitpid( pid, NULL, 0);
			_exit(1);
		}
				
	
	
		if( !(dir = opendir(name) )){
			printf("directory name is %s\n", name);
			fprintf(stderr, "no such directory exist\n");
			_exit(1);
		}
		
		else if ( !entry -> d_name[0] == '.' ){
			_exit(1);
		}

		else{
		

			while( entry = readdir(dir) ){
				printf("file in the current directory is %s\n", entry -> d_name);	
			
				if(strstr(entry -> d_name, ".csv")){
					printf("valid file in node %lu\n", entry->d_fileno);
				}
				

				if (entry -> d_type == DT_DIR){
					if(entry -> d_name[0] != '.'){
						pid_t pid = fork();
						printf("pid is %d\n", pid);
						if (pid == 0){
							char file[1024];
							//printf("%s\n", entry -> d_name);
							strcpy(file, entry -> d_name);
							//printf("%s\n", file);
							printf("path is %s\n", name);
							strcat(name, "/");
							strcat(name, file);
							//printf("%s\n", path);
							printf("current pid is %d, current directory is %s\n",pid, name);
							navigation(name, pid);
						}
						//_exit(1);


					}
				}

			}
			_exit(1);
		}
	}	
}



int main(int argc, char** argv){
	if (argc == 3){//search in the current directory
		char dir_name[1024];
		getcwd( dir_name, 1024);
		navigation(dir_name, 0);
	}

	return 0;
}




