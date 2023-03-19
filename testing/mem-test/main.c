#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    pid_t pid;
    int status;
    int size = 1024;
    char *str = "Hello, child process!";
    char *ptr = NULL;

    // create child process
    pid = fork();

    if (pid == -1) {
        printf("Failed to create child process.\n");
        exit(1);
    } else if (pid == 0) {
        // child process
        ptr = (char*)malloc(size); // allocate memory block of size 1024 bytes
        strcpy(ptr, str); // copy string to memory block
        printf("Memory contents of child process:\n");
        for (int i = 0; i < size; i++) {
            printf("c %x ", *((unsigned char*)ptr + i)); // print the values of each byte in the memory block
        }
        printf("\n");
        free(ptr); // free the memory block
        exit(0);
    } else {
        // parent process
        printf("Memory contents of parent process:\n");
        for (int i = 0; i < size; i++) {
            printf("p %x ", *((unsigned char*)ptr + i)); // try to access the memory of the child process from the parent process
        }
        printf("\n");
        waitpid(pid, &status, 0); // wait for child process to finish
    }

    return 0;
}
