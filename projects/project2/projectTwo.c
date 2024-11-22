#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

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

const char *colors[] = {
    "\033[0;31m", "\033[0;32m", "\033[0;33m", "\033[0;34m",
    "\033[0;35m", "\033[0;36m", "\033[1;31m", "\033[1;32m",
    "\033[1;33m", "\033[1;34m"
    };

typedef struct {
    int baker_id;
    const char *color;
    int ramsey_baker;
} BakerContext;

void use_resource(int semID, int resource_index, const char *resource_name, int baker_id, const char *color) {
    p.sem_num = resource_index; // Set the semaphore index
    if (semop(semID, &p, 1) < 0) {
        perror("semop wait failed");
        exit(1);
    }
    printf("%sBaker %d: Acquired %s\033[0m\n", color, baker_id, resource_name); // Use the color
}

void release_resource(int semID, int resource_index, const char *resource_name, int baker_id, const char *color) {
    v.sem_num = resource_index; // Set the semaphore index
    if (semop(semID, &v, 1) < 0) {
        perror("semop signal failed");
        exit(1);
    }
    printf("%sBaker %d: Released %s\033[0m\n", color, baker_id, resource_name); // Use the color
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

    return recipe_index;
}

void grab_ingredients(BakerContext *context, const char *needed_ingredients[], int num_ingredients) {
    int is_acquired[6] = {0};
    int acquired_ingredients = 0;

    while (acquired_ingredients < num_ingredients) {
        int progress = 0;
        for (int i = 0; i < num_ingredients; i++) {
            if (is_acquired[i]) continue;

            const char *ingredient = needed_ingredients[i];
            if (is_in_refrigerator(ingredient)) {
                p.sem_num = REFRIGERATOR;
                if (semop(semID, &p, 1) == 0) {
                    printf("%sBaker %d: Got %s from Refrigerator\033[0m\n", context->color, context->baker_id, ingredient);
                    is_acquired[i] = 1;
                    acquired_ingredients++;
                    release_resource(semID, REFRIGERATOR, "Refrigerator", context->baker_id, context->color);
                    progress = 1;
                }
            } else if (is_in_pantry(ingredient)) {
                p.sem_num = PANTRY;
                if (semop(semID, &p, 1) == 0) {
                    printf("%sBaker %d: Got %s from Pantry\033[0m\n", context->color, context->baker_id, ingredient);
                    is_acquired[i] = 1;
                    acquired_ingredients++;
                    release_resource(semID, PANTRY, "Pantry", context->baker_id, context->color);
                    progress = 1;
                }
            }
        }

        if (!progress) {
            printf("%sBaker %d: Stuck waiting for ingredients\033[0m\n", context->color, context->baker_id);
            break;
        }
    }
}

void mix_ingredients(BakerContext *context) {
    struct sembuf utensil_ops[3];

    utensil_ops[0].sem_num = BOWL;
    utensil_ops[0].sem_op = -1;
    utensil_ops[0].sem_flg = SEM_UNDO;

    utensil_ops[1].sem_num = SPOON;
    utensil_ops[1].sem_op = -1;
    utensil_ops[1].sem_flg = SEM_UNDO;

    utensil_ops[2].sem_num = MIXER;
    utensil_ops[2].sem_op = -1;
    utensil_ops[2].sem_flg = SEM_UNDO;

    printf("%sBaker %d: Waiting for Bowl, Spoon, and Mixer\033[0m\n", context->color, context->baker_id);

    if (semop(semID, utensil_ops, 3) < 0) {
        perror("Error acquiring utensils");
        exit(1);
    }

    printf("%sBaker %d: Acquired Bowl, Spoon, and Mixer\033[0m\n", context->color, context->baker_id);
    printf("%sBaker %d: Mixing ingredients\033[0m\n", context->color, context->baker_id);

    utensil_ops[0].sem_op = 1;
    utensil_ops[1].sem_op = 1;
    utensil_ops[2].sem_op = 1;

    if (semop(semID, utensil_ops, 3) < 0) {
        perror("Error releasing utensils");
        exit(1);
    }

    printf("%sBaker %d: Released Bowl, Spoon, and Mixer\033[0m\n", context->color, context->baker_id);
}

void bake_ingredients(BakerContext *context) {
    // Acquire the oven
    printf("%sBaker %d: Waiting for Oven\033[0m\n", context->color, context->baker_id);
    use_resource(semID, OVEN, "Oven", context->baker_id, context->color);

    // Simulate baking in the oven
    printf("%sBaker %d: Baking ingredients in the Oven\033[0m\n", context->color, context->baker_id);

    // Release the oven
    release_resource(semID, OVEN, "Oven", context->baker_id, context->color);
    printf("%sBaker %d: Finished baking!\033[0m\n", context->color, context->baker_id);
}

