#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define SIZE 16

int main (int argc, char *argv[]) {
    int status;
    long int i, loop, temp, *shmPtr;
    int shmId, semId;
    pid_t pid;
	struct sembuf swait, ssig;

    if(argc != 2) {
      fprintf(stderr, "Usage: %s [number of loops]\n", argv[0]);
      return 1;  
    }

	sscanf(argv[1], "%ld", &loop);
    fprintf(stdout, "loop: %ld\n", loop); 

    if ((shmId = shmget (IPC_PRIVATE, SIZE,
                         IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }
	if ((semId = semget(IPC_PRIVATE, 1, 00600)) < 0) {
		perror("Sem cant be created\n");
		exit(1);
    }
	if ((semctl(semId, 0, SETVAL, 1)) < 0) {
		perror("Sem couldnt be set\n");
		exit(1);
	} 

    shmPtr[0] = 0;
    shmPtr[1] = 1;

	swait.sem_num = 0;
	swait.sem_op = -1;
	swait.sem_flg = 0;

	ssig.sem_num = 0;
	ssig.sem_op = 1;
	ssig.sem_flg = 0;

    if (!(pid = fork ())) {
        for (i = 0; i < loop; i++) {

			semop(semId, &swait, 1);
			//Critical Section
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;
			/////////////////
			semop(semId, &ssig, 1);
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can 't let go\n");
            exit (1);
        }
        exit (0);
    }
    else {
        for (i = 0; i < loop; i++) {
			
			semop(semId, &swait, 1);
			//Critical Section
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;
			/////////////////
			semop(semId, &ssig, 1);
        }
    }

    wait (&status);
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
	if((semctl(semId, 0, IPC_RMID)) < 0) {
		perror ("Cant deallocate sem\n");
		exit(1);
	}
    return 0;
}
