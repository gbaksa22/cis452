#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define READ 0
#define WRITE 1

int k;
int apple = 0;

typedef struct {
    int targetNode;
    char message[256];
} Message;

void sigint_handler(int sigNum) {
    printf(" received.  Gracefully exiting...\n");
    exit(0);
}

void node(int id, int read_fd, int write_fd) {
    Message receivedMessage;
    while (1) {
        read(read_fd, &receivedMessage, sizeof(receivedMessage));

        if (id == receivedMessage.targetNode) {
            printf("[Target Node %d] has the apple and recieved the message: \"%s\"\n", id, receivedMessage.message);
            strcpy(receivedMessage.message, "");
        } else {
            printf("[Node %d] has the apple and is passing on this message: \"%s\" for target node %d\n", id, receivedMessage.message, receivedMessage.targetNode);
        }

        write(write_fd, &receivedMessage, sizeof(receivedMessage));
    }
}

int main () {
    signal(SIGINT, sigint_handler);

    printf("Enter the number of nodes (k): "); 
    scanf("%d", &k);

    // create pipe for communication
    int pipes[k][2];
    int pipeCreationResult;
    for (int i = 0; i < k; i++) {
        pipeCreationResult = pipe(pipes[i]);
        // check for proper creation
        if (pipeCreationResult < 0)
        {
            perror("Failed pipe creation\n");
            exit(1);
        }
    }

    // create k child nodes
    int pid;
    for (int i = 0; i < k; i++) {
        pid = fork();
        // check for proper creation
        if (pid < 0)
        {
            perror("Fork failed\n");
            exit(1);
        }
        if (pid == 0) 
        {
            printf("[%d] - Node %d created\n", getpid(), i);
            int read_fd = pipes[i][READ];
            int write_fd = pipes[(i + 1) % k][WRITE];
            node(i, read_fd, write_fd);
            exit(0); // placeholder
        }
    }

    // logic for node 0

    getchar(); // clear buffer
    // prompt the user for a string message 
    // should be able to handle messages of more than one word
    printf("Enter a message: \n"); 
    fgets(messageToSend.message, sizeof(messageToSend.message), stdin);

    // prompt the user for the number of the node to send the message to
    printf("Enter a node to send the message to: \n"); 
    scanf("%d", &messageToSend.targetNode);
    getchar(); // clear buffer

    // send the message and node number to the first child node

    write(pipes[0][WRITE], &messageToSend, sizeof(messageToSend));

    printf("[Parent] Message sent. Waiting for the next message...\n");
    return 0;
}