#include <stdio.h>
#include <semaphore.h>
#include <errno.h>

int main() {
    sem_t sem;
    int max_value = 0;

    while (1) {
        if (sem_init(&sem, 0, max_value) != 0) {
            printf("Max counting semaphore value: %d\n", max_value);
            break;
        }
        max_value++;
    }
    return 0;
}