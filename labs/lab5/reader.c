#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>

#define FOO 4096

// recommended by Bobeldyk
struct sharedData {
    char message[FOO];  
    bool readerOneReady;
    bool readerTwoReady;
    bool newMessage;
};

int shmId;
struct sharedData *sharedMemoryPtr;

void handleSignal(int signal) {
    shmdt(sharedMemoryPtr);
    exit(0);
}

int main() {
    int shmId;
    struct sharedData *sharedMemoryPtr;

    key_t key = ftok("writer.c", 'W');

    shmId = shmget(key, sizeof(struct sharedData), 0);

    sharedMemoryPtr = (struct sharedData *)shmat(shmId, NULL, 0);

    while (1) {
        if (sharedMemoryPtr->newMessage) {
            printf("Reader received message: %s", sharedMemoryPtr->message);
            if (sharedMemoryPtr->readerOneReady == false) {
                sharedMemoryPtr->readerOneReady = true;
            } else {
                sharedMemoryPtr->readerTwoReady = true; 
            }
            sharedMemoryPtr->newMessage = false; 
        }
    }


    shmdt(sharedMemoryPtr);

    return 0;
}