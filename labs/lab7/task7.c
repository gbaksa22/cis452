#include <stdio.h>
#include <sys/resource.h>
#include <errno.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NPROC, &limit) == -1) {
        perror("getrlimit failed");
        return 1;
    }

    printf("Soft limit (current): %ld\n", limit.rlim_cur);
    printf("Hard limit (maximum): %ld\n", limit.rlim_max);

    return 0;
}
