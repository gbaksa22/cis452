#include <Windows.h>
#include <stdio.h>

int main() {
    // 1. Query the system for page size
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("Page size: %lu bytes\n", sysInfo.dwPageSize);

    // 2. Allocate 1 MB (2^20 bytes) of memory
    SIZE_T memorySize = 1 << 20; // 1 MB
    LPVOID allocatedMemory = VirtualAlloc(NULL, memorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (allocatedMemory == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // 3. Query the system to determine the state of the allocated memory
    MEMORY_BASIC_INFORMATION memInfo;
    VirtualQuery(allocatedMemory, &memInfo, sizeof(memInfo));

    // 4. Report the memory state in human-friendly terms
    printf("Memory State after allocation: ");
    if (memInfo.State == MEM_COMMIT) {
        printf("Committed\n");
    } else if (memInfo.State == MEM_RESERVE) {
        printf("Reserved\n");
    } else if (memInfo.State == MEM_FREE) {
        printf("Free\n");
    } else {
        printf("Unknown state\n");
    }

    // 5. De-allocate the memory
    VirtualFree(allocatedMemory, 0, MEM_RELEASE);

    // 6. Re-query and report the memory state after deallocation
    VirtualQuery(allocatedMemory, &memInfo, sizeof(memInfo));
    printf("Memory State after deallocation: ");
    if (memInfo.State == MEM_COMMIT) {
        printf("Committed\n");
    } else if (memInfo.State == MEM_RESERVE) {
        printf("Reserved\n");
    } else if (memInfo.State == MEM_FREE) {
        printf("Free\n");
    } else {
        printf("Unknown state\n");
    }

    return 0;
}
