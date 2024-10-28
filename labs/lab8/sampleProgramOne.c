#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 16
#define GROWTH_FACTOR 2

int main() {
    char *data1;
    size_t size = INITIAL_SIZE;
    size_t length = 0;

    data1 = malloc(size);
    if (data1 == NULL) {
        perror("Unable to allocate memory");
        return 1;
    }

    printf("Please input username: ");

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (length + 1 >= size) {
            size *= GROWTH_FACTOR;
            char *new_data = realloc(data1, size);
            if (new_data == NULL) {
                perror("Unable to reallocate memory");
                free(data1);
                return 1;
            }
            data1 = new_data;
        }

        data1[length++] = ch;
    }

    data1[length] = '\0';

    printf("You entered: [%s]\n", data1);
    free(data1);
    return 0;
}
