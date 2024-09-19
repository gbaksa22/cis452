#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int totalRequests = 0;

int main() {
    char fileName[100];

    printf("Enter filenames (Ctrl + C to exit):\n");
    
    while (1) {
        scanf("%s", fileName);
        char* fileArg = malloc(strlen(fileName) + 1); 
        strcpy(fileArg, fileName);
        printf("testing input: %s\n", fileArg);
        free(fileArg);
    }
    /* 
    • Input a string from the user (simulating the name of a file to access) 
    • Spawn a child thread and communicate to it the filename 
    requested/entered by the user 
    • Immediately repeat the input/spawn sequence (i.e., accept a new file 
    request) 
    • Ensure your dispatcher is capable of properly handling rapidly entered 
    filenames (i.e., the user rapidly entering the filenames in succession)*/
}

// void *workerThread(void *arg) {
//     /*
//     • Obtain the simulated filename from the dispatcher 
//     • Sleep for a certain amount of time, simulating the time spent performing a 
//     file access: 
//         o with 80% probability, sleep for 1 second.  This simulates the 
//         scenario that the Worker thread has found the desired file in the 
//         disk cache and serves it up quickly. 
//         o with 20% probability, sleep for 7-10 seconds (randomly).  This 
//         simulates the scenario that the worker thread has not found the 
//         requested file in the disk cache and must block while it is read in 
//         from the hard drive. 
//     • Wake up, print a diagnostic message that includes the name of the file 
//     accessed, then terminate
//     */
// }

void handleExit(int signum) {
    printf("\nReceived Ctrl + C. Total file requests: %d\n", totalRequests);
    exit(0);
    /*
    Your program should continue to execute until terminated by the user (^C).  At 
    that point, your program should print out basic statistics: 
        • Total number of file requests received 
    When terminated, your program should cleanup as appropriate (i.e., free 
    resources) and shutdown gracefully. If there are any executing ‘file requests’ 
    those should complete before the program exits.
    */
}