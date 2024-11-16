#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>


// resource indices used in the semaphore, chatgpt helped with this
#define MIXER 0
#define PANTRY 1
#define REFRIGERATOR 2
#define BOWL 3
#define SPOON 4
#define OVEN 5

// resource counts
#define NUM_MIXERS 2
#define NUM_PANTRIES 1
#define NUM_REFRIGERATORS 2
#define NUM_BOWLS 3
#define NUM_SPOONS 5
#define NUM_OVENS 1

struct sembuf p = { 0, -1, SEM_UNDO}; // wait
struct sembuf v = { 0, +1, SEM_UNDO}; // signal

int semID;

void use_resource(int semID, int resource_index, const char *resource_name, int baker_id) {
    p.sem_num = resource_index; // here is where the resource index is used
    if (semop(semID, &p, 1) < 0) {
        perror("semop wait failed");
        exit(1);
    }
    printf("Baker %d: Acquired %s\n", baker_id, resource_name);
}

void release_resource(int semID, int resource_index, const char *resource_name, int baker_id) {
    v.sem_num = resource_index;
    if (semop(semID, &v, 1) < 0) {
        perror("semop signal failed");
        exit(1);
    }
    printf("Baker %d: Released %s\n", baker_id, resource_name);
}

void *baker(void *arg) {
    int baker_id = *(int *)arg;
    free(arg);

    // implement baker recipe logic here
    return NULL;
}

int main() {
    int num_bakers;

    printf("Enter the number of bakers: ");
    scanf("%d", &num_bakers);

    pthread_t threads[num_bakers];

    // create semaphores 
    semID = semget(IPC_PRIVATE, 6, IPC_CREAT | 0666);
    if (semID < 0) {
        perror("Unable to create semaphores");
        exit(1);
    }

    // initialize created semaphore with above defined values
    semctl(semID, MIXER, SETVAL, NUM_MIXERS);
    semctl(semID, PANTRY, SETVAL, NUM_PANTRIES);
    semctl(semID, REFRIGERATOR, SETVAL, NUM_REFRIGERATORS);
    semctl(semID, BOWL, SETVAL, NUM_BOWLS);
    semctl(semID, SPOON, SETVAL, NUM_SPOONS);
    semctl(semID, OVEN, SETVAL, NUM_OVENS);

    // create baker threads
    for (int i = 0; i < num_bakers; i++) {
        int *baker_id = malloc(sizeof(int)); 
        if (!baker_id) {
            perror("Unable to allocate memory");
            exit(1);
        }
        *baker_id = i + 1;

        // set each thread to execute baker function
        if (pthread_create(&threads[i], NULL, baker, baker_id) != 0) {
            perror("Error creating thread");
            exit(1);
        }
    }

    for (int i = 0; i < num_bakers; i++) {
        pthread_join(threads[i], NULL);
    }

    if (semctl(semID, 0, IPC_RMID) < 0) {
        perror("Unable to deallocate semaphores");
        exit(1);
    }

    printf("All bakers have finished their recipes!\n");
    return 0;
}
