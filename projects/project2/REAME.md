# Project README

## Baker logic

### Random Recipe Selection
1. **Recipes Array**: Lists all available recipes.
2. **Random Recipe Selection**:
   - `srand(time(NULL) + baker_id)`: Seeds the random number generator uniquely for each baker.
   - `rand() % num_recipes`: Chooses a recipe index in the range `[0, num_recipes - 1]`.
3. **Print Statement**: Outputs which recipe the baker is working on.

### Pantry and Fridge Collection
1. **Collect Ingredients**:
   - We create an array of all ingredients the baker needs (`needed_ingredients`).
   - Use `is_acquired` to track which ingredients are already taken.
   
2. **Simultaneous Access**:
   - The loop alternates between checking the pantry and refrigerator.
   - The semaphore (`semop`) ensures that only one baker accesses a resource at a time.

3. **Non-Blocking Resource Use**:
   - If the pantry is busy, the baker can grab ingredients from the refrigerator (or vice versa).

4. **Release Resources**:
   - After accessing a resource, the baker releases the semaphore for others to use.
