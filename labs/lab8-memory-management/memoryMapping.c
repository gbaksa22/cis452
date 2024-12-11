#include <stdio.h>
#include <stdlib.h>

int globalVarInitialized = 10;
int globalVarInitialized2 = 20;
int globalVarUninitialized;
int globalVarUninitialized2;

void recursiveStack(int depth) {
    int recursiveStackVar = depth;

    if (depth == 3) {
        printf("1st stack address: %p\n", (void*)&recursiveStackVar);
    } else if (depth == 2) {
        printf("2nd stack address: %p\n", (void*)&recursiveStackVar);
    } else if (depth == 1) {
        printf("3rd stack address: %p\n", (void*)&recursiveStackVar);
    } else {
        printf("%dth stack address: %p\n", 4 - depth, (void*)&recursiveStackVar);
    }

    if (depth > 0) {
        recursiveStack(depth - 1);
    }
}

void recursiveHeap(int depth) {
    char *heapVar = malloc(sizeof(char) * 10);

    if (depth == 3) {
        printf("1st heap address: %p\n", (void*)heapVar);
    } else if (depth == 2) {
        printf("2nd heap address: %p\n", (void*)heapVar);
    } else if (depth == 1) {
        printf("3rd heap address: %p\n", (void*)heapVar);
    } else {
        printf("%dth heap address: %p\n", 4 - depth, (void*)heapVar);
    }

    if (depth > 0) {
        recursiveHeap(depth - 1);
    }

    free(heapVar);
}

void printMemoryAddresses() {
    int stackVar1 = 1;
    int stackVar2 = 2;

    char *heapVar1 = malloc(sizeof(char) * 10);
    char *heapVar2 = malloc(sizeof(char) * 10);

    printf("Stack - Address of stackVar1: %p\n", (void*)&stackVar1);
    printf("Stack - Address of stackVar2: %p\n\n", (void*)&stackVar2);

    printf("Heap - Address of heapVar1: %p\n", (void*)heapVar1);
    printf("Heap - Address of heapVar2: %p\n\n", (void*)heapVar2);

    printf("Data/BSS - Address of globalVarInitialized: %p\n", (void*)&globalVarInitialized);
    printf("Data/BSS - Address of globalVarInitialized2: %p\n", (void*)&globalVarInitialized2);
    printf("Data/BSS - Address of globalVarUninitialized: %p\n", (void*)&globalVarUninitialized);
    printf("Data/BSS - Address of globalVarUninitialized2: %p\n\n", (void*)&globalVarUninitialized2);

    printf("Text - Address of printMemoryAddresses: %p\n\n", (void*)&printMemoryAddresses);

    free(heapVar1);
    free(heapVar2);

    printf("Recursion begins to observe stack and heap growth:\n");
    recursiveStack(3);
    recursiveHeap(3);
}

int main() {
    printMemoryAddresses();
    return 0;
}
