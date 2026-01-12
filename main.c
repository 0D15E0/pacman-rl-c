#include "game.h"
#include "ml_agent.h"
#include "features.h"
#include "rewards.h"
#include <stdio.h>
#include <stdlib.h>

// The number of actions Pac-Man can take.
#define NUM_ACTIONS 4

static int turbo_mode = 0;
static int manual_override = 0;
static int desired_action = RIGHT;

int main(void) {
    srand(time(NULL));

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    init_sdl_and_window(&window, &renderer);
    // To change behavior, plug in different functions from features.h and rewards.h.
    ml_agent_init(get_complex_state_id, get_standard_reward, COMPLEX_STATE_SPACE_SIZE, NUM_ACTIONS);

    int quit = 0;
    SDL_Event e;

    reset_game(); // Initialize game state
    int state = ml_agent_get_current_state();

    GameState before_state;
    GameState after_state;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_t) {
                    turbo_mode = !turbo_mode;
                    printf("Turbo: %s\n", turbo_mode ? "ON" : "OFF");
                } else if (e.key.keysym.sym == SDLK_m) {
                    manual_override = !manual_override;
                    printf("Manual Override: %s\n", manual_override ? "ON" : "OFF");
                }
                if (manual_override) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:    desired_action = UP;    break;
                        case SDLK_DOWN:  desired_action = DOWN;  break;
                        case SDLK_LEFT:  desired_action = LEFT;  break;
                        case SDLK_RIGHT: desired_action = RIGHT; break;
                        default: break;
                    }
                }
            }
        }

        if (!manual_override) {
            desired_action = ml_agent_get_next_action(state);
        }

        // --- Buffered Input Logic ---
        int action;
        int next_x_desired = pacman.x + dx[desired_action];
        int next_y_desired = pacman.y + dy[desired_action];

        // If the desired move is valid, take it.
        if (!is_wall(next_x_desired, next_y_desired)) {
            action = desired_action;
        } else {
            // Otherwise, try to continue in the current direction.
            int next_x_straight = pacman.x + dx[pacman.dir];
            int next_y_straight = pacman.y + dy[pacman.dir];
            action = !is_wall(next_x_straight, next_y_straight) ? pacman.dir : desired_action;
        }

        // --- CAPTURE PRE-ACTION STATE ---
        capture_game_state(&before_state);

        // new position
        int next_x = pacman.x + dx[action];
        int next_y = pacman.y + dy[action];

        // Handle tunnel wrapping for Pacman
        if (next_x < 0) next_x = MAP_COLS - 1;
        if (next_x >= MAP_COLS) next_x = 0;
        
        if (is_wall(next_x, next_y)) {
            // Pacman doesn't move if it hits a wall. Position remains the same.
        } else {
            // Move Pacman
            pacman.x = next_x;
            pacman.y = next_y;
            pacman.dir = action;

            if (map[pacman.y][pacman.x] == 0) { // Pacman eats a dot at the new location
                map[pacman.y][pacman.x] = 9; // Mark as eaten
                current_score++;
            }
        }

        move_ghosts_deterministic(); // Move ghosts

        // --- CAPTURE POST-ACTION STATE ---
        capture_game_state(&after_state);

        bool episode_done = false;
        // Check for collision with ghosts
        for(int i=0; i<4; i++) {
            if (pacman.x == ghosts[i].x && pacman.y == ghosts[i].y) {
                after_state.caught_by_ghost = true; // Update the 'after' state with the outcome
                episode_done = true; // Episode ends
                break;
            }
        }

        // Check for win condition (all dots eaten)
        if (current_score >= MAX_SCORE) {
            printf("LEVEL CLEARED!\n");
            after_state.level_cleared = true;
            episode_done = true;
        }

        // --- UPDATE THE ML AGENT ---
        float reward = ml_agent_get_reward(&before_state, &after_state, action);
        int next_state = ml_agent_get_current_state();
        ml_agent_update(state, action, next_state, reward);

        // --- PREPARE FOR NEXT STEP ---
        state = next_state;

        if (episode_done) {
            ml_agent_end_episode(current_score);
            reset_game(); // Reset game for a new episode
            if (manual_override) {
                desired_action = pacman.dir; // Reset desired action to default on new episode
            }
            state = ml_agent_get_current_state();
            desired_action = ml_agent_get_next_action(state);
        }

        // --- RENDER (if not in turbo mode) ---
        if (!turbo_mode) {
            render_game(renderer);
            SDL_Delay(100); // Frame delay
        }
    }

    ml_agent_cleanup(); // Deallocate Q-Table and other agent resources
    cleanup_sdl(window, renderer); // Clean up SDL resources

    return 0;
}