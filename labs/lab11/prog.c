#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>

int main(int argc, char *argv[])
{
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat statBuf;
    char *directory;

    if (argc < 2) {
        directory = ".";
    } else {
        directory = argv[1];
    }

    dirPtr = opendir(directory);
    if (dirPtr == NULL) {
        perror("Failed to open directory");
        exit(1);
    }

    printf("%-18s %-10s %-10s %-20s\n", "Inode", "User ID", "Group ID", "Name");
    printf("--------------------------------------------------------------\n");

    while ((entryPtr = readdir(dirPtr))) {
        if (stat(entryPtr->d_name, &statBuf) < 0) {
            perror("Failed to get file status");
            exit(1);
        }
        printf("%-18lu %-10d %-10d %-20s\n", 
            statBuf.st_ino,
            statBuf.st_uid,
            statBuf.st_gid,
            entryPtr->d_name);
    }
    closedir(dirPtr);
    return 0;
}