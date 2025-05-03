#pragma once

#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// macros

#define GRID_WIDTH 14
#define GRID_HEIGHT 8

// constants

const char *WINDOW_TITLE = "Pong Game";

const size_t WINDOW_HEIGHT = 600;
const size_t WINDOW_WIDTH = 800;

const size_t PLATFORM_SIZE = 80;

const size_t BLOCK_WIDTH = 10;
const size_t BLOCK_HEIGHT = 35;
const size_t BLOCK_MARGIN = 3;

const size_t BORDER_WIDTH = 35;

// types

typedef enum GameState {
  HOME,
  GAME,
  END,
} GameState;

// globals
GameState current_state = HOME;
bool grid[GRID_HEIGHT][GRID_WIDTH];

// function declaration

// main.c
int main(void);
void update(void);
void draw(void);

// home.c
void update_home(void);
void draw_home(void);
