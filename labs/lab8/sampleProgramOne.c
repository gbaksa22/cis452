#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main() 
{
    size_t current_size = SIZE;
    char *data1 = malloc(current_size);

    if (data1 == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    printf("Please input username: ");

    while (fgets(data1, current_size, stdin)) {
        if (data1[strlen(data1) - 1] == '\n') {
            data1[strlen(data1) - 1] = '\0';
            break;
        } else {
            current_size *= 2;
            char *temp = realloc(data1, current_size);

            if (temp == NULL) {
                perror("Failed to reallocate memory");
                free(data1);
                return 1;
            }

            data1 = temp;
        }
    }

    printf("You entered: [%s]\n", data1);

    free(data1);
    return 0;
}
