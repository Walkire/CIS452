#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define FOO 4096

void sigHandler(int);
int shmId;

int main() {
    signal(SIGINT, sigHandler);
    key_t key;
    char *shmPtr;
    char input[FOO];
    char *newInput;
    int quit = 1;

    key = ftok("reader.c", 'R');
    

    if ((shmId =
         shmget (key, FOO,
                 IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    shmPtr[0] = '1';

    while(quit) {
      fgets(input, FOO-1, stdin);
      newInput = malloc(strlen(input)*sizeof(char));
      newInput = strtok(input, "\n");
      strcpy(&shmPtr[1], input);
      shmPtr[0] = '0';
      if(strcmp("quit", newInput) == 0){
        kill(getpid(), SIGINT);
      }
      while(1){
        //1 == good to write
        if(shmPtr[0] == '1'){
          break;
        }
      }   
    }
}

void sigHandler(int sig){
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
    exit(0);
}
