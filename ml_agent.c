#include "ml_agent.h"
#include "rl.h"       // The underlying RL algorithm implementation
#include <stdio.h>    // For printf
#include <stdlib.h>   // For NULL

// --- STATIC (PRIVATE) VARIABLES ---

// Function pointers to the configured state and reward logic
static get_state_func_t s_get_state_func = NULL;
static get_reward_func_t s_get_reward_func = NULL;

// The number of actions, needed by the RL algorithm
static int s_num_actions = 0;


// --- PUBLIC API IMPLEMENTATION ---

void ml_agent_init(get_state_func_t state_func, get_reward_func_t reward_func, long num_states, int num_actions) {
    printf("Initializing ML Agent...\n");
    s_get_state_func = state_func;
    s_get_reward_func = reward_func;
    s_num_actions = num_actions;

    // Initialize the underlying Q-learning module with the specified parameters
    init_q_table(num_states, num_actions);
}

void ml_agent_cleanup(void) {
    printf("Cleaning up ML Agent...\n");
    free_q_table();
}

int ml_agent_get_current_state(void) {
    if (s_get_state_func) {
        return s_get_state_func();
    }
    return 0; // Default/error state
}

float ml_agent_get_reward(const GameState* before, const GameState* after, int action) {
    if (s_get_reward_func) {
        return s_get_reward_func(before, after, action);
    }
    return 0.0f; // Default/error reward
}

int ml_agent_get_next_action(int current_state) {
    // Delegate action selection to the RL module
    return choose_action(current_state, s_num_actions);
}

void ml_agent_update(int state, int action, int next_state, float reward) {
    // Delegate Q-Table update to the RL module
    update_q_table(state, action, reward, next_state, s_num_actions);
}

void ml_agent_end_episode(int score) {
    episode++;
    if (score > max_score) {
        max_score = score;
    }

    // Decay epsilon for epsilon-greedy strategy
    if (epsilon > EPSILON_MIN) {
        epsilon *= EPSILON_DECAY;
    }

    // Log progress periodically
    if (episode % 100 == 0) {
        printf("Ep: %d | Max Score: %d | Epsilon: %.3f\n", episode, max_score, epsilon);
    }
}