#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void *do_work(void *arg);
void sigHandler(int);
int numreq, threadnum, status;
pthread_t thread[10];

int main() {
  
  char temparr[1024];
  char userarr[10][1024];
  signal(SIGINT, sigHandler);
  srand((unsigned)time(NULL));
  threadnum = 0;

  while(1) {

    printf ("Name a file: ");
    fgets(temparr,1024,stdin);
    strcpy(userarr[threadnum], temparr);

    if((status = pthread_create(&thread[threadnum], NULL, do_work, &userarr[threadnum])) != 0) {
      fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
      exit(1);
    }
    threadnum++;
  }
}

void *do_work(void *arg) {
  char *val_ptr = (char *)arg;
  int randNum, randNum2;
  randNum = rand() % 5;

  if (!randNum){
    
    randNum2 = (rand() % 4) + 7;
    sleep(randNum2);
      
  }
  else {
    sleep(1);
  }

  numreq++;
  printf("File %s accessed\n", val_ptr);
  threadnum--;
  return NULL;
}

 void sigHandler(int sig){
   void *result;
   int i;
   threadnum--; 
   for(i = threadnum; i >= 0; i--) {
     if ((status = pthread_join(thread[i], &result)) != 0) {
       fprintf(stderr, "join error %d:%s\n", status, strerror(status));
       exit(1);
     }
   }
   printf("%d files accessed\n",numreq);
   exit(0);
 }
