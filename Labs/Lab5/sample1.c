#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

int main () {
    int shmId;
    char *shmPtr;
    struct shmid_ds seg;

    if ((shmId =
         shmget (IPC_PRIVATE, FOO,
                 IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    //Question 5
    printf ("value a: %lu\t value b: %lu\nID of shared memory: %d\n", 
            (unsigned long) shmPtr,
            (unsigned long) shmPtr + FOO,
	    shmId);
    sleep(5);
    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_STAT, &seg) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
    //Question 4
    printf("Size of segment: %zd\n", seg.shm_segsz);

    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }

    return 0;
}

