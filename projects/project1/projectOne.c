#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int k;

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
            strcpy(receivedMessage.message, "empty");
        } else if (strcmp(receivedMessage.message, "empty") == 0) {
            printf("[Node %d] has the apple and is passing an empty message\n", id);
            
        } else {
            printf("[Node %d] has the apple and is passing on this message: \"%s\" for target node %d\n", id, receivedMessage.message, receivedMessage.targetNode);
        }
        write(write_fd, &receivedMessage, sizeof(receivedMessage));
        if (id == (k-1)) {
            printf("[Node 0] has the apple again. The apple has passed through the ring.\n");
        }
    }
    close(read_fd);
    close(write_fd);
}

int main () {

    printf("Enter the number of nodes (k): "); 
    scanf("%d", &k);

    // create pipe for communication
    int pipes[k][2];
    int pipeCreationResult;
    for (int i = 0; i < k; i++) {
        pipeCreationResult = pipe(pipes[i]);
        if (pipeCreationResult < 0)
        {
            perror("Failed pipe creation\n");
            exit(1);
        }
    }
    printf("[%d] - Node 0 is the parent. It has already been created.\n", getpid());
    // change number of nodes being created
    int pid;
    for (int i = 1; i < k; i++) {
        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed\n");
            exit(1);
        }
        if (pid == 0) // creates child nodes
        {
            printf("[%d] - Node %d created.\n", getpid(), i);
            // change read write pipes
            int read_fd = pipes[i][READ];
            printf("Node %d writes to node %d. \n", i, ((i + 1) % k));
            int write_fd = pipes[(i + 1) % k][WRITE];
            node(i, read_fd, write_fd);
        }
    }
    // Node 0
    signal(SIGINT, sigint_handler);
    
    Message messageToSend;
    Message finalReceivedMessage;
    int write_fd = pipes[1][WRITE];
    int read_fd = pipes[0][READ];
    getchar(); 

    while (1) {
        printf("Enter a message: \n"); 
        fgets(messageToSend.message, sizeof(messageToSend.message), stdin);
        messageToSend.message[strcspn(messageToSend.message, "\n")] = 0; //ChatGPT helped remove newline char

        printf("Enter a node to send the message to: \n"); 
        scanf("%d", &messageToSend.targetNode);
        getchar();
        //printf("[Node 0] has the apple and is passing on this message: \"%s\" for target node %d\n", receivedMessage.message, receivedMessage.targetNode);
        // print out parent node has the apple
        //int write_fd = write(pipes[1][WRITE], &messageToSend, sizeof(messageToSend));

        //int read_fd = read(pipes[k - 1][READ], &messageToSend, sizeof(messageToSend));
        write(write_fd, &messageToSend, sizeof(messageToSend));
        printf("Node 0 is reading from node %d\n", (k-1));
        read(read_fd, &finalReceivedMessage, sizeof(finalReceivedMessage));

        if (strcmp(finalReceivedMessage.message, "empty") == 0) {
            printf("The message reached the target and the parent now has the apple.\n");
        } else {
            printf("The target was never reached, but the parent now has the apple again.\n");
        }
        //printf("[Node 0] has the apple again. The apple has passed through the ring.\n");
        
        char choice;
        printf("Do you want to send another message? (y/n): ");
        scanf("%c", &choice);
        getchar();

        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    close(write_fd);
    close(read_fd);
    return 0;
}