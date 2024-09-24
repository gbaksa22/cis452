#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int k;
int apple = 0;

void sigint_handler(int sigNum) {
    printf(" received.  Gracefully exiting...\n");
    exit(0);
}

int main () {
    signal(SIGINT, sigint_handler);

    printf("Enter the number of nodes (k): "); 
    scanf("%d", &k);

    // create pipe for communication
    int fd = [k][2];
    int pipeCreationResult;
    // check for proper creation
    for (int i = 0; i < k; i++) {
        pipeCreationResult = pipe(fd[i]);
        if (pipeCreationResult < 0)
        {
            perror("Failed pipe creation\n");
            exit(1);
        }
    }

    // create k child nodes
    // check for proper creation

    // if else for parent and children ?

    // prompt the user for a string message 
    // should be able to handle messages of more than one word

    // prompt the user for the number of the node to send the message to
}