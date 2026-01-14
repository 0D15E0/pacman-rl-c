#include "features.h"
#include "game.h"
#include <stdio.h>

// State of the world based on selected features
int get_complex_state_id(void) {
  // 1. Nearest Dot Direction
  int nearest_dot_dist = 9999;
  int dot_dir = 0; // Default R

  for (int r = 0; r < MAP_ROWS; r++) {
    for (int c = 0; c < MAP_COLS; c++) {
      if (map[r][c] == TILE_DOT || map[r][c] == TILE_BIG_PILL) {
        int dist = abs(r - pacman.y) + abs(c - pacman.x);
        if (dist < nearest_dot_dist) {
          nearest_dot_dist = dist;
          if (abs(r - pacman.y) > abs(c - pacman.x)) {
            dot_dir = (r > pacman.y) ? DOWN : UP;
          } else {
            dot_dir = (c > pacman.x) ? RIGHT : LEFT;
          }
        }
      }
    }
  }

  // 2. Nearest Ghost Proximity
  int nearest_ghost_dist = 9999;
  int ghost_idx = -1;
  for (int i = 0; i < 4; i++) {
    int dist = abs(ghosts[i].x - pacman.x) + abs(ghosts[i].y - pacman.y);
    if (dist < nearest_ghost_dist) {
      nearest_ghost_dist = dist;
      ghost_idx = i;
    }
  }

  int g_dist_level = 2; // Far (6+)
  if (nearest_ghost_dist <= 2)
    g_dist_level = 0; // Near
  else if (nearest_ghost_dist <= 5)
    g_dist_level = 1; // Medium

  int g_dir = 0;
  bool g_frightened = false;
  if (ghost_idx != -1) {
    g_frightened =
        ghosts[ghost_idx].is_frightened && !ghosts[ghost_idx].is_eaten;
    if (abs(ghosts[ghost_idx].y - pacman.y) >
        abs(ghosts[ghost_idx].x - pacman.x)) {
      g_dir = (ghosts[ghost_idx].y > pacman.y) ? DOWN : UP;
    } else {
      g_dir = (ghosts[ghost_idx].x > pacman.x) ? RIGHT : LEFT;
    }
  }

  // 3. Valid Moves Mask
  int valid_mask = 0;
  if (!is_wall(pacman.x + dx[RIGHT], pacman.y + dy[RIGHT]))
    valid_mask |= (1 << RIGHT);
  if (!is_wall(pacman.x + dx[UP], pacman.y + dy[UP]))
    valid_mask |= (1 << UP);
  if (!is_wall(pacman.x + dx[LEFT], pacman.y + dy[LEFT]))
    valid_mask |= (1 << LEFT);
  if (!is_wall(pacman.x + dx[DOWN], pacman.y + dy[DOWN]))
    valid_mask |= (1 << DOWN);

  // Combine into a compact state ID
  // Range: 4 (dot_dir) * 3 (g_dist) * 4 (g_dir) * 2 (frightened) * 16 (mask)
  int idx = dot_dir;
  idx = (idx * 3) + g_dist_level;
  idx = (idx * 4) + g_dir;
  idx = (idx * 2) + (g_frightened ? 1 : 0);
  idx = (idx * 16) + valid_mask;

  if (idx >= COMPLEX_STATE_SPACE_SIZE) {
    printf("State ID overflow! idx: %d\n", idx);
    idx = COMPLEX_STATE_SPACE_SIZE - 1;
  }
  return idx;
}