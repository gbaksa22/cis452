#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{   
    char command;
    printf("Enter command:\n");
    scanf("%d", &command);
    if (argc < 3) {
        fputs("Usage: must supply a command\n", stderr);
        exit(1);
    }
    
    puts("Before the exec");
    if (execvp(argv[1], &argv[1]) < 0) {
        perror("exec failed");
        exit(1);
    }

    return 0;
}
