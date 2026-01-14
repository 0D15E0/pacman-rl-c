#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h> // For bool return types

// --- CONSTANTS ---
#define TILE_SIZE 16
#define MAP_COLS 28
#define MAP_ROWS 31
#define SCREEN_WIDTH (MAP_COLS * TILE_SIZE)
#define HEADER_HEIGHT 32
#define SCREEN_HEIGHT (MAP_ROWS * TILE_SIZE + HEADER_HEIGHT)

// DIRECTIONS
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

// GLOBAL MODE
#define MODE_SCATTER 0
#define MODE_CHASE 1
#define MODE_FRIGHTENED 2
#define SCATTER_TIME 150
#define CHASE_TIME 300
#define FRIGHTENED_TIME 100

// Tile IDs
#define TILE_DOT 0
#define TILE_WALL 1
#define TILE_DOOR 2
#define TILE_BIG_PILL 3
#define TILE_EMPTY 9

// Scores
#define SCORE_DOT 1
#define SCORE_BIG_PILL 10
#define SCORE_GHOST 200

// Ghost IDs
#define GHOST_BLINKY 0
#define GHOST_PINKY 1
#define GHOST_INKY 2
#define GHOST_CLYDE 3

// Tile IDs
#define TILE_DOT 0
#define TILE_WALL 1
#define TILE_DOOR 2
#define TILE_BIG_PILL 3
#define TILE_EMPTY 9

// Scores
#define SCORE_DOT 1
#define SCORE_BIG_PILL 10
#define SCORE_GHOST 200

// Ghost IDs
#define GHOST_BLINKY 0
#define GHOST_PINKY 1
#define GHOST_INKY 2
#define GHOST_CLYDE 3

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
  bool is_eaten;
  bool is_frightened;
} Ghost;

// A snapshot of the game's state, used for reward calculation.
typedef struct {
  // Pacman state
  int pacman_x;
  int pacman_y;
  int pacman_dir;

  // Score and dots
  int score;
  int ghosts_eaten;
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

extern int current_mode;
extern int mode_timer;

extern int current_score;
extern int pills_eaten;
// The maximum possible score is the total number of dots.
static const int MAX_SCORE = 244;
static const int MAP_DEFINITION[MAP_ROWS][MAP_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 3, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 3, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,
     1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,
     1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 9, 1,
     1, 9, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 9, 1,
     1, 9, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 9, 9, 9, 9,
     9, 9, 9, 9, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1, 1, 1, 2,
     2, 1, 1, 1, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1, 9, 9, 9,
     9, 9, 9, 1, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {9, 9, 9, 9, 9, 9, 0, 9, 9, 9, 1, 9, 9, 9,
     9, 9, 9, 1, 9, 9, 9, 0, 9, 9, 9, 9, 9, 9},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1, 9, 9, 9,
     9, 9, 9, 1, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1, 1, 1, 1,
     1, 1, 1, 1, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 9, 9, 9, 9,
     9, 9, 9, 9, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1, 1, 1, 1,
     1, 1, 1, 1, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1, 1, 1, 1,
     1, 1, 1, 1, 9, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 3, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 9,
     9, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3, 1},
    {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,
     1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,
     1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
     1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// --- GAME FUNCTION PROTOTYPES ---
bool is_in_ghost_house(int x, int y);
bool is_wall(int c, int r);
void capture_game_state(GameState *gs);
void reset_game();
void move_ghosts_deterministic();
// --- HELPER FUNCTIONS ---
void check_ghost_collisions(bool *episode_done);
void init_sdl_and_window(SDL_Window **window, SDL_Renderer **renderer);
// --- FONT RENDERING ---
static const unsigned char font_data[10][5] = {
    {0x7, 0x5, 0x5, 0x5, 0x7}, // 0
    {0x2, 0x6, 0x2, 0x2, 0x7}, // 1
    {0x7, 0x1, 0x7, 0x4, 0x7}, // 2
    {0x7, 0x1, 0x7, 0x1, 0x7}, // 3
    {0x5, 0x5, 0x7, 0x1, 0x1}, // 4
    {0x7, 0x4, 0x7, 0x1, 0x7}, // 5
    {0x7, 0x4, 0x7, 0x5, 0x7}, // 6
    {0x7, 0x1, 0x1, 0x1, 0x1}, // 7
    {0x7, 0x5, 0x7, 0x5, 0x7}, // 8
    {0x7, 0x5, 0x7, 0x1, 0x7}  // 9
};

static const unsigned char char_S[5] = {0x7, 0x4, 0x7, 0x1, 0x7};
static const unsigned char char_C[5] = {0x7, 0x4, 0x4, 0x4, 0x7};
static const unsigned char char_O[5] = {0x7, 0x5, 0x5, 0x5, 0x7};
static const unsigned char char_R[5] = {0x7, 0x5, 0x7, 0x6, 0x5};
static const unsigned char char_E[5] = {0x7, 0x4, 0x6, 0x4, 0x7};
static const unsigned char char_COLON[5] = {0x0, 0x2, 0x0, 0x2, 0x0};

void draw_pixel_char(SDL_Renderer *renderer, int x, int y,
                     const unsigned char glyph[5], int scale);
void draw_score(SDL_Renderer *renderer, int x, int y, int score);

void render_game(SDL_Renderer *renderer);
void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer);

#endif // GAME_H