#include <stdio.h>
#include <unistd.h>

int main() {
    long pageSize;

    pageSize = sysconf(_SC_PAGESIZE);

    if (page_size == -1) {
        perror("sysconf failed");
        return 1;
    }

    printf("Page size: %ld bytes\n", pageSize);

    return 0;
}
