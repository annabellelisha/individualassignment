#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define CODE_LENGTH 4
#define CODE_WAIT 2

void getCODE(char code[CODE_LENGTH +1]) {
  srand(getpid() + getppid());

  code[0]= 49 + rand() % 7;

  for(int i = 1; i < CODE_LENGTH; i++) {
    code[i] = 48 + rand() % 7;
  }
  code[CODE_LENGTH] = '\0';
}

void sigint_handler(int sig);

int main(void) {
  signal(SIGINT, sigint_handler);

  while(1){
   int pipefds[2];
   char code[CODE_LENGTH + 1];
   char buffer[CODE_LENGTH + 1];

  pipe(pipefds);

  pid_t pid = fork();

  if(signal(SIGINT, sigint_handler) == SIG_ERR) {
   perror("signal");
   exit(1);
   }

  if (pid == 0) {
   getCODE(code);
   close(pipefds[0]);
   write(pipefds[1], code, CODE_LENGTH + 1);
   
   printf("Sending code from child to parent...\n");

   sleep(CODE_WAIT);

   exit(EXIT_SUCCESS);
   }

  if (pid > 0) {
   wait(NULL);

   close(pipefds[1]);
   read(pipefds[0], buffer, CODE_LENGTH +1);
   close(pipefds[0]);
   printf("Parent recieved: '%s' \n\n", buffer);
   }
  }

 return EXIT_SUCCESS; 
}
  void sigint_handler(int sig){

  printf("Message was interrupted. \n");
  exit(1);
  }




