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
    sharedMemoryPtr->readerOneReady = false;
    sharedMemoryPtr->readerTwoReady = false;
    sharedMemoryPtr->newMessage = false;

    if (shmdt(sharedMemoryPtr) < 0) {
            perror("Unable to detach\n");
            exit(1);
    }
    
    if (shmctl(shmId, IPC_RMID, 0) < 0) {
        perror("Unable to deallocate\n");
        exit(1);
    }
    exit(0);
}

int main()
{
    // must use the same pathname as argument to get the same key in both files - double checked with ChatGPT
    key_t key = ftok("writer.c", 'W'); 
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    if ((shmId = shmget(IPC_PRIVATE, FOO, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0)
    {
        perror("Unable to get shared memory\n");
        exit(1);
    }

    if ((sharedMemoryPtr = shmat(shmId, 0, 0)) == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    sharedMemoryPtr->readerOneReady = false;
    sharedMemoryPtr->readerTwoReady = false;
    sharedMemoryPtr->newMessage = false;

    signal(SIGINT, handleSignal);

    while ( 1 ) {
        // do nothing while readers are reading message
        while ( sharedMemoryPtr->reader1 == 0 || sharedMemoryPtr->reader2 == 0 ) {}

        printf("Enter a string: ");
        fgets(sharedMemoryPtr->message, FOO, stdin);

        sharedMemoryPtr->newMessage = true;
        sharedMemoryPtr->readerOneReady = false;
        sharedMemoryPtr->readerTwoReady = false;
    }

    if (shmdt(sharedMemoryPtr) < 0)
        {
            perror("Unable to detach\n");
            exit(1);
        }

    if (shmctl(shmId, IPC_RMID, 0) < 0)
    {
        perror("Unable to deallocate\n");
        exit(1);
    }

    return 0;
}