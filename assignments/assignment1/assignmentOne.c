#include <stdio.h>

int main() {
    printf("Hello, World!\n"); // prints Hello World

    int myIntVariable = 1; // creates myIntVariable

    // Correct the printf syntax by using a semicolon instead of a comma
    printf("myIntVariable value: %d\n", myIntVariable); // value of myIntVariable
    printf("myIntVariable address: %p\n", (void*)&myIntVariable); // address of myIntVariable

    int* myPointerToIntVariable = &myIntVariable; // creates pointer to int myIntVariable

    printf("myPointerToIntVariable value: %p\n", (void*)myPointerToIntVariable); // value of myPointerToIntVariable, the memory address of myIntVariable
    printf("myPointerToIntVariable address: %p\n", (void*)&myPointerToIntVariable); // address of myPointerToIntVariable
    printf("myPointerToIntVariable points to: %d\n", *myPointerToIntVariable); // value of myIntVariable that myPointerToIntVariable points to
    
    return 0;
}
