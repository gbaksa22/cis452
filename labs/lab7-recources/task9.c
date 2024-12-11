#include <stdio.h>
#include <sys/resource.h>
#include <errno.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit failed");
        return 1;
    }

    printf("Hard limit (maximum number of open files): %ld\n", limit.rlim_max);

    return 0;
}
