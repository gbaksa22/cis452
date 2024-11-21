#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>



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

// Define globally at the top of the file
const char *recipe_ingredients[5][7] = {
    {"Flour", "Sugar", "Milk", "Butter", NULL},               // Cookies
    {"Flour", "Sugar", "Baking Soda", "Salt", "Egg", "Milk", NULL}, // Pancakes
    {"Yeast", "Sugar", "Salt", NULL},                        // Pizza Dough
    {"Flour", "Sugar", "Salt", "Yeast", "Baking Soda", "Egg", NULL}, // Soft Pretzels
    {"Flour", "Sugar", "Salt", "Butter", "Egg", "Cinnamon", NULL}   // Cinnamon Rolls
};



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

int select_recipe(int completed_recipes[], int num_recipes, int baker_id) {
    srand(time(NULL) + baker_id); // Seed random generator uniquely
    int recipe_index;
    do {
        recipe_index = rand() % num_recipes; // Randomly select a recipe
    } while (completed_recipes[recipe_index]); // Ensure the recipe hasn't been completed

    completed_recipes[recipe_index] = 1; // Mark the recipe as completed
    return recipe_index;
}

void grab_ingredients(int semID, const char *needed_ingredients[], int num_ingredients, int baker_id) {
    int is_acquired[6] = {0}; // 1 if acquired, 0 otherwise
    int acquired_ingredients = 0;

    while (acquired_ingredients < num_ingredients) {
        int progress = 0; // Track if any ingredient was acquired in this iteration
        for (int i = 0; i < num_ingredients; i++) {
            if (is_acquired[i]) continue; // Skip already acquired ingredients

            const char *ingredient = needed_ingredients[i];
            if (is_in_refrigerator(ingredient)) {
                p.sem_num = REFRIGERATOR;
                if (semop(semID, &p, 1) == 0) { // Successfully acquired refrigerator
                    printf("Baker %d: Got %s from Refrigerator\n", baker_id, ingredient);
                    is_acquired[i] = 1;
                    acquired_ingredients++;
                    progress = 1;
                    release_resource(semID, REFRIGERATOR, "Refrigerator", baker_id);
                }
            } else if (is_in_pantry(ingredient)) {
                p.sem_num = PANTRY;
                if (semop(semID, &p, 1) == 0) { // Successfully acquired pantry
                    printf("Baker %d: Got %s from Pantry\n", baker_id, ingredient);
                    is_acquired[i] = 1;
                    acquired_ingredients++;
                    progress = 1;
                    release_resource(semID, PANTRY, "Pantry", baker_id);
                }
            }
        }

        if (!progress) {
            printf("Baker %d: Stuck waiting for ingredients\n", baker_id);
            break;
        }
    }
}

void mix_ingredients(int semID, int baker_id) {
    struct sembuf utensil_ops[3];

    // Prepare semaphore operations for acquiring resources
    utensil_ops[0].sem_num = BOWL;
    utensil_ops[0].sem_op = -1; // Wait for a bowl
    utensil_ops[0].sem_flg = SEM_UNDO;

    utensil_ops[1].sem_num = SPOON;
    utensil_ops[1].sem_op = -1; // Wait for a spoon
    utensil_ops[1].sem_flg = SEM_UNDO;

    utensil_ops[2].sem_num = MIXER;
    utensil_ops[2].sem_op = -1; // Wait for a mixer
    utensil_ops[2].sem_flg = SEM_UNDO;

    printf("Baker %d: Waiting for Bowl, Spoon, and Mixer\n", baker_id);

    // Attempt to acquire all utensils
    if (semop(semID, utensil_ops, 3) < 0) {
        perror("Error acquiring Bowl, Spoon, and Mixer");
        exit(1);
    }

    printf("Baker %d: Acquired Bowl, Spoon, and Mixer\n", baker_id);
    printf("Baker %d: Mixing ingredients\n", baker_id);

    // Release all 3 utensils
    utensil_ops[0].sem_op = 1; // Signal for a bowl
    utensil_ops[1].sem_op = 1; // Signal for a spoon
    utensil_ops[2].sem_op = 1; // Signal for a mixer

    if (semop(semID, utensil_ops, 3) < 0) {
        perror("Error releasing Bowl, Spoon, and Mixer");
        exit(1);
    }

    printf("Baker %d: Released Bowl, Spoon, and Mixer\n", baker_id);
}

void bake_ingredients(int semID, int baker_id) {
    // Acquire the oven
    use_resource(semID, OVEN, "Oven", baker_id);

    // Simulate baking in the oven
    printf("Baker %d: Baking ingredients in the Oven\n", baker_id);

    // Release the oven
    release_resource(semID, OVEN, "Oven", baker_id);

    printf("Baker %d: Finished baking!\n", baker_id);
}


void *baker(void *arg) {
    int baker_id = *(int *)arg;
    free(arg);

    const char *recipes[] = {"Cookies", "Pancakes", "Pizza Dough", "Soft Pretzels", "Cinnamon Rolls"};
    int completed_recipes[5] = {0}; // Tracks recipes completed by this baker
    const int num_recipes = 5;

    while (1) {
        // Select a recipe that hasn't been made yet
        int recipe_index = select_recipe(completed_recipes, num_recipes, baker_id);
        if (recipe_index < 0 || recipe_index >= num_recipes) {
            fprintf(stderr, "Baker %d: Invalid recipe index %d\n", baker_id, recipe_index);
            pthread_exit(NULL);
        }
        printf("Baker %d: Working on recipe %s\n", baker_id, recipes[recipe_index]);

        // Calculate the number of ingredients
        int num_ingredients = 0;
        while (recipe_ingredients[recipe_index][num_ingredients] != NULL) {
            num_ingredients++;
        }

        // Dynamically allocate memory for needed_ingredients
        const char **needed_ingredients = malloc(num_ingredients * sizeof(char *));
        if (!needed_ingredients) {
            perror("Unable to allocate memory for needed ingredients");
            pthread_exit(NULL);
        }

        // Populate needed_ingredients
        for (int i = 0; i < num_ingredients; i++) {
            needed_ingredients[i] = recipe_ingredients[recipe_index][i];
        }

        // Debug: Print ingredients
        printf("Baker %d: Ingredients for recipe %s:\n", baker_id, recipes[recipe_index]);
        for (int i = 0; i < num_ingredients; i++) {
            printf("  - %s\n", needed_ingredients[i]);
        }

        // Grab all necessary ingredients
        grab_ingredients(semID, needed_ingredients, num_ingredients, baker_id);

        // Mix ingredients
        mix_ingredients(semID, baker_id);

        // Bake ingredients
        bake_ingredients(semID, baker_id);

        // Free allocated memory
        free(needed_ingredients);

        // Check if all recipes are done
        int done = 1;
        for (int i = 0; i < num_recipes; i++) {
            if (!completed_recipes[i]) {
                done = 0;
                break;
            }
        }
        if (done) break; // Exit loop if all recipes are complete
    }

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
