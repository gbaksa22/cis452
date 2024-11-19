#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>


// resource indices used in the semaphore, chatgpt helped with this
#define MIXER 0
#define PANTRY 1
#define REFRIGERATOR 2
#define BOWL 3
#define SPOON 4
#define OVEN 5

// resource counts
#define NUM_MIXERS 2
#define NUM_PANTRIES 1
#define NUM_REFRIGERATORS 2
#define NUM_BOWLS 3
#define NUM_SPOONS 5
#define NUM_OVENS 1

struct sembuf p = { 0, -1, SEM_UNDO}; // wait
struct sembuf v = { 0, +1, SEM_UNDO}; // signal

int semID;

void use_resource(int semID, int resource_index, const char *resource_name, int baker_id) {
    p.sem_num = resource_index; // here is where the resource index is used
    if (semop(semID, &p, 1) < 0) {
        perror("semop wait failed");
        exit(1);
    }
    printf("Baker %d: Acquired %s\n", baker_id, resource_name);
}

void release_resource(int semID, int resource_index, const char *resource_name, int baker_id) {
    v.sem_num = resource_index;
    if (semop(semID, &v, 1) < 0) {
        perror("semop signal failed");
        exit(1);
    }
    printf("Baker %d: Released %s\n", baker_id, resource_name);
}

// Helper function: Check if an ingredient is in the refrigerator
int is_in_refrigerator(const char *ingredient) {
    const char *refrigerator_items[] = {"Egg", "Milk", "Butter"};
    for (int i = 0; i < 3; i++) {
        if (strcmp(ingredient, refrigerator_items[i]) == 0) {
            return 1; // Found in refrigerator
        }
    }
    return 0; // Not in refrigerator
}

// Helper function: Check if an ingredient is in the pantry
int is_in_pantry(const char *ingredient) {
    const char *pantry_items[] = {"Flour", "Sugar", "Yeast", "Baking Soda", "Salt", "Cinnamon"};
    for (int i = 0; i < 6; i++) {
        if (strcmp(ingredient, pantry_items[i]) == 0) {
            return 1; // Found in pantry
        }
    }
    return 0; // Not in pantry
}

void *baker(void *arg) {
    int baker_id = *(int *)arg;
    free(arg);

    //current recipe working on chosen, so that bakers arent all working on the same ones
    const char *recipes[] = {"Cookies", "Pancakes", "Pizza Dough", "Soft Pretzels", "Cinnamon Rolls"};
    const int num_recipes = 5;

    // Randomly choose a recipe for this baker to work on
    srand(time(NULL) + baker_id); // Ensure different seeds for each thread
    int recipe_index = rand() % num_recipes; // Random recipe index

    printf("Baker %d: Working on recipe %s\n", baker_id, recipes[recipe_index]);

    //if access to refrigerator, grab ingredient you dont have for current recipe

    //if access to pantry, grab ingredient you dont have for current recipe

    int acquired_ingredients = 0;
    const char *needed_ingredients[6];
    int num_ingredients = 0;

    // Collect the list of ingredients for the current recipe
    for (int i = 0; recipe_ingredients[recipe_index][i] != NULL; i++) {
        needed_ingredients[num_ingredients++] = recipe_ingredients[recipe_index][i];
    }

    // Keep track of which ingredients have been acquired
    int is_acquired[6] = {0}; // 1 if acquired, 0 otherwise

    while (acquired_ingredients < num_ingredients) {
        int ingredient_acquired = 0; // Track if an ingredient was successfully acquired

        for (int i = 0; i < num_ingredients; i++) {
            if (is_acquired[i]) continue; // Skip already acquired ingredients

            const char *ingredient = needed_ingredients[i];
            if (is_in_refrigerator(ingredient)) {
                // Attempt to acquire refrigerator
                p.sem_num = REFRIGERATOR;
                if (semop(semID, &p, 1) == 0) { // Successfully acquired refrigerator
                    printf("Baker %d: Got %s from Refrigerator\n", baker_id, ingredient);
                    is_acquired[i] = 1;
                    acquired_ingredients++;
                    ingredient_acquired = 1;
                    release_resource(semID, REFRIGERATOR, "Refrigerator", baker_id);
                }
            } else if (is_in_pantry(ingredient)) {
                // Attempt to acquire pantry
                p.sem_num = PANTRY;
                if (semop(semID, &p, 1) == 0) { // Successfully acquired pantry
                    printf("Baker %d: Got %s from Pantry\n", baker_id, ingredient);
                    is_acquired[i] = 1;
                    acquired_ingredients++;
                    ingredient_acquired = 1;
                    release_resource(semID, PANTRY, "Pantry", baker_id);
                }
            }
        }

    // If no ingredient was acquired, loop continues immediately without waiting
}


    //bowl, spoon, mixer needed, only grab the 3 at the same time if all 3 are available that way we dont block someone

    //oven

    // implement baker recipe logic here
    return NULL;
}

int main() {
    int num_bakers;

    printf("Enter the number of bakers: ");
    scanf("%d", &num_bakers);

    pthread_t threads[num_bakers];

    // create semaphores 
    semID = semget(IPC_PRIVATE, 6, IPC_CREAT | 0666);
    if (semID < 0) {
        perror("Unable to create semaphores");
        exit(1);
    }

    // initialize created semaphore with above defined values
    semctl(semID, MIXER, SETVAL, NUM_MIXERS);
    semctl(semID, PANTRY, SETVAL, NUM_PANTRIES);
    semctl(semID, REFRIGERATOR, SETVAL, NUM_REFRIGERATORS);
    semctl(semID, BOWL, SETVAL, NUM_BOWLS);
    semctl(semID, SPOON, SETVAL, NUM_SPOONS);
    semctl(semID, OVEN, SETVAL, NUM_OVENS);

    // create baker threads
    for (int i = 0; i < num_bakers; i++) {
        int *baker_id = malloc(sizeof(int)); 
        if (!baker_id) {
            perror("Unable to allocate memory");
            exit(1);
        }
        *baker_id = i + 1;

        // set each thread to execute baker function
        if (pthread_create(&threads[i], NULL, baker, baker_id) != 0) {
            perror("Error creating thread");
            exit(1);
        }
    }

    for (int i = 0; i < num_bakers; i++) {
        pthread_join(threads[i], NULL);
    }

    if (semctl(semID, 0, IPC_RMID) < 0) {
        perror("Unable to deallocate semaphores");
        exit(1);
    }

    printf("All bakers have finished their recipes!\n");
    return 0;
}
