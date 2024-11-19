# Project README

## Baker logic

### Random Recipe Selection
1. **Recipes Array**: Lists all available recipes.
2. **Random Recipe Selection**:
   - `srand(time(NULL) + baker_id)`: Seeds the random number generator uniquely for each baker.
   - `rand() % num_recipes`: Chooses a recipe index in the range `[0, num_recipes - 1]`.
3. **Print Statement**: Outputs which recipe the baker is working on.