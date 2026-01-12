#ifndef ML_AGENT_H
#define ML_AGENT_H

#include "game.h" // For GameState struct
#include <stdbool.h>

// --- FUNCTION POINTERS FOR PLUGGABLE BEHAVIOR ---

/**
 * @brief A function pointer for getting the current state of the world.
 * @return An integer representing the current state.
 */
typedef int (*get_state_func_t)(void);

/**
 * @brief A function pointer for calculating the reward for an action.
 * @param before The state of the game before the action was taken.
 * @param after The state of the game after the action was taken.
 * @param action The action Pac-Man just took.
 * @return A float representing the calculated reward.
 */
typedef float (*get_reward_func_t)(const GameState* before, const GameState* after, int action);


// --- AGENT ABSTRACTION LAYER ---

/**
 * @brief Initializes the ML agent with specific behaviors and parameters.
 *
 * @param state_func The function to use for state representation.
 * @param reward_func The function to use for reward calculation.
 * @param num_states The total number of possible states for the given state_func.
 * @param num_actions The total number of actions the agent can take.
 */
void ml_agent_init(get_state_func_t state_func, get_reward_func_t reward_func, long num_states, int num_actions);

/**
 * @brief Cleans up resources used by the ML agent (e.g., the Q-Table).
 */
void ml_agent_cleanup(void);

/**
 * @brief Determines the agent's next action based on the current state.
 *
 * @param current_state The current state of the world.
 * @return The integer representing the action to take.
 */
int ml_agent_get_next_action(int current_state);

/**
 * @brief Updates the agent's knowledge (Q-Table) based on the last transition.
 *
 * @param state The state before the action.
 * @param action The action taken.
 * @param next_state The state after the action.
 * @param reward The reward received for the action.
 */
void ml_agent_update(int state, int action, int next_state, float reward);

/**
 * @brief Gets the current state using the configured state function.
 *
 * @return The current state ID.
 */
int ml_agent_get_current_state(void);

/**
 * @brief Gets the reward for a transition using the configured reward function.
 */
float ml_agent_get_reward(const GameState* before, const GameState* after, int action);

/**
 * @brief Handles logic at the end of an episode (e.g., epsilon decay, logging).
 *
 * @param score The final score for the completed episode.
 */
void ml_agent_end_episode(int score);

#endif // ML_AGENT_H