void ramsied(int baker_id, const char *color) {
    printf("%sBaker %d: Ramsied! Restarting recipe\033[0m\n", color, baker_id);

    release_resource(semID, MIXER, "Mixer", baker_id, color);
    release_resource(semID, PANTRY, "Pantry", baker_id, color);
    release_resource(semID, REFRIGERATOR, "Refrigerator", baker_id, color);
    release_resource(semID, BOWL, "Bowl", baker_id, color);
    release_resource(semID, SPOON, "Spoon", baker_id, color);
    release_resource(semID, OVEN, "Oven", baker_id, color);
}

void *baker(void *arg) {
    BakerContext *context = (BakerContext *)arg;
    int baker_id = context->baker_id;
    const char *color = context->color;
    int ramsey_baker = context->ramsey_baker;

    const char *recipes[] = {"Cookies", "Pancakes", "Pizza Dough", "Soft Pretzels", "Cinnamon Rolls"};
    int completed_recipes[5] = {0}; // Tracks recipes completed by this baker
    const int num_recipes = 5;

    bool ramsied_triggered = false; // ensures baker is only "Ramsied" once

    while (1) {
        int recipe_index = select_recipe(completed_recipes, num_recipes, baker_id);
        printf("%sBaker %d: Working on recipe %s\033[0m\n", color, baker_id, recipes[recipe_index]);

        int num_ingredients = 0;
        while (recipe_ingredients[recipe_index][num_ingredients] != NULL) {
            num_ingredients++;
        }

        const char **needed_ingredients = malloc(num_ingredients * sizeof(char *));
        if (!needed_ingredients) {
            perror("Unable to allocate memory for needed ingredients");
            pthread_exit(NULL);
        }

        for (int i = 0; i < num_ingredients; i++) {
            needed_ingredients[i] = recipe_ingredients[recipe_index][i];
        }

        while(1) {
            printf("%sBaker %d: Ingredients for recipe %s:\033[0m\n", color, baker_id, recipes[recipe_index]);
            for (int i = 0; i < num_ingredients; i++) {
                printf("%s  - %s\033[0m\n", color, needed_ingredients[i]);
            }

            grab_ingredients(context, needed_ingredients, num_ingredients);

            if (baker_id == ramsey_baker && !ramsied_triggered && rand() % 10 == 0) {
                ramsied(baker_id, color);
                ramsied_triggered = true;
                continue;
            }

            mix_ingredients(context);

            if (baker_id == ramsey_baker && !ramsied_triggered && rand() % 10 == 0) {
                ramsied(baker_id, color);
                ramsied_triggered = true;
                continue;
            }

            bake_ingredients(context);

            completed_recipes[recipe_index] = 1; // Mark the recipe as completed
            break;
        }

        free(needed_ingredients);

        int done = 1;
        for (int i = 0; i < num_recipes; i++) {
            if (!completed_recipes[i]) {
                done = 0;
                break;
            }
        }
        if (done) break;
    }

    free(context);
    return NULL;
}

int main() {
    int num_bakers;

    printf("Enter the number of bakers: ");
    scanf("%d", &num_bakers);

    pthread_t threads[num_bakers];

    // Create semaphores
    semID = semget(IPC_PRIVATE, 6, IPC_CREAT | 0666);
    if (semID < 0) {
        perror("Unable to create semaphores");
        exit(1);
    }

    // Initialize created semaphore with defined values
    semctl(semID, MIXER, SETVAL, NUM_MIXERS);
    semctl(semID, PANTRY, SETVAL, NUM_PANTRIES);
    semctl(semID, REFRIGERATOR, SETVAL, NUM_REFRIGERATORS);
    semctl(semID, BOWL, SETVAL, NUM_BOWLS);
    semctl(semID, SPOON, SETVAL, NUM_SPOONS);
    semctl(semID, OVEN, SETVAL, NUM_OVENS);

    // randomly selects a baker to be "Ramsied"
    int ramsey_baker = rand() % num_bakers;

    // Create baker threads
    for (int i = 0; i < num_bakers; i++) {
        BakerContext *context = malloc(sizeof(BakerContext)); // Dynamically allocate memory for BakerContext
        if (!context) {
            perror("Unable to allocate memory for baker context");
            exit(1);
        }
        context->baker_id = i + 1;
        context->color = colors[i % 10]; // Assign a unique color to each baker
        context->ramsey_baker = ramsey_baker;

        // Create the thread
        if (pthread_create(&threads[i], NULL, baker, context) != 0) {
            perror("Error creating thread");
            free(context); // Free memory in case of thread creation failure
            exit(1);
        }
    }

    // Join threads
    for (int i = 0; i < num_bakers; i++) {
        pthread_join(threads[i], NULL);
    }

    // Deallocate semaphores
    if (semctl(semID, 0, IPC_RMID) < 0) {
        perror("Unable to deallocate semaphores");
        exit(1);
    }

    printf("All bakers have finished their recipes!\n");
    return 0;
}
