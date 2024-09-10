#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
 // use these variables
    pid_t pid, child;
    int status;
    if ((pid = fork()) < 0) {
        fprintf(stderr, "Fork failure: %s", strerror(errno));
        exit(1);
    }
    else if (pid == 0) {
        printf("I am child PID %ld\n", (long) getpid());
        /* insert an appropriate form of the exit() function here */
        exit(0);
    }
    else {
        /* insert an appropriate form of the wait() system call here */
        child = wait(&status);
        printf("I am parent PID %ld\n", (long)getpid());
        printf("Child PID %ld terminated with return status %d\n", (long)
        child, status);
    }
    return 0;
}


