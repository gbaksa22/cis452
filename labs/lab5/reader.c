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

    //ftok

    //shmget

    //attach

    //loop

    //detach

    return 0;
}