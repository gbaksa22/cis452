#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>

int main() {
    int shmid;
    size_t size = 1;
    size_t max_size = 0;

    while (1) {
        shmid = shmget(IPC_PRIVATE, size, IPC_CREAT);
        if (shmid == -1) {
            max_size = size / 2;
            break;
        }
        size *= 2;
    }

    for (size_t i = max_size; i < max_size * 2; i++) {
        shmid = shmget(IPC_PRIVATE, i, IPC_CREAT);
        if (shmid == -1) {
            printf("Max shared memory segment size: %zu bytes\n", i - 1);
            break;
        }
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
