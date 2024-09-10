#include <stdio.h>
#include <unistd.h>  // For fork(), getpid()
#include <stdlib.h>  // For exit()
#include <sys/wait.h>  // For wait()

int main() {
    pid_t childProcessID;

    // Creates a new process using fork
    childProcessID = fork();

    if (childProcessID < 0) {
        // if fork fails exit with an error message
        // error if childProcessID is -1
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (childProcessID == 0) {
        // This is the child process
        printf("[%d] - childProcessID with value of [%d]\n", getpid(), childProcessID);
        printf("Child process has completed running.\n");
    } else {
        // This is the parent process, wait for the child to complete
        wait(NULL);  // Wait for the child process to finish
        printf("[%d] - parent process\n", getpid());
        printf("Parent process is finished running.\n");
    }

    return 0;
}
