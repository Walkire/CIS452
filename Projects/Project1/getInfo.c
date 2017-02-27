//getInfo.c
//Author: Nathaniel P Allvin

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>

int main() {

	char hostname[100];
	char *loginname;

	//Print user login name -- getlogin()
	loginname = getlogin();
	printf("User is: %s\n", loginname);

	//Print user ID -- getuid()
	printf("UID is: %d\n", getuid());

	//Print group ID -- getgid()
	printf("GID is: %d\n", getgid());	

	//Print hostname -- gethostname()
	gethostname(hostname, sizeof(hostname));
	printf("Host is: %s\n",hostname);
	
	//Print names of first 5 entries in password file -- getpwent()
	struct passwd* entry;
	int i = 0;

	while(i < 5) {
		entry = getpwent();
		printf("entry %d: %s\n",i,entry->pw_name);
		i++;
	}	

	//Print value of user-specified environment variable -- getenv(name)
	char *enviro;
	char input[50];
	
	printf("\nenvironment variable desired?");
	scanf("%s",input);

	enviro = getenv(input);

	printf("\nvalue is: %s\n",enviro);
	
	return 0;
}
