# Compiler to use
CC = clang

# Target executable name
TARGET = pacman

# Compiler flags:
# -g: Include debugging information
# -Wall -Wextra: Enable all and extra warnings for better code quality
# -std=c99: Use the C99 standard (optional but good practice)
CFLAGS = -g -Wall -Wextra -std=c99

# Preprocessor flags:
# -I: Specify include directories (for SDL2)
CPPFLAGS = -I/opt/homebrew/include

# Linker flags:
# -L: Specify library search directories
# -l: Specify libraries to link (SDL2)
LDFLAGS = -L/opt/homebrew/lib -lSDL2

# Source files
SRCS = main.c game.c features.c rewards.c rl.c ml_agent.c

# Object files (automatically generated from SRCS)
OBJS = $(SRCS:.c=.o)

# The default goal: build the target executable
.PHONY: all
all: $(TARGET)
	@echo "Build successful. Cleaning up object files..."
	@rm -f $(OBJS)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to clean up build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)