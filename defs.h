#pragma once
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// macros
#define GRID_WIDTH 14
#define GRID_HEIGHT 8

// types

typedef enum GameState {
  HOME,
  GAME,
  END,
} GameState;

// constants

extern const char *WINDOW_TITLE;
extern const size_t WINDOW_HEIGHT;
extern const size_t WINDOW_WIDTH;
extern const size_t PLATFORM_SIZE;
extern const size_t BLOCK_WIDTH;
extern const size_t BLOCK_HEIGHT;
extern const size_t BLOCK_MARGIN;
extern const size_t BORDER_WIDTH;

extern const float MENU_ROUNDESS;

extern const Color COLOR_BG;
extern const Color COLOR_TEXT;
extern const Color COLOR_BUTTON_HOVER;
extern const Color COLOR_BUTTON;
extern const Color COLOR_MENU_BG;
extern const Color COLOR_MENU_BORDER;

// globals
extern GameState current_state;
extern bool grid[GRID_HEIGHT][GRID_WIDTH];

// function declaration

// main.c
int main(void);
void update(void);
void draw(void);
void transition_state(GameState);

// home.c
void update_home(void);
void draw_home(void);

// utils.c

void draw_text_centered(const char *, size_t, size_t, size_t);
bool mouse_over(Rectangle);
bool mouse_pressed_on(Rectangle);
Vector2 rect_center(Rectangle);
