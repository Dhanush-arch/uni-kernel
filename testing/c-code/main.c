#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;
    char *args[] = {"ls", "-la", NULL};

    pid = fork();
    if (pid == 0) {
        // Child process
        printf("Executing child process...\n");
        args[0] = "c11";
        args[1] = "c12";
        args[2] = "c13";
        printf("Exec failed...\n");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error creating child process
        printf("Error creating child process...\n");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printf("Waiting for child process to finish...\n");
        printf("Child process finished...\n");
        printf("%s", args[0]);
        exit(EXIT_SUCCESS);
    }
}

