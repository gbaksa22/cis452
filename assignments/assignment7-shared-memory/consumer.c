#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>
//Create a struct, named ‘myStruct’, that contains both an integer and a character string (max length 40). 
typedef struct myStruct {
    int value;
    char message[40];
} myStruct;

int sharedMemoryID;
struct myStruct *sharedMemoryPointer;
int main() {
    //Generate a shared key using the ‘ftok’ command by referencing ‘producer.c’ as the filename. 
    key_t key = ftok("producer.c", 'W');

    // Create a shared memory segment using the ‘shmget’ command. 
    // The size of the shared memory segment should be the same size as the struct ‘myStruct’.
    // Use the following string for the permissions: “S_IWUSR|S_IRUSR|S_IRGRP|S_IWGRP|IPC_CREAT”.
    // You should store the ID (the return value) in the variable ‘sharedMemoryID’. 
    if ((sharedMemoryID = shmget(key, sizeof(struct myStruct), S_IWUSR|S_IRUSR|S_IRGRP|S_IWGRP|IPC_CREAT)) < 0) {
            // Print an appropriate error message and exit the program if the above step fails.
            perror("Unable to get shared memory"); 
            exit(1);
    }
    // Attach to the memory segment using the ‘shmat’ command. Use the shmid generated from the shmget command
    // Store the pointer in the variable ‘sharedMemoryPointer’. 
    if ((sharedMemoryPointer = shmat(sharedMemoryID, 0, 0)) == (void *)-1) {
            // Print an appropriate error message and exit the program if the above step fails.
            perror("Unable to attach");
            exit(1);
    }
    // Display both the integer value and the character string 
    printf("Conusumer received message: %s\n", sharedMemoryPointer->message);
    printf("Conusumer received value: %d\n", sharedMemoryPointer->value);

    //Detach from the shared memory segment using the ‘shmdt’ command. 
    if (shmdt(sharedMemoryPointer) < 0) {
        perror("Unable to detach\n");
        exit(1);
    }

    // Destroy the shared memory segment using ‘shmctl’ command with ‘IPC_RMID’ as the second option. 
    if (shmctl(sharedMemoryID, IPC_RMID, 0) < 0) {
        perror("Unable to deallocate\n");
        exit(1);
    }

    return 0;
}

/*
• Compile and run your program. 
• Try running the reader a second time, what is output and why? (include your answer in 
the submission text) 
• Run the ipcs command, make a note of the shared memory segments.*/
