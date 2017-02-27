#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#define READ 0
#define WRITE 1

void complementer();
void incrementer();
void adder();
void sigHandler(int);

int fd[2];
int fd2[2];
int lineSize;
pid_t pid, pid1;
char doneChar = 'd';

int main(int argc, char *argv[]){

  if(argc != 4) {
    fprintf(stderr, "Usage: %s [file containing vectorA] [file containing vectorB] [size of line]\n", argv[0]);
    return 1;  
  }
  signal(SIGINT, sigHandler);
  sscanf(argv[3], "%d", &lineSize);
  lineSize += 2;
  fprintf(stderr, "lineSize declared as %d\n", lineSize);

  complementer(argv[1], argv[2]);
  return 0;
}

void complementer(char *fileA, char *fileB){

  //Pipe / Fork / Send
  fprintf(stderr, "Creating pipe(fd)...\n");
  if(pipe(fd) < 0) {
    perror("Issue creating pipe fd\n");
    exit(1);
  }
  fprintf(stderr, "Created pipe(fd)\n");
  fprintf(stderr, "Creating child...\n");
  if((pid = fork()) < 0) {
    perror("Issue creating child\n");
    exit(1);
  }
  if (!pid) {
    fprintf(stderr, "Created child, sending to incrementer\n");
    incrementer(fileA);
  }

  //File B opening
  char currentChar;
  FILE *file_B;

  pause();
  
  fprintf(stderr, "Parent: Opening File B\n");
  if ((file_B = fopen(fileB, "r")) == NULL) {
    perror("Parent: Issue opening file\n");
    exit(1);
  }
  fprintf(stderr, "Parent: File B opened\n");  
  
  //Assign pipe input/output
  fprintf(stderr, "Parent: Duping and closing\n");
  dup2 (fd[WRITE], STDOUT_FILENO);
  close (fd[READ]);
  close (fd[WRITE]);

  //Read File / Send to pipe
  fprintf(stderr, "Parent: Begin loop\n");
  while((currentChar = fgetc(file_B)) != EOF) { 
    //flips the bits
    if((char)currentChar == '0' || (char)currentChar == '1'){
      if ((char)currentChar == '0') {
        currentChar = '1';
      }
      else{
        currentChar = '0';
      }
    }
    if(currentChar == '1' || currentChar == '0'){
      fprintf(stderr, "Parent: Sending char (%c) to child\n", currentChar);
    }
    else{
      fprintf(stderr, "Parent: Sending char newline or null to child\n");
    }
    write(STDOUT_FILENO, &currentChar, sizeof(currentChar));
  }
  //sends 'd' to child and grandchild to notify completion
  fprintf(stderr, "Parent: Sending char (%c) to child\n", doneChar);
  write(STDOUT_FILENO, &doneChar, sizeof(doneChar));

  //Close file and wait for child
  fclose(file_B);
  fprintf(stderr, "Parent done, waiting for children\n");
  wait(NULL);
}

void incrementer(char *fileA){
  
  //Pipe / Fork / Send child away
  fprintf(stderr,"Creating pipe(fd2)...\n");
  if(pipe(fd2) < 0) {
    perror("Issue creating pipe fd2\n");
    exit(1);
  }
  fprintf(stderr, "Created pipe(fd2)\n");
  fprintf(stderr, "Creating grandchild...\n");
  if((pid1 = fork()) < 0) {
    perror("Issue creating grandchild\n");
    exit(1);
  }
  if (!pid1) {
    fprintf(stderr, "Created grandchild, sending to adder\n");
    adder(fileA);
  }
  
  char currentChar;
  char currentLine[lineSize];
  int counter = 0;  
  
  //Assign both pipe input/outputs
  fprintf(stderr, "Child: duping and closing\n");
  dup2(fd2[WRITE],STDOUT_FILENO); 
  dup2(fd[READ], STDIN_FILENO);
  close(fd[READ]);
  close(fd[WRITE]);
  close(fd2[READ]);
  close(fd2[WRITE]);
  
  //Read pipe(fd) char by char / build a char string / add 1 / send
  fprintf(stderr, "Child: Begin Loop\n");
  read(STDIN_FILENO, &currentChar, sizeof(currentChar));
  while(currentChar != 'd'){ //parent will send 'd' when it is done
    if(counter < lineSize){
      currentLine[counter] = currentChar;
      //when line is filled to lineSize
      if (counter == lineSize-1) {
        counter = counter - 2;  //to ignore /n and /0
        //loops through reading left to right changing all
        //1's to 0's until it finds a 1 then it leaves the loop
        while (counter >= 0) {
          if(currentLine[counter] == '1'){
            currentLine[counter] = '0';
          } 
          else {
            currentLine[counter] = '1';
            break;
          }
          counter--;
        }
        //write to pipe and reset counter
        fprintf(stderr, "Child: Sending line (%s) to grandchild\n", currentLine);
        write(STDOUT_FILENO, (const void*)currentLine, sizeof(currentLine));
        counter = -1;
      }
      counter++;
    }
    //reads next char
    read(STDIN_FILENO, &currentChar, sizeof(currentChar));
  }

  //writes the same 'd' char to grandchild
  //finishes and waits for grandchild
  write(STDOUT_FILENO, &doneChar, sizeof(doneChar));
  fprintf(stderr, "Child done, waiting for Grandchild\n");
  wait(NULL);
  exit(0);
}

