#ifndef FEATURES_H
#define FEATURES_H

// The number of unique states produced by the get_complex_state_id function.
// This is crucial for allocating the correct size for the Q-Table.
// New size: 4 (score) * 9^4 (ghosts) * 16 (valid moves) * 16 (frightened mask)
// = 6,718,464
#define COMPLEX_STATE_SPACE_SIZE 6720000

/**
 * @brief Calculates a complex state ID based on multiple game features.
 *
 * This function generates a state representation by combining information
 * about:
 * 1. The current score, binned into levels.
 * 2. The position of each ghost, binned into a 3x3 grid.
 * 3. The valid moves available to Pac-Man from the current position.
 *
 * @return An integer representing the calculated state ID.
 */
int get_complex_state_id(void);

// To add a new state representation function, you would:
// 1. Define its state space size (e.g., #define SIMPLE_STATE_SPACE_SIZE 1000).
// 2. Declare its function prototype here (e.g., int
// get_simple_state_id(void);).
// 3. Implement it in features.c.
// 4. In main.c, pass the new function and its size to ml_agent_init.

#endif // FEATURES_H