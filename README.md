# Pac-Man Reinforcement Learning Project

This project is inspired by the classic Pac-Man game implemented in C, featuring a reinforcement learning agent that learns to play the game using Q-learning.

## Features

-   **Classic Pac-Man Gameplay:** A faithful recreation of the core Pac-Man mechanics, including dots, power-ups, and ghost house behavior recreated with simple colored squares for a clean, retro feel.
-   **On-Screen Score Display:** Real-time score tracking at the top of the screen rendered using a custom-built pixel font.
-   **Enhanced Reinforcement Learning:** A highly optimized Q-learning agent that uses relative spatial features for extremely fast convergence.
-   **Power-Up System:** Consume "Big Pills" to turn ghosts frightened (blue), allowing Pac-Man to eat them for significant bonus points.
-   **Pluggable Architecture:** The agent's core logic is decoupled from the game. You can easily swap out different state representation (`features.c`) and reward (`rewards.c`) functions to experiment with agent behavior.
-   **Manual Override:** Take control of Pac-Man at any time with the arrow keys to guide the agent's learning through demonstration.
-   **Turbo Mode:** Disable rendering to run training simulations at maximum speed.
-   **Built with C and SDL2:** A lightweight and cross-platform implementation.

## Building and Running

### Dependencies

You will need the SDL2 development library installed on your system.

-   **macOS (using Homebrew):**
    ```bash
    brew install sdl2
    ```
-   **Debian/Ubuntu:**
    ```bash
    sudo apt-get install libsdl2-dev
    ```

### A Note on Library Paths

The `Makefile` is pre-configured with paths for an SDL2 installation via Homebrew on Apple Silicon (`/opt/homebrew/...`). If you installed SDL2 elsewhere or are on a different system (like Linux), you may need to update the `CPPFLAGS` and `LDFLAGS` variables in the `Makefile` to point to the correct include and library directories for SDL2 on your system.

For example, on many Linux systems, the `LDFLAGS` might not need a special path, and the `CPPFLAGS` might point to `/usr/include/SDL2`.

### Compilation

The project includes a `Makefile` for easy compilation. Simply run `make` in the project's root directory.

```bash
make
```

This will create an executable named `pacman` in the project directory.

### Running the Game

To run the game, execute the compiled binary:

```bash
./pacman
```

## How to Play

-   **Arrow Keys:** Control Pac-Man when in Manual Override mode.
-   **'M' Key:** Toggle between AI Agent control and Manual Override.
-   **'T' Key:** Toggle Turbo Mode for accelerated training (disables rendering).
-   **'Q' Key / Close Window:** Quit the application.

## Project Structure

The project is organized into several modules to separate concerns:

-   `main.c`: The main application entry point, containing the game loop and event handling.
-   `game.c`: Core game logic, including entity movement, collision detection, and rendering.
-   `ml_agent.c`: An abstraction layer that connects the game to the underlying reinforcement learning algorithm.
-   `features.c`: Defines the agent's state representation. This is where you can change how the agent "sees" the world.
-   `rewards.c`: Defines the reward function. This is where you shape the agent's goals and behavior.
-   `rl.c`: The core Q-learning algorithm implementation, including the Q-Table and update logic.
-   `*.h`: Header files defining the public interfaces for each module.

## Reinforcement Learning Details

The agent learns using a standard Q-learning algorithm.

### State Representation

The agent's "state" is a single integer representing the local environment, reducing the state space from millions to just **~1,500 unique states**. This allows the agent to reach high performance within minutes:

1.  **Nearest Dot Direction (4 values):** The direction (UP, DOWN, LEFT, RIGHT) to the closest remaining dot or big pill.
2.  **Nearest Ghost Proximity (3 levels):** Discretized distance to the closest ghost (Near: 1-2 tiles, Medium: 3-5 tiles, Far: 6+ tiles).
3.  **Nearest Ghost Direction (4 values):** The relative direction to the closest ghost.
4.  **Ghost Frightened Status (2 values):** Whether the nearest ghost is currently frightened (blue) and edible.
5.  **Valid Moves (16 values):** A 4-bit mask of legal directions at the current position.

### Reward Function

The agent receives "dense" feedback on every step to guide it towards goals and away from danger:

-   **Goal Rewards:**
    -   Level Clear: +1000.0
    -   Eating a Dot/Pill: +20.0
    -   Eating a Ghost: +200.0
    -   **Proximity Bonus:** +2.0 for moving closer to the nearest dot.
-   **Danger Penalties:**
    -   Caught by Ghost: -500.0
    -   **Proximity Penalty:** -15.0 for moving towards a non-frightened ghost within 5 tiles.
    -   Wall Hit: -15.0
-   **Efficiency:** 
    -   Step Penalty: -1.0 (encourages speed).
    -   Reversing Direction: -5.0 (discourages oscillation).

### Algorithm

-   **Q-Learning:** The agent updates its Q-Table based on the reward received and the estimated value of the next state.
-   **Bellman Equation:** The Q-learning update rule is a practical application of the Bellman equation, which recursively defines the value of an action in a given state based on the expected future rewards.
-   **Epsilon-Greedy Policy:** The agent balances exploration (taking random actions) and exploitation (taking the best-known action). The `epsilon` value, which controls the probability of exploring, decays over time, causing the agent to rely more on its learned strategy as it gains experience.