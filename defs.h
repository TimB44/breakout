#pragma once
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// macros
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

// types

typedef enum GameState {
  HOME,
  GAME,
  END,
} GameState;

// constants

extern const char *WINDOW_TITLE;
extern const float MENU_ROUNDESS;

extern const Color COLOR_BG;
extern const Color COLOR_TEXT;
extern const Color COLOR_BUTTON_HOVER;
extern const Color COLOR_BUTTON;
extern const Color COLOR_MENU_BG;
extern const Color COLOR_MENU_BORDER;
extern const Color COLOR_PLATFORM;
extern const Color COLOR_BALL;
// globals
extern GameState current_state;

// function declaration

// main.c
int main(void);
void update(void);
void draw(void);
void transition_state(GameState);

// home.c
void update_home(void);
void draw_home(void);

// game.c
void init_game(void);
void update_game(void);
void draw_game(void);

// utils.c
void draw_text_centered(const char *, size_t, size_t, size_t);
bool mouse_over(Rectangle);
bool mouse_pressed_on(Rectangle);
Vector2 rect_center(Rectangle);
