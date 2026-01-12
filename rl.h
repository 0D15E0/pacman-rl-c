#ifndef RL_H
#define RL_H

#include <stdbool.h> // For bool types if needed

// RL PARAMETERS
#define ALPHA 0.1
#define GAMMA 0.9
#define EPSILON_START 1.0
#define EPSILON_MIN 0.05
#define EPSILON_DECAY 0.9999


// Dynamic Pointer (Heap)
extern float *Q_Table;

// RL Global Variables
extern int episode;
extern float epsilon;
extern int max_score; // Moved from game.h

// --- RL FUNCTION PROTOTYPES ---
void init_q_table(long num_states, int num_actions);
void free_q_table();
int choose_action(int state, int num_actions);
void update_q_table(int state, int action, float reward, int next_state, int num_actions);

#endif // RL_H