#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char *argv[]) {

	if (argc < 3) {
		printf("Usage: %s optional[-s] [Filename1] [Filename2]\n",argv[0]);
		exit(1);
	}

	if ((strcmp(argv[1], "-s")) == 0) {
		if ((symlink(argv[2], argv[3])) < 0) {
			perror("Unable to symbolic link");
			exit(1);
		}
		
		printf("%s and %s symbolic link\n",argv[2],argv[3]);	
	}
	else{
		if ((link(argv[1], argv[2])) < 0) {
			perror("Unable to hard link\n");
			exit(1);
		}

		printf("%s and %s hard linked\n", argv[1], argv[2]);
	}
	return 0;

}
