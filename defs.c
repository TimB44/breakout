#include "defs.h"

// constants (definitions)
const char *WINDOW_TITLE = "Pong Game";
const size_t WINDOW_HEIGHT = 600;
const size_t WINDOW_WIDTH = 800;
const size_t PLATFORM_SIZE = 80;
const size_t BLOCK_WIDTH = 10;
const size_t BLOCK_HEIGHT = 35;
const size_t BLOCK_MARGIN = 3;
const size_t BORDER_WIDTH = 35;

const float MENU_ROUNDESS = 0.2;

const Color COLOR_BG = {24, 24, 24, 255};      // Almost black
const Color COLOR_TEXT = {240, 240, 240, 255}; // Light gray (almost white)

const Color COLOR_BUTTON_HOVER = {200, 50, 50, 255}; // Bright red
const Color COLOR_BUTTON = {160, 30, 30, 255};       // Deep red

const Color COLOR_MENU_BG = {48, 48, 48, 255}; // Deep gray

const Color COLOR_MENU_BORDER = {80, 80, 80, 255}; // Mid-gray

// globals (definitions)
GameState current_state = HOME;
bool grid[GRID_HEIGHT][GRID_WIDTH] = {false};
