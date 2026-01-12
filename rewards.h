#ifndef REWARDS_H
#define REWARDS_H

#include "game.h" // For GameState struct
#include <stdbool.h>

/**
 * @brief Calculates a standard reward based on game events.
 *
 * This function provides a reward/penalty based on the outcome of an action.
 * It encourages eating dots and clearing quadrants, while penalizing
 * hitting walls, getting caught, and making inefficient moves (like reversing).
 *
 * @param before The state of the game before the action was taken.
 * @param after The state of the game after the action was taken.
 * @param action The action Pac-Man just took.
 * @return A float representing the calculated reward.
 */
float get_standard_reward(const GameState* before, const GameState* after, int action);

// To add a new reward function, you would:
// 1. Declare its function prototype here.
// 2. Implement it in rewards.c.
// 3. In main.c, pass the new function to ml_agent_init.

#endif // REWARDS_H