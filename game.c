#include "defs.h"
#include "math.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define GRID_WIDTH 14
#define GRID_HEIGHT 8

static const size_t LARGE_PLATFORM_WIDTH = 80;
static const size_t SMALL_PLATFORM_WIDTH = 40;
static const size_t BLOCK_WIDTH = 25;
static const size_t BLOCK_HEIGHT = 10;
static const size_t BLOCK_MARGIN = 3;
static const size_t STARTING_BALLS = 3;
static const size_t GRID_START_Y = 50;

static const float PLATFORM_MOVMENT_PER_MS = 0.25;
static const float PLATFORM_Y_POS = 600 - 100;
static const float PLATFORM_HEIGHT = 10;
static const float BALL_SIZE = 10;

#define GAME_WIDTH (BLOCK_WIDTH * GRID_WIDTH + (GRID_WIDTH + 1) * BLOCK_MARGIN)
#define GAME_HEIGHT 550
#define GAME_START ((WINDOW_WIDTH - GAME_WIDTH) / 2.0f)

static const size_t GAME_BORDER_WIDTH = 3;
static const bool PRESNT = false;
static const bool BROKEN = true;
static const Color BLOCK_COLORS[GRID_WIDTH / 2] = {
    {.r = 150, .g = 44, .b = 25, .a = 255},
    {.r = 185, .g = 136, .b = 47, .a = 255},
    {.r = 59, .g = 131, .b = 61, .a = 255},
    {.r = 194, .g = 194, .b = 74, .a = 255},
};

typedef enum BallSpeed {
  SLOW,
  FAST,
} BallSpeed;

typedef enum Level {
  FIRST,
  SECOND,
} Level;

// false => present
// true  => broken
static bool grid[GRID_HEIGHT][GRID_WIDTH];
static BallSpeed ball_speed;
static Level current_level;
static size_t cur_platform_width;
static size_t balls_left;
static size_t score;
static bool ball_active;
static Vector2 ball_pos;
static Vector2 ball_vel;
static Vector2 platform_pos;
static float platform_x_vel;

void clamp_platform() {}

void update_platform() {
  float frame_time = GetFrameTime() * 1000;
  printf("FRAME_TIME = %f\n", frame_time);
  if (IsKeyDown(KEY_A) | IsKeyDown(KEY_LEFT)) {
    platform_x_vel = -PLATFORM_MOVMENT_PER_MS;
  } else if (IsKeyDown(KEY_D) | IsKeyDown(KEY_RIGHT)) {
    platform_x_vel = PLATFORM_MOVMENT_PER_MS;
  } else {
    platform_x_vel = 0;
  }

  float min_x = GAME_START + (cur_platform_width / 2.0);
  float max_x = GAME_START + GAME_WIDTH - (cur_platform_width / 2.0);
  float new_x = platform_pos.x + frame_time * platform_x_vel;
  new_x = fmaxf(new_x, min_x);
  new_x = fminf(new_x, max_x);

  platform_pos.x = new_x;
}

void update_ball() {}

void init_game(void) {
  memset(grid, 0, sizeof(grid));
  ball_speed = SLOW;
  current_level = FIRST;
  cur_platform_width = LARGE_PLATFORM_WIDTH;
  balls_left = STARTING_BALLS;
  score = 0;
  ball_active = false;

  ball_pos = (Vector2){.x = 0, .y = 0};
  ball_vel = (Vector2){.x = 0, .y = 0};
  platform_pos =
      (Vector2){.x = GAME_START + (GAME_WIDTH / 2.0), .y = PLATFORM_Y_POS};
}

void update_game(void) { update_platform(); }
void draw_game(void) {
  DrawRectangleRec(
      (Rectangle){
          .x = GAME_START,
          .y = (WINDOW_HEIGHT - GAME_HEIGHT) / 2,
          .width = GAME_WIDTH,
          .height = GAME_HEIGHT,
      },
      COLOR_MENU_BG);
  Rectangle platform_rect = {.x = platform_pos.x - (cur_platform_width / 2.0),
                             .y = platform_pos.y - (PLATFORM_HEIGHT / 2.0),
                             .width = cur_platform_width,
                             .height = PLATFORM_HEIGHT};

  DrawRectangleRec(platform_rect, COLOR_PLATFORM);

  for (size_t row = 0; row < GRID_HEIGHT; row++) {
    for (size_t col = 0; col < GRID_WIDTH; col++) {
      Rectangle block_rect = {
          .x = GAME_START + BLOCK_MARGIN + (BLOCK_MARGIN + BLOCK_WIDTH) * col,
          .y = GRID_START_Y + (BLOCK_MARGIN + BLOCK_HEIGHT) * row,
          .width = BLOCK_WIDTH,
          .height = BLOCK_HEIGHT,
      };

      DrawRectangleRec(block_rect, BLOCK_COLORS[row / 2]);
    }
  }
}
