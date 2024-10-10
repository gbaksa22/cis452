#include <stdio.h>
#include <semaphore.h>
#include <errno.h>

int main() {
    sem_t sem;
    long maxValue = 0;

    while (1) {
        if (sem_init(&sem, 0, maxValue) != 0) {
            printf("Max counting semaphore value: %ld\n", maxValue);
            break;
        }
        maxValue++;
    }
    return 0;
}
