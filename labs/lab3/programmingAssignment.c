#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
void sigHandler(int);
int main()
{
    // create pipe between child and parent

    int pid = fork();

    if (pid < 0) // Fork failed
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    { // Child process
        // wait between 1 and 5 seconds
        // send one of two user signals to parent
        return 0;
    }
    else
    { // Parent process
        printf("Spawned child PID# %d\n", pid);
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigHandler);
        printf("waiting...");
        pause();
        return 0;
    }    
}

void sigHandler(int sigNum)
{
    // differentiate between user signals and SIGINT
    printf(" received a %d signal\n", sigNum);
}
