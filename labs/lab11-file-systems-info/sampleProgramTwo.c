#include <stdio.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>

int main()
{
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat statBuf;
    dirPtr = opendir(".");
    while ((entryPtr = readdir(dirPtr))) {
        if (stat(entryPtr->d_name, &statBuf) < 0) {
            perror("Failed to get file status");
            exit(1);
        }
        printf("%-20s %10ld bytes\n", entryPtr->d_name, statBuf.st_size);
    }
    closedir(dirPtr);
    return 0;
}