#include "rewards.h"
#include "game.h"

float get_standard_reward(const GameState *before, const GameState *after,
                          int action) {
  // 1. Terminal States
  if (after->level_cleared)
    return 1000.0f;
  if (after->caught_by_ghost)
    return -500.0f;

  // 2. Wall Hit Penalty
  if (before->pacman_x == after->pacman_x &&
      before->pacman_y == after->pacman_y) {
    return -15.0f;
  }

  float reward = -1.0f; // Step penalty

  // 3. Direct Event Rewards
  if (after->score > before->score)
    reward += 20.0f;
  if (after->ghosts_eaten > before->ghosts_eaten)
    reward += 200.0f;

  // 4. Relative Proximity Rewards
  // Distance to nearest dot
  int dist_before_dot = 999;
  int dist_after_dot = 999;
  for (int r = 0; r < MAP_ROWS; r++) {
    for (int c = 0; c < MAP_COLS; c++) {
      if (map[r][c] == TILE_DOT || map[r][c] == TILE_BIG_PILL) {
        int d_b = abs(r - before->pacman_y) + abs(c - before->pacman_x);
        int d_a = abs(r - after->pacman_y) + abs(c - after->pacman_x);
        if (d_b < dist_before_dot)
          dist_before_dot = d_b;
        if (d_a < dist_after_dot)
          dist_after_dot = d_a;
      }
    }
  }
  if (dist_after_dot < dist_before_dot)
    reward += 2.0f; // Closer to dot
  else if (dist_after_dot > dist_before_dot)
    reward -= 1.0f; // Further from dot

  // Distance to nearest ghost
  int dist_before_ghost = 999;
  int dist_after_ghost = 999;
  int g_idx = -1;
  for (int i = 0; i < 4; i++) {
    int d_b = abs(ghosts[i].x - before->pacman_x) +
              abs(ghosts[i].y - before->pacman_y);
    int d_a =
        abs(ghosts[i].x - after->pacman_x) + abs(ghosts[i].y - after->pacman_y);
    if (d_b < dist_before_ghost) {
      dist_before_ghost = d_b;
      g_idx = i;
    }
    if (d_a < dist_after_ghost)
      dist_after_ghost = d_a;
  }

  if (g_idx != -1) {
    bool frightened = ghosts[g_idx].is_frightened && !ghosts[g_idx].is_eaten;
    if (frightened) {
      if (dist_after_ghost < dist_before_ghost)
        reward += 5.0f; // Chase!
    } else if (dist_after_ghost < 6) {
      if (dist_after_ghost < dist_before_ghost)
        reward -= 15.0f; // Move away!
      else
        reward += 5.0f; // Good job, running away
    }
  }

  // 5. Directional Penalties
  if (action == (before->pacman_dir + 2) % 4) {
    reward -= 5.0f; // Stronger penalty for reversal
  }

  return reward;
}