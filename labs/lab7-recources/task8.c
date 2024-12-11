#include <sys/resource.h>
#include <stdio.h>

int main() {
    struct rlimit limit;
    getrlimit(RLIMIT_FSIZE, &limit);
    printf("Max File Size: %lu\n", limit.rlim_max);
    return 0;
}