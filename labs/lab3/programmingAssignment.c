#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>


void signal_handler(int sigNum)
{
    if (sigNum == SIGUSR1) {
        printf("received a SIGUSR1 signal\n");
    } else if (sigNum == SIGUSR2) {
        printf("received a SIGUSR2 signal\n");
    }

    // Reinstall signal handler
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
}

void sigint_handler(int sig) {
    printf(" received.  That's it, I'm shutting you down...\n");
    fflush(stdout);  // Flush the output buffer - asked ChatGPT because of issues with ^C formatting
    exit(0);
}

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

        while (1) { //needed help from ChatGPT for the random parts
            int random_time = (rand() % 5) + 1;  // Random wait between 1 and 5 seconds
            sleep(random_time);

            // Randomly send SIGUSR1 or SIGUSR2 to parent
            int signal_to_send = (rand() % 2) ? SIGUSR1 : SIGUSR2;
            kill(getppid(), signal_to_send);
        }
        return 0;
    }
    else
    { // Parent process
        printf("Spawned child PID# %d\n", pid);
        while (1) {
            printf("waiting...\t");
            signal(SIGUSR1, signal_handler);
            signal(SIGUSR2, signal_handler);
            signal(SIGINT, sigint_handler);
            fflush(stdout);  // Flush the output buffer - asked ChatGPT because of issues with ^C formatting
            pause();  // Wait for a signal
        }
    } 
    return 0;  
}