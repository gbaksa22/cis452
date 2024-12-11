#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define READ 0
#define WRITE 1
int main()
{
    int fd[2];
    int pipeCreationResult;
    int pid;
    pipeCreationResult = pipe(fd);
    if (pipeCreationResult < 0)
    {
        perror("Failed pipe creation\n");
        exit(1);
    }
    pid = fork();
    if (pid < 0) // Fork failed
    {
        perror("Fork failed");
        exit(1);
    }
    int output = 3;
    int input;
    char myStringOutput[] = "This a test!";
    char myStringInput[50];

    if (pid == 0)
    { // Child process
        close(fd[READ]);
        write(fd[WRITE], &myStringOutput, (sizeof(char)*13));
        printf("Child wrote [%s]\n", myStringOutput);
        close(fd[WRITE]);
    }
    else
    {
        close(fd[WRITE]);
        read(fd[READ], &myStringInput, (sizeof(char)*13));
        printf("Parent received [%s] from child process\n", myStringInput);
        close(fd[READ]);
    }

    return 0;
}