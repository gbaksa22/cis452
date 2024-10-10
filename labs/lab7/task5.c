#include <stdio.h>
#include <unistd.h>

int main() {
    long page_size;
    
    page_size = sysconf(_SC_PAGESIZE);

    if (page_size == -1) {
        perror("sysconf failed");
        return 1;
    }

    printf("Page size: %ld bytes\n", page_size);

    return 0;
}
