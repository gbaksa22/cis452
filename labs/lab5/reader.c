#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

#define FOO 4096

// recommended by Bobeldyk
struct sharedData {
    char message[FOO];  
    int readerOneReady;
    int readerTwoReady;
    int newMessage;
};

int shmId;
struct sharedData *sharedMemoryPtr;

void handleSignal(int signal) {
    printf("\nReader shutting down gracefully...\n");
    sharedMemoryPtr->newMessage = 0;
    if (shmdt(sharedMemoryPtr) < 0) {
            perror("Unable to detach\n");
            exit(1);
    }
    exit(0);
}

int main() {
    key_t key = ftok("writer.c", 'W');
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    shmId = shmget(key, sizeof(struct sharedData), 0);
    if (shmId < 0) { //ChatGPT helped with error checking
        perror("Unable to get shared memory\n");
        exit(1);
    }

    sharedMemoryPtr = (struct sharedData *)shmat(shmId, NULL, 0);
    if (sharedMemoryPtr == (void *)-1) { //ChatGPT helped with error checking
        perror("Unable to attach\n");
        exit(1);
    }

    signal(SIGINT, handleSignal);

    if (sharedMemoryPtr->readerOneReady == 0) {
                sharedMemoryPtr->readerOneReady = 1;
            } else {
                sharedMemoryPtr->readerTwoReady = 1; 
            }
            sharedMemoryPtr->newMessage = 0; 

    while (1) {
        if (sharedMemoryPtr->newMessage) {
            if (strncmp(sharedMemoryPtr->message, "quit", 4) == 0) {
                raise(SIGINT);
            }
            printf("Reader received message: %s", sharedMemoryPtr->message);
            if (sharedMemoryPtr->readerOneReady == 0) {
                sharedMemoryPtr->readerOneReady = 1;
            } else {
                sharedMemoryPtr->readerTwoReady = 1; 
            }
            sharedMemoryPtr->newMessage = 0; 
        }
    }


    if (shmdt(sharedMemoryPtr) < 0) { //ChatGPT helped with error checking
        perror("Unable to detach\n");
        exit(1);
    }

    return 0;
}