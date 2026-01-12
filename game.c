#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

// --- GLOBAL GAME STATE VARIABLES ---
int map[MAP_ROWS][MAP_COLS];
Entity pacman;
Ghost ghosts[4];
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, -1, 0, 1};

int current_score = 0;
static int current_mode = MODE_SCATTER;
static int mode_timer = 0;

bool is_in_ghost_house(int x, int y) {
    return (x >= 10 && x <= 17 && y >= 13 && y <= 15);
}

bool is_wall(int c, int r) {
    if (r == 14 && (c < 0 || c >= MAP_COLS)) return false; // Tunnel
    if (c < 0 || c >= MAP_COLS || r < 0 || r >= MAP_ROWS) return true; // Out of bounds is a wall
    return (map[r][c] == 1 || map[r][c] == 2); // 1 for wall, 2 for ghost house door
}

void capture_game_state(GameState* gs) {
    gs->pacman_x = pacman.x;
    gs->pacman_y = pacman.y;
    gs->pacman_dir = pacman.dir;
    gs->score = current_score;
    gs->caught_by_ghost = false;
    gs->level_cleared = false;
}

void reset_game() {
    // Reset map to its original state
    for(int r=0; r<MAP_ROWS; r++) {
        for(int c=0; c<MAP_COLS; c++) {
            map[r][c] = MAP_DEFINITION[r][c];
        }
    }

    pacman.x = 13; pacman.y = 23;
    pacman.dir = RIGHT;
    current_score = 0;

    current_mode = MODE_SCATTER;
    mode_timer = 0;

    ghosts[0] = (Ghost){0, 13, 11, LEFT, {255,0,0,255}};
    ghosts[1] = (Ghost){1, 13, 14, UP, {255,182,255,255}};
    ghosts[2] = (Ghost){2, 11, 14, UP, {0,255,255,255}};
    ghosts[3] = (Ghost){3, 15, 14, UP, {255,182,85,255}};
}

// --- DETERMINISTIC GHOST ---
void move_ghosts_deterministic() {
    mode_timer++;
    int switch_needed = 0;

    if (current_mode == MODE_SCATTER && mode_timer > SCATTER_TIME) {
        current_mode = MODE_CHASE;
        mode_timer = 0;
        switch_needed = 1;
    } else if (current_mode == MODE_CHASE && mode_timer > CHASE_TIME) {
        current_mode = MODE_SCATTER;
        mode_timer = 0;
        switch_needed = 1;
    }

    for(int i=0; i<4; i++) {
        Ghost *g = &ghosts[i];

        if (switch_needed && !is_in_ghost_house(g->x, g->y)) {
            g->dir = (g->dir + 2) % 4; // Reverse direction
        }

        int tx, ty; // Target X, Target Y

        if (is_in_ghost_house(g->x, g->y)) {
            tx = 13; ty = 11; // Exit of ghost house
        }
        else if (current_mode == MODE_SCATTER) {
            // Scatter targets for each ghost
            if (g->id == 0) { tx = MAP_COLS-2; ty = 1; }           // Blinky (Red) - Top-right
            else if (g->id == 1) { tx = 1; ty = 1; }                // Pinky (Pink) - Top-left
            else if (g->id == 2) { tx = MAP_COLS-2; ty = MAP_ROWS-2; } // Inky (Cyan) - Bottom-right
            else if (g->id == 3) { tx = 1; ty = MAP_ROWS-2; }       // Clyde (Orange) - Bottom-left
        }
        else { // MODE_CHASE
            // Chase targets based on ghost ID
            tx = pacman.x; ty = pacman.y; // Blinky's target (Pacman's position)
            if (g->id == 1) { // Pinky's target (4 tiles ahead of Pacman)
                tx = pacman.x + (4 * dx[pacman.dir]);
                ty = pacman.y + (4 * dy[pacman.dir]);
            } else if (g->id == 2) { // Inky's target (complex, based on Blinky and Pacman)
                int px = pacman.x + (2 * dx[pacman.dir]);
                int py = pacman.y + (2 * dy[pacman.dir]);
                tx = px + (px - ghosts[0].x);
                ty = py + (py - ghosts[0].y);
            } else if (g->id == 3) { // Clyde's target (Pacman if far, scatter corner if close)
                int d = abs(g->x - pacman.x) + abs(g->y - pacman.y);
                if (d < 8) { tx = 0; ty = MAP_ROWS; } // Scatter corner (bottom-left) if close
            }
        }

        int best_dir = -1;
        int min_dist = 99999;

        int currently_inside = is_in_ghost_house(g->x, g->y);
        int reverse_dir = (g->dir + 2) % 4;
        int start_dir = rand() % 4; // Randomize starting direction to break ties

        for (int k = 0; k < 4; k++) {
            int d_idx = (start_dir + k) % 4;
            if (d_idx == reverse_dir) continue; // Cannot reverse direction

            int nx = g->x + dx[d_idx];
            int ny = g->y + dy[d_idx];

            if (!currently_inside && is_in_ghost_house(nx, ny)) continue; // Cannot enter ghost house from outside (except door)
            if (is_wall(nx, ny)) {
                // Allow ghosts inside the house to pass through the door (tile type 2)
                if (map[ny][nx] == 2 && currently_inside) {
                    // It's the door and we are inside, so this is a valid exit move.
                } else continue; // It's a real wall, so skip.
            }
            // Calculate squared Euclidean distance to target
            int dist = (nx - tx)*(nx - tx) + (ny - ty)*(ny - ty);
            if (dist < min_dist) {
                min_dist = dist;
                best_dir = d_idx;
            }
        }

        if (best_dir != -1) {
            g->dir = best_dir;
            g->x += dx[best_dir];
            g->y += dy[best_dir];
        }

        // Handle tunnel wrapping
        if (g->x < 0) g->x = MAP_COLS - 1;
        if (g->x >= MAP_COLS) g->x = 0;
    }
}

void init_sdl_and_window(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    *window = SDL_CreateWindow("Pac-Man RL (Quadrant Rewards)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
}

void render_game(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for(int r=0; r<MAP_ROWS; r++) {
        for(int c=0; c<MAP_COLS; c++) {
            int id = map[r][c];
            if (id == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
                SDL_Rect wall = {c * TILE_SIZE + 2, r * TILE_SIZE + 2, TILE_SIZE - 4, TILE_SIZE - 4};
                SDL_RenderFillRect(renderer, &wall);
            } else if (id == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 184, 151, 255);
                SDL_Rect dot = {c * TILE_SIZE + 6, r * TILE_SIZE + 6, 4, 4};
                SDL_RenderFillRect(renderer, &dot);
            } else if (id == 2) { // Ghost house door
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_Rect door = {c * TILE_SIZE, r * TILE_SIZE + TILE_SIZE/2 - 1, TILE_SIZE, 2};
                SDL_RenderFillRect(renderer, &door);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect pacRect = {pacman.x * TILE_SIZE, pacman.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &pacRect);

    for(int i=0; i<4; i++) {
        SDL_SetRenderDrawColor(renderer, ghosts[i].color.r, ghosts[i].color.g, ghosts[i].color.b, 255);
        SDL_Rect gRect = {ghosts[i].x * TILE_SIZE, ghosts[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &gRect);
    }

    SDL_RenderPresent(renderer);
}

void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}