void adder(char *fileA){
  
  char currentCharA, currentCharB;
  char currentLineA[lineSize];
  char currentLineB[lineSize];
  FILE *file_A;
  FILE *output;
  int counter = 0;
  int carry = 0;

  //duping and closing the respected fd's
  fprintf(stderr, "Grandchild: duping and closing\n");
  dup2(fd2[READ], STDIN_FILENO);
  close(fd2[WRITE]);
  close(fd2[READ]);

  //FileA and out.dat opening
  fprintf(stderr, "Grandchild: Opening fileA and output file\n");
  if((file_A = fopen(fileA, "r")) == NULL){
    perror("Issue opening fileA\n");
    exit(1);
  }
  if((output = fopen("out.dat", "w")) == NULL) {
    perror("Issue opening output file\n");
    exit(1);
  }

  //Begin adding
  fprintf(stderr, "Grandchild: Begin Loop\n");
  while((currentCharA = fgetc(file_A)) != EOF){
    read(STDIN_FILENO, &currentCharB, sizeof(currentCharB));
    if (counter < lineSize){
      currentLineA[counter] = currentCharA;
      currentLineB[counter] = currentCharB;
      //when the currentLines are filled being adding A + (-B)
      //This is done by checking the bits from left to right and compairing them
      //with the carry.
      if(counter == lineSize-1) {
        counter = counter - 2;
        while(counter >= 0){
          //if Abit = 1 and Bbit = 1
          if(currentLineA[counter] == '1' && currentLineB[counter] == '1') {
            if(carry == 0){
              currentLineB[counter] = '0';
	      carry = 1;
            }
            else{
              currentLineB[counter] = '1';
              carry = 1;
            }
	  }
          //if Abit = 0 and Bbit = 1 or Abit = 1 and Bbit = 0
          else if((currentLineA[counter] == '0' && currentLineB[counter] == '1') || 
            (currentLineA[counter] == '1' && currentLineB[counter] == '0')){
            if(carry == 0){
              currentLineB[counter] = '1';
              carry = 0;
            }
            else{
              currentLineB[counter] = '0';
              carry = 1;
            }
          }
          //if Abit = 0 and Bbit = 0
          else if(currentLineA[counter] == '0' && currentLineB[counter] == '0'){
            if(carry == 0){
              currentLineB[counter] = '0';
              carry = 0;
            }
            else{
              currentLineB[counter] = '1';
              carry = 0;
            }
          }
          counter--;
	}
        //put result in output file and reset carry and counter
        fprintf(stderr, "Grandchild: printing (%s) to out.dat file\n", currentLineB);
        //currentLineB[sizeof(currentLineB)-1] = '\0';
        fputs(currentLineB, output);
        carry = 0;
        counter = -1;
      }
    }
    counter++;
  }

  //close files and finish
  fclose(file_A);
  fclose(output);
  fprintf(stderr, "Grandchild is done\n");
  exit(0);
}

void sigHandler(int sig) {
  fprintf(stderr,"Ctrl-c recieved - continuing process");
}
