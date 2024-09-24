#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

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

    // Node 0
    Message messageToSend;
    getchar(); 
    printf("Enter a message: \n"); 
    fgets(messageToSend.message, sizeof(messageToSend.message), stdin);
    messageToSend.message[strcspn(messageToSend.message, "\n")] = 0; //ChatGPT helped remove newline char

    printf("Enter a node to send the message to: \n"); 
    scanf("%d", &messageToSend.targetNode);
    getchar();

    write(pipes[0][WRITE], &messageToSend, sizeof(messageToSend));

    read(pipes[k - 1][READ], &messageToSend, sizeof(messageToSend));
    printf("[First Node] has the apple. The apple has passed through the ring.\n");
    return 0;
}