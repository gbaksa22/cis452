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
    printf("\nWriter shutting down gracefully...\n");
    sharedMemoryPtr->newMessage = 1;
    strcpy(sharedMemoryPtr->message, "quit");
    while(sharedMemoryPtr->newMessage == 1) {}
    sharedMemoryPtr->readerOneReady = 0;
    sharedMemoryPtr->readerTwoReady = 0;
    sharedMemoryPtr->newMessage = 0;

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
    key_t key = ftok("./writer.c", 'W'); 
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    if ((shmId = shmget(key, sizeof(struct sharedData), IPC_CREAT | S_IRUSR | S_IWUSR)) < 0)
    {
        perror("Unable to get shared memory");
        exit(1);
    }

    if ((sharedMemoryPtr = shmat(shmId, 0, 0)) == (void *)-1)
    {
        perror("Unable to attach");
        exit(1);
    }

    sharedMemoryPtr->readerOneReady = 0;
    sharedMemoryPtr->readerTwoReady = 0;
    sharedMemoryPtr->newMessage = 0;

    signal(SIGINT, handleSignal);

    printf("\nWaiting for both readers...\n");
    while ( 1 ) {
        // do nothing while readers are reading message
        while ( sharedMemoryPtr->readerOneReady == 0 || sharedMemoryPtr->readerTwoReady == 0 ) {}

        printf("Enter a string: ");
        fgets(sharedMemoryPtr->message, FOO, stdin);

        if (strncmp(sharedMemoryPtr->message, "quit", 4) == 0) {
            raise(SIGINT); //asked ChatGPT how to use sigint handler with quit
        }

        sharedMemoryPtr->newMessage = 1;
        sharedMemoryPtr->readerOneReady = 0;
        sharedMemoryPtr->readerTwoReady = 0;
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