#include <stdio.h>
#include <unistd.h>

int main () {
    long pageNum = sysconf(_SC_AVPHYS_PAGES);
    printf("Physical Pages in System: %ld\n", pageNum);
    return 0;
}