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
int openThreads = 0;

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
        openThreads++;
        
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
    openThreads--;
    pthread_exit(NULL); //ChatGPT suggested using pthread_exit with NULL return value
}

void handleExit(int signum) {
    while (openThreads > 0) {
        printf("\nWaiting for %d threads to finish...\n", openThreads);
        sleep(1);
    }
    printf("\nReceived Ctrl + C. Total file requests: %d\n", totalRequests);
    exit(0);
}