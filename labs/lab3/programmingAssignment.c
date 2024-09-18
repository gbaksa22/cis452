#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
void sigHandler(int);
int main()
{
    int pid = fork();

    if (pid < 0) // Fork failed
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    { // Child process
        printf("Child\n");
    }
    else
    { // Parent process
        printf("Spawned child PID# %d\n", pid);
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigintHandler);
        printf("waiting...");
        pause();
        return 0;
    }    
}

// handles user signals from child process
void sigHandler(int sigNum)
{
    printf(" received a %s signal\n", sigNum)
}

// handles ^C interrupt
void sigintHandler(int sigNum)
{
    printf(" received. Shutting down...\n");
    exit(0);
}