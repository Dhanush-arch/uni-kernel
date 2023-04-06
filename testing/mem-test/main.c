/**
#include <stdio.h>
typedef unsigned long ul;

void g() {  printf("Gotcha!\n"); }

char *f (char *x) {
   int i;
   char *ptr;
   printf("Made it into f - returning now\n");
   return x;
}

int main () {
   int i;
   char s[8];
   printf("main=%lx, g=%lx\n",(ul)main, (long)g);
   for (i=0 ; i < 24 ; i++) s[i] = 'A';
   s[24] = 0x06;
   s[25] = 0x1F;
   s[26] = 0x1C;
   for (i=27 ; i < 31 ; i++) s[i] = 0x0;
   f(s);
   printf("Done with f\n");
}
**/
/**
#include <stdio.h>
int out;

void g(char *ptr) {
   int i;
   for (i=100 ; i > -5 ; i--) {
      if (*(ptr+i) < 90 && *(ptr+i) > 64) printf("%c",*(ptr+i));	
      else if (out == 1 && (unsigned char)(*(ptr+i)) < 16) printf("0%x",(unsigned char)(*(ptr+i)));
      else if (out == 1) printf("0%2x",(unsigned char)(*(ptr+i)));		
      else printf(".");
   }
   return;
}

void f(char *ptr) {
   int i;
   char s[10];
   s[0]='K'; s[1]='L'; s[2]='M'; s[3]='N'; s[4]='O'; 
   s[5]='P'; s[6]='Q'; s[7]='R'; s[8]='S'; s[9]='T';
   g(s);
   return;
}

int main (int argc, char **argv) {
   if (argc > 1) out = 1; else out = 0;
   char s[10];
   printf("main=%lx s=%lx\n", (unsigned long)main, (unsigned long)s);
   s[0]='A'; s[1]='B'; s[2]='C'; s[3]='D'; s[4]='E'; 
   s[5]='F'; s[6]='G'; s[7]='H'; s[8]='I'; s[9]='J';
   f(s);

   char st[2];
  st[0] = 'V';
  st[1] = 'W';
   printf("\n");
}**/
/**
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
 volatile int modified;
 char buffer[64];

 modified = 0;
 fgets(buffer, 73, stdin);

 if(modified != 0) {
  printf("you have changed the 'modified' variable\n");
 } else {
  printf("Try again?\n");
 }
}
**/
/**
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int modified = 0;
    char buffer[64];

    // Open input and output files

    // Read input from file

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        printf("Error: failed to fork\n");
        return 1;
    } else if (pid == 0) {
        // This is the child process

    fgets(buffer, 73, stdin);
        // Run vulnerable code
        if(modified != 0) {
          modified = 1;
        }
        // Write output to file
        printf("modified: %d", modified); 
        // Clean up and exit
        exit(0);
    } else {
        // This is the parent process

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {

            // Print result
            if (modified != 0) {
                printf("You have changed the 'modified' variable\n");
            } else {
                printf("Try again?\n");
            }
        } else {
            printf("Error: child process terminated abnormally\n");
        }

        // Clean up and exit
        return 0;
    }
}
**/

/**
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void win()
{
 printf("code flow successfully changed\n");
}

int main(int argc, char **argv)
{
 volatile int (*fp)();
 char buffer[64];

 fp = 0;

 fgets(buffer, 80, stdin);

 printf("%08x\n", fp);
 printf("%d\n", fp);

 if(fp) {
  printf("calling function pointer, jumping to 0x%08x\n", fp);
  fp();
 }
 printf("end");
 return 0;
}
**/

/**
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void win()
{
 printf("code flow successfully changed\n");
}

int main(int argc, char **argv)
{
 char buffer[56];
 volatile int (*fp)();
 fp = 0;
 fgets(buffer, 56, stdin);
 for(int i=-100;i<500;i++){
   printf("%c\n", (char)(*buffer+i));
 }
 printf("calling function pointer, jumping to 0x%08x\n", fp);
 //fp();
}

**/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void vulnerable_function(char *input) {
    char buffer[8];
    strcpy(buffer, input);
    printf("Buffer contents: %s\n", buffer);
}

void sigsegv_handler(int sig) {
    printf("Child process crashed with SIGSEGV\n");
    exit(1);
}

int main() {
    pid_t pid;
    int status;
    char input[100];
    printf("Enter a string: ");
    scanf("%s", input);

    pid = fork(); // create a child process
    if (pid == -1) {
        printf("Error: fork failed\n");
        return 1;
    } else if (pid == 0) {
        // child process
        signal(SIGSEGV, sigsegv_handler); // register the signal handler
        vulnerable_function(input);
    } else {
        // parent process
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
            printf("Parent process: Stack smashing issue detected\n");
        } else {
            printf("Parent process: Child process exited normally\n");
        }
    }

    return 0;
}

