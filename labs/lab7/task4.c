#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>

int main() {
    int shmid;
    size_t size = 1024 * 1024;
    size_t max_size = 0;

    while (1) {
        shmid = shmget(IPC_PRIVATE, size, IPC_CREAT);
        if (shmid == -1) {
            printf("Max shared memory segment size: %zu MB\n", size);
            break;
        }
        size += 1024 * 1024;
    }
    return 0;
}
