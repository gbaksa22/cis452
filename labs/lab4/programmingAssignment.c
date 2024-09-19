#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void* workerThread(void *arg);
void handleExit(int signum);
int totalRequests = 0;

int main() {
    signal(SIGINT, handleExit);
    pthread_t worker;
    char fileName[100];

    printf("Enter filenames (Ctrl + C to exit):\n");
    
    while (1) {
        scanf("%s", fileName);
        char* fileArg = malloc(strlen(fileName) + 1); 
        strcpy(fileArg, fileName);

        totalRequests++;
        pthread_create(&worker, NULL, workerThread, fileArg);
    }
    return 0;
}

void* workerThread(void* arg) {
    char* fileName = (char*) arg;
    printf("\nWorker started for file: %s\n", fileName);

    srand(time(NULL)); // Needed ChatGPT's help for random syntax
    int randomValue = rand() % 10;
    if (randomValue < 8) {
        sleep(1);  // Fast access
    } else {
        sleep(rand() % 4 + 7);  // Slow access (7-10 seconds)
    }
    printf("\n%s just woke up!\n", fileName);
    free(fileName);
    char *result = "Success";
    pthread_exit(result);
}

void handleExit(int signum) {
    printf("\nReceived Ctrl + C. Total file requests: %d\n", totalRequests);
    exit(0);
}