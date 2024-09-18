#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main() {
    int fd[2];
    int pipeCreationResult;
    int pid;

    pipeCreationResult = pipe(fd);
    if (pipeCreationResult < 0) {
        perror("Failed pipe creation");
        exit(1);
    }

    pid = fork();
    if (pid < 0) { // Fork failed
        perror("Fork failed");
        exit(1);
    }

    char childToParent[500];
    char userInput[500];

    if (pid == 0) {
        close(fd[READ]); // Asked ChatGPT how to close()
        printf("Enter a string (up to 500 characters): ");
        fgets(userInput, sizeof(userInput), stdin);

        // Asked ChatGPT how to remove the new line
        size_t len = strlen(userInput);
        if (userInput[len - 1] == '\n') {
            userInput[len - 1] = '\0';
        }

        write(fd[WRITE], userInput, strlen(userInput) + 1);
        printf("Child sent this message: \"%s\"\n", userInput);
        close(fd[WRITE]); 
    } else { 
        close(fd[WRITE]); 
        read(fd[READ], childToParent, sizeof(childToParent));
        printf("Parent received this message: \"%s\" from child process\n", childToParent);
        close(fd[READ]);
    }

    return 0;
}

