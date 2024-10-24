#include <stdio.h>
#include <stdlib.h>

// Global variables
int global_var_initialized = 10;  // Initialized data
int global_var_initialized2 = 20; // Initialized data
int global_var_uninitialized;     // Uninitialized data (BSS)
int global_var_uninitialized2;    // Uninitialized data (BSS)

// Function to represent the text/code segment
void print_memory_addresses() {
    // Stack variables
    int stack_var1 = 1;
    int stack_var2 = 2;

    // Dynamically allocated memory (Heap)
    char *heap_var1 = malloc(sizeof(char) * 10);  // Heap variable 1
    char *heap_var2 = malloc(sizeof(char) * 10);  // Heap variable 2
    
    // Printing addresses of the memory segments

    printf("Stack - Address of stack_var1: %p\n", (void*)&stack_var1);
    printf("Stack - Address of stack_var2: %p\n\n", (void*)&stack_var2);

    printf("Heap - Address of heap_var1: %p\n", (void*)heap_var1);
    printf("Heap - Address of heap_var2: %p\n\n", (void*)heap_var2);

    printf("Data - Address of global_var_initialized: %p\n", (void*)&global_var_initialized);
    printf("Data - Address of global_var_initialized2: %p\n\n", (void*)&global_var_initialized2);

    printf("BSS - Address of global_var_uninitialized: %p\n", (void*)&global_var_uninitialized);
    printf("BSS - Address of global_var_uninitialized2: %p\n\n", (void*)&global_var_uninitialized2);

    printf("Text - Address of print_memory_addresses: %p\n\n", (void*)&print_memory_addresses);
    // Freeing dynamically allocated memory
    free(heap_var1);
    free(heap_var2);
}

int main() {
    print_memory_addresses();
    return 0;
}
