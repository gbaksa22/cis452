#include <time.h>
#include <stdio.h>

int main() {
    struct timespec res;
    clock_getres(CLOCK_MONOTONIC, &res);
    double milliseconds = res.tv_sec * 1000 + res.tv_nsec / 1e6;
    printf("Clock Resolution: %f milliseconds\n", milliseconds);
    return 0;
}