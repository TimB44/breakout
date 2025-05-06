#include "defs.h"

// constants (definitions)
const char *WINDOW_TITLE = "Breakout Game";

const float MENU_ROUNDESS = 0.2;

const Color COLOR_BG = {
    .r = 24,
    .g = 24,
    .b = 24,
    .a = 255,
}; // Almost black
const Color COLOR_TEXT = {
    .r = 240,
    .g = 240,
    .b = 240,
    .a = 255,
}; // Light gray (almost white)

const Color COLOR_BUTTON_HOVER = {
    .r = 200,
    .g = 50,
    .b = 50,
    .a = 255,
};
const Color COLOR_BUTTON = {
    .r = 160,
    .g = 30,
    .b = 30,
    .a = 255,
};

const Color COLOR_MENU_BG = {
    .r = 48,
    .g = 48,
    .b = 48,
    .a = 255,
};

const Color COLOR_MENU_BORDER = {
    .r = 80,
    .g = 80,
    .b = 80,
    .a = 255,
};

const Color COLOR_PLATFORM = {
    .r = 240,
    .g = 240,
    .b = 240,
    .a = 255,
};

const Color COLOR_BALL = SKYBLUE;
// globals
GameState current_state = HOME;
