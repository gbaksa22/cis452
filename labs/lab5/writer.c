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
    bool reader1;
    bool reader2;
    bool new_message;
};

int shmId;
struct sharedData *sharedMemoryPtr;

int main()
{
    //create a unique key?
    //ftok


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

    // initialize flags
    sharedMemoryPtr->reader1_ready = false;
    sharedMemoryPtr->reader2_ready = false;
    sharedMemoryPtr->new_message = false;

    while ( 1 ) {
        // do nothing while readers are reading message
        while ( sharedMemoryPtr->reader1 == 0 || sharedMemoryPtr->reader2 == 0 ) {}

        printf("Enter a string: ");
        fgets(sharedMemoryPtr->message, FOO, stdin);

        printf("String in shared memory: %d", sharedMemoryPtr->reader1);

        sharedMemoryPtr->new_message = true;  // Indicate new message
        sharedMemoryPtr->reader1_ready = false;  // Indicate Reader 1 is not ready
        sharedMemoryPtr->reader2_ready = false;  // Indicate Reader 2 is not ready
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