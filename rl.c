#include "rl.h"
#include "game.h" // To access game state variables like pacman, ghosts, map, etc.
#include <stdio.h>
#include <stdlib.h> // For calloc, rand, exit

// Dynamic Pointer (Heap)
float *Q_Table = NULL;

// RL Global Variables
int episode = 0;
float epsilon = EPSILON_START;
int max_score = 0; // Defined here

void init_q_table(long num_states, int num_actions) {
    printf("Allocating Q-Table...\n");
    Q_Table = (float*)calloc(num_states * num_actions, sizeof(float));
    if (Q_Table == NULL) {
        fprintf(stderr, "Failed to allocate Q_Table!\n");
        exit(1);
    }
}

void free_q_table() {
    if (Q_Table != NULL) {
        free(Q_Table);
        Q_Table = NULL;
    }
}

int choose_action(int state, int num_actions) {
    // Epsilon-greedy policy
    if ((double)rand() / RAND_MAX < epsilon) {
        return rand() % num_actions; // Explore: choose a random action
    }

    // Exploit: choose the action with the highest Q-value
    int best_action = 0;
    float max_val = -999999.0f; // Initialize with a very small number
    for(int a=0; a<num_actions; a++) {
        float val = Q_Table[(long)state * num_actions + a];
        if (val > max_val) {
            max_val = val;
            best_action = a;
        }
    }
    return best_action;
}

void update_q_table(int state, int action, float reward, int next_state, int num_actions) {
    float max_next_q = -999999.0f;
    for(int a=0; a<num_actions; a++) {
        float val = Q_Table[(long)next_state * num_actions + a];
        if (val > max_next_q) {
            max_next_q = val;
        }
    }

    long q_idx = (long)state * num_actions + action;
    // Q-learning update formula
    Q_Table[q_idx] = Q_Table[q_idx] + ALPHA * (reward + GAMMA * max_next_q - Q_Table[q_idx]);
}