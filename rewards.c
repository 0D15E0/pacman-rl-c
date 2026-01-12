#include "rewards.h"
#include "game.h" 

float get_standard_reward(const GameState* before, const GameState* after, int action) {
    // Large bonus for clearing the level
    if (after->level_cleared) {
        return 500.0f;
    }

    // Large penalty for getting caught
    if (after->caught_by_ghost) {
        return -200.0f;
    }

    // Check for wall hit: Pac-Man's position didn't change.
    if (before->pacman_x == after->pacman_x && before->pacman_y == after->pacman_y) {
        return -10.0f; // Just the wall hit penalty.
    }

    float reward = -0.5f; // Base penalty for taking a step (encourages efficiency)

    // Reward for eating a dot
    // The most reliable way to check this is to see if the score increased.
    if (after->score > before->score) {
        reward += 10.0f;
    }

    // Penalty for trying to reverse direction immediately (discourages oscillation)
    if (action == (before->pacman_dir + 2) % 4) {
         reward -= 2.0f; // Small penalty, must be less than hitting a wall
    }

    return reward;
}