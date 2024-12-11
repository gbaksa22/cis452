#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAX_INPUT 1024  // Maximum input buffer size

int main() {
    char input[MAX_INPUT];  // Buffer for user input
    char *word_1;           // Pointer to hold the command
    char *args[100];        // Array to hold the arguments for execvp
    int i;

    while (1) {
        // Display prompt
        printf("simple_shell> ");
        
        // Read input using fgets
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("Error reading input");
            exit(1);
        }

        // Remove the newline character if present
        input[strcspn(input, "\n")] = 0;

        // If user enters "quit", break the loop and exit
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        // Tokenize the input using strtok and store arguments in args array
        word_1 = strtok(input, " ");
        i = 0;
        while (word_1 != NULL) {
            args[i++] = word_1;
            word_1 = strtok(NULL, " ");
        }
        args[i] = NULL;  // Last element must be NULL for execvp

        // Fork a new process
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            continue;
        }

        if (pid == 0) {
            if (execvp(args[0], args) < 0) {
                perror("Exec failed");
                exit(1);
            }
        } else { 
            ///ChatGPT helped with struct, and the printf
            int status;
            struct rusage usage; //ChatGPT helped with struct
            
            wait4(pid, &status, 0, &usage);
            
            printf("User CPU time used: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
            printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
        }
    }

    return 0;
}
