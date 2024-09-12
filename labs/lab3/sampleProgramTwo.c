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

    if (pid == 0)
    { // Child process
        write(fd[1], &output, sizeof(int));
        printf("Child wrote [%d]\n", output);
    }
    else
    {
        read(fd[0], &input, sizeof(int));
        printf("Parent received [%d] from child process\n", input);
    }

    return 0;
}
