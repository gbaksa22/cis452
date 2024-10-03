#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 16

int main(int argc, char *argv[])
{
    int status;
    long int i, loop, temp, *sharedMemoryPointer;
    int sharedMemoryID, semID;
    pid_t pid;

    struct sembuf p = { 0, -1, SEM_UNDO};
    struct sembuf v = { 0, +1, SEM_UNDO};

    loop = atoi(argv[1]);

    semID = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
    if (semID < 0) 
    {
        perror("Unable to obtain semaphore\n");
        exit(1);
    }

    semctl(semID, 0, SETVAL, 1);

    sharedMemoryID = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (sharedMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }

    sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);
    if (sharedMemoryPointer == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    sharedMemoryPointer[0] = 0;
    sharedMemoryPointer[1] = 1;

    pid = fork();
    if (pid < 0)
    {
        printf("Fork failed\n");
    }

    if (pid == 0)
    { // Child

        for (i = 0; i < loop; i++)
        {
            if (semop(semID, &p, 1) < 0) {
                perror("semop wait failed");
                exit(1);
            }
            // swap the contents of sharedMemoryPointer[0] and sharedMemoryPointer[1]
            temp = sharedMemoryPointer[0];
            sharedMemoryPointer[0] = sharedMemoryPointer[1];
            sharedMemoryPointer[1] = temp;

            if (semop(semID, &v, 1) < 0) {
                perror("semop signal failed");
                exit(1);
            }
        }
        if (shmdt(sharedMemoryPointer) < 0)
        {
            perror("Unable to detach\n");
            exit(1);
        }
        exit(0);
    }
    else
        for (i = 0; i < loop; i++)
        {
            // swap the contents of sharedMemoryPointer[1] and sharedMemoryPointer[0]
            temp = sharedMemoryPointer[1];
            sharedMemoryPointer[1] = sharedMemoryPointer[0];
            sharedMemoryPointer[0] = temp;
        }

    wait(&status);
    printf("Values: %li\t%li\n", sharedMemoryPointer[0], sharedMemoryPointer[1]);

    if (shmdt(sharedMemoryPointer) < 0)
    {
        perror("Unable to detach\n");
        exit(1);
    }
    if (shmctl(sharedMemoryID, IPC_RMID, 0) < 0)
    {
        perror("Unable to deallocate\n");
        exit(1);
    }

    semctl(semID, 0, IPC_RMID);

    return 0;
}