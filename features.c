#include "features.h"
#include "game.h"
#include <stdio.h>

// State of the world based on selected features
int get_complex_state_id(void) {
    // Score Level
    // Discretize the score into levels to represent game progress.
    static int total_dots = -1;
    if (total_dots == -1) {
        total_dots = 0;
        for (int r = 0; r < MAP_ROWS; r++) {
            for (int c = 0; c < MAP_COLS; c++) {
                if (MAP_DEFINITION[r][c] == 0) {
                    total_dots++;
                }
            }
        }
    }

    int score_level = 0;
    if (total_dots > 0) {
        float score_ratio = (float)current_score / total_dots;
        if (score_ratio > 0.75f) score_level = 3;
        else if (score_ratio > 0.50f) score_level = 2;
        else if (score_ratio > 0.25f) score_level = 1;
    }

    // Ghosts positions
    // By binning the map into a grid, we represent each ghost's absolute
    // position with one of those values. This captures location
    int ghost_ids[4];
    const int GRID_DIVISIONS = 3;
    const int CELL_WIDTH = MAP_COLS / GRID_DIVISIONS;
    const int CELL_HEIGHT = MAP_ROWS / GRID_DIVISIONS;

    for(int i=0; i<4; i++) {
        int gx = ghosts[i].x;
        int gy = ghosts[i].y;

        // Determine the ghost's cell in the grid
        int grid_x = gx / CELL_WIDTH;
        int grid_y = gy / CELL_HEIGHT;

        // Clamp values to be safe, as integer division can result in an index equal to GRID_DIVISIONS
        if (grid_x >= GRID_DIVISIONS) grid_x = GRID_DIVISIONS - 1;
        if (grid_y >= GRID_DIVISIONS) grid_y = GRID_DIVISIONS - 1;

        // Combine grid coordinates into a single ID
        ghost_ids[i] = grid_y * GRID_DIVISIONS + grid_x;
    }

    // Valid Moves for Pacman
    int valid_mask = 0;
    // Using dx, dy from game.h to check next positions
    if (!is_wall(pacman.x + dx[RIGHT], pacman.y + dy[RIGHT])) valid_mask |= (1 << RIGHT);
    if (!is_wall(pacman.x + dx[UP],    pacman.y + dy[UP]))    valid_mask |= (1 << UP);
    if (!is_wall(pacman.x + dx[LEFT],  pacman.y + dy[LEFT]))  valid_mask |= (1 << LEFT);
    if (!is_wall(pacman.x + dx[DOWN],  pacman.y + dy[DOWN]))  valid_mask |= (1 << DOWN);

    // Combine all features into a single state index
    // The order and multipliers are crucial for uniqueness and range
    long idx = score_level;                      // 4 values
    // Combine ghost positions into the state ID
    for (int i = 0; i < 4; i++) {
        idx = (idx * 9) + ghost_ids[i];      // 9 values per ghost
    }
    idx = (idx * 16) + valid_mask;           // 16 values

    // Ensure index does not exceed the defined state space size
    if (idx >= COMPLEX_STATE_SPACE_SIZE) {
        // This is a safeguard. This block should ideally never be reached if the
        // state space size is calculated correctly. It prevents out-of-bounds access
        // to the Q-Table in case of a logic error in the state calculation.
        fprintf(stderr, "Warning: State ID %ld exceeds space size. Capping to %d.\n", idx, COMPLEX_STATE_SPACE_SIZE - 1);
        idx = COMPLEX_STATE_SPACE_SIZE - 1;
    }
    return (int)idx;
}