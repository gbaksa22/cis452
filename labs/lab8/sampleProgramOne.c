#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main() 
{
    size_t current_size = SIZE;
    char *data1 = malloc(current_size);

    printf("Please input username: ");

    while (fgets(data1, current_size, stdin)) {
        size_t len = strlen(data1);
        if (len > 0 && data1[len - 1] == '\n') {
            data1[len - 1] = '\0';
            break;
        }

        current_size *= 2;
        char *temp = realloc(data1, current_size);
        data1 = temp;
        printf("Input too long, please input username (again): ");
    }

    printf("You entered: [%s]\n", data1);
    free(data1);
    return 0;
}
