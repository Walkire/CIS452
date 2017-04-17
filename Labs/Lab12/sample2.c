#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat statbuf;

    dirPtr = opendir(".");

    while ((entryPtr = readdir(dirPtr))){
	printf("%-20s", entryPtr->d_name);
	stat(entryPtr->d_name, &statbuf);
	printf("\t%u Bytes\n",statbuf.st_size);
    }
    closedir(dirPtr);
    return 0;
}
