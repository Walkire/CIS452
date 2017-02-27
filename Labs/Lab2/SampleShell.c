#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char* argv[]) {

	char* arr[1000];
	char inputs[1000];
	int status;
	pid_t pid; 	
	arr[0] = "hello";
	struct rusage usage;
	struct timeval start, end;

	while(1) {
		
		printf(">>> ");
		fgets(inputs, 1000, stdin);
	        
		arr[0] = strtok(inputs," ");
		int i = 1;

		do {
			arr[i] = strtok(NULL, " ");
			i++;
		}
		while (arr[i-1] != NULL);

		arr[i-2][strlen(arr[i-2])-1] = '\0';

		if (strcmp(arr[0], "quit") == 0) {
			break;
		}
		
		//Fork
		getrusage(RUSAGE_CHILDREN, &usage);
		start = usage.ru_utime;
		long startcs = usage.ru_nivcsw;
		if((pid = fork()) < 0) {
			perror("fork failure\n");
			exit(1);
		}
		else if(pid == 0) {
			if (execvp(arr[0], &arr[0]) < 0) {
				perror("exec failed\n");
				exit(1);
			}
		}
		else{
			wait(&status);
			getrusage(RUSAGE_CHILDREN, &usage);
			end = usage.ru_utime;
			
			printf("\n**End time: %lu.%06ld sec**\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
			printf("**Number of involuntary context switches: %ld**\n", usage.ru_nivcsw-startcs); 
		}
		
	}

	return 0;
