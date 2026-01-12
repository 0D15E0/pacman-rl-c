#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h> // For bool return types

// --- CONSTANTS ---
#define TILE_SIZE 16
#define MAP_COLS 28
#define MAP_ROWS 31
#define SCREEN_WIDTH  (MAP_COLS * TILE_SIZE)
#define SCREEN_HEIGHT (MAP_ROWS * TILE_SIZE)

// DIRECTIONS
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

// GLOBAL MODE
#define MODE_SCATTER 0
#define MODE_CHASE 1
#define SCATTER_TIME 150
#define CHASE_TIME 300

// --- ENTITIES ---
typedef struct {
    int x, y;
    int dir;
} Entity;

typedef struct {
    int id;
    int x, y;
    int dir;
    SDL_Color color;
} Ghost;

// A snapshot of the game's state, used for reward calculation.
typedef struct {
    // Pacman state
    int pacman_x;
    int pacman_y;
    int pacman_dir;

    // Score and dots
    int score;
    // Outcome
    bool caught_by_ghost;
    bool level_cleared;
} GameState;

// --- EXTERNAL GAME STATE VARIABLES ---
extern int map[MAP_ROWS][MAP_COLS];
extern Entity pacman;
extern Ghost ghosts[4];
extern int dx[4];
extern int dy[4];

extern int current_score;
// The maximum possible score is the total number of dots.
static const int MAX_SCORE = 244;
static const int MAP_DEFINITION[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,9,1,1,9,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,9,1,1,9,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,9,9,9,9,9,9,9,9,9,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,1,1,1,2,2,1,1,1,9,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,1,9,9,9,9,9,9,1,9,1,1,0,1,1,1,1,1,1},
    {9,9,9,9,9,9,0,9,9,9,1,9,9,9,9,9,9,1,9,9,9,0,9,9,9,9,9,9},
    {1,1,1,1,1,1,0,1,1,9,1,9,9,9,9,9,9,1,9,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,1,1,1,1,1,1,1,1,9,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,9,9,9,9,9,9,9,9,9,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,1,1,1,1,1,1,1,1,9,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,9,1,1,1,1,1,1,1,1,9,1,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,1,1,0,0,0,0,0,0,0,9,9,0,0,0,0,0,0,0,1,1,0,0,0,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// --- GAME FUNCTION PROTOTYPES ---
bool is_in_ghost_house(int x, int y);
bool is_wall(int c, int r);
void capture_game_state(GameState* gs);
void reset_game();
void move_ghosts_deterministic();
void init_sdl_and_window(SDL_Window** window, SDL_Renderer** renderer);
void render_game(SDL_Renderer* renderer);
void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer);

#endif // GAME_H