#include "defs.h"
#include "math.h"
#include "raylib.h"
#include "raymath.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_WIDTH 14
#define GRID_HEIGHT 8

#define LARGE_PLATFORM_WIDTH 80
#define SMALL_PLATFORM_WIDTH 40
#define BLOCK_WIDTH 25
#define BLOCK_HEIGHT 10
#define BLOCK_MARGIN 3
#define STARTING_BALLS 3
#define GRID_START_Y 50

#define PLATFORM_MOVMENT_PER_MS 0.25
#define PLATFORM_Y_POS 600 - 100
#define PLATFORM_HEIGHT 10
#define PLATFORM_EDGE_X_MOV 0.8
static_assert(PLATFORM_EDGE_X_MOV < 1.0, "X movment should be less then 1");

#define SLOW_BALL_MOVMENT_PER_MS 0.2
#define FAST_BALL_MOVMENT_PER_MS 0.4
#define BALL_SIZE 10
#define DEATH_LINE_Y WINDOW_HEIGHT

#define GAME_WIDTH (BLOCK_WIDTH * GRID_WIDTH + (GRID_WIDTH + 1) * BLOCK_MARGIN)
#define GAME_HEIGHT 550
#define GAME_START ((WINDOW_WIDTH - GAME_WIDTH) / 2.0f)
#define GAME_START_Y (WINDOW_HEIGHT - GAME_HEIGHT) / 2.0
#define GAME_END_Y GAME_START_Y + GAME_HEIGHT
#define GAME_BORDER_WIDTH 3
#define PRESENT false
#define BROKEN true

#define BLOCK_COLOR_GROUP_SIZE 2

#define GAME_WALL_WIDTH 10
#define GAME_WALL_COUNT 3

// #define BALL_CHAR '⬜'
// TODO: use box but acount for it being 3 bytes
#define BALL_CHAR 'X'
#define BALL_CHAR_START_INDEX 7
#define BALL_CHAR_MAX_LEN 3
#define SCORE_NUM_START_INDEX 7
#define SCORE_NUM_MAX_LEN 3

typedef enum BallSpeed {
  SLOW,
  FAST,
} BallSpeed;

typedef enum Level {
  FIRST,
  SECOND,
} Level;

typedef struct CollisionResult {
  Vector2 undo_move;
  Vector2 new_dir;
} CollisionResult;

static const Color BLOCK_COLORS[GRID_WIDTH / BLOCK_COLOR_GROUP_SIZE] = {
    {.r = 150, .g = 44, .b = 25, .a = 255},
    {.r = 185, .g = 136, .b = 47, .a = 255},
    {.r = 59, .g = 131, .b = 61, .a = 255},
    {.r = 194, .g = 194, .b = 74, .a = 255},
};

static const size_t ROW_TO_POINTS[GRID_HEIGHT] = {
    1, 1, 3, 3, 5, 5, 7, 7,
};

static const Rectangle GAME_WALLS[GAME_WALL_COUNT] = {
    {
        .x = GAME_START - GRID_WIDTH,
        .y = GAME_START_Y,
        .width = GAME_WALL_WIDTH,
        .height = GAME_HEIGHT,
    },
    {
        .x = GAME_START,
        .y = GAME_START_Y - GAME_WALL_WIDTH,
        .width = GAME_WIDTH,
        .height = GAME_WALL_WIDTH,
    },
    {
        .x = GAME_START + GAME_WIDTH,
        .y = GAME_START_Y,
        .width = GAME_WALL_WIDTH,
        .height = GAME_HEIGHT,
    },
};

// false => present
// true  => broken
static bool grid[GRID_HEIGHT][GRID_WIDTH];

static BallSpeed ball_speed;
static Level current_level;
static size_t cur_platform_width;
static size_t balls_left;
static char balls_left_str[BALL_CHAR_START_INDEX + BALL_CHAR_MAX_LEN + 1] =
    "BALLS:    ";
static size_t score;
static char score_str[SCORE_NUM_START_INDEX + SCORE_NUM_MAX_LEN + 1] =
    "SCORE:    ";

static bool ball_active;
static Vector2 ball_pos;
static Vector2 ball_dir;

static Vector2 platform_pos;
static float platform_x_vel;

void init_game(void) {
  memset(grid, 0, sizeof(grid));
  ball_speed = SLOW;
  current_level = FIRST;
  cur_platform_width = LARGE_PLATFORM_WIDTH;
  balls_left = STARTING_BALLS;
  score = 0;
  ball_active = false;

  ball_pos = (Vector2){.x = 0, .y = 0};
  ball_dir = (Vector2){.x = 0, .y = 0};
  platform_pos =
      (Vector2){.x = GAME_START + (GAME_WIDTH / 2.0), .y = PLATFORM_Y_POS};
}

static void update_platform() {
  float frame_time = GetFrameTime() * 1000;
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
    platform_x_vel = -PLATFORM_MOVMENT_PER_MS;
  } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
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

static CollisionResult check_overlap(Rectangle ball, Rectangle obstacle) {
  float horizontal_overlap =
      fminf(ball.x + ball.width, obstacle.x + obstacle.width) -
      fmaxf(ball.x, obstacle.x);

  float vertical_overlap =
      fminf(ball.y + ball.height, obstacle.y + obstacle.height) -
      fmaxf(ball.y, obstacle.y);

  if (horizontal_overlap < 0 || vertical_overlap < 0)
    return (CollisionResult){
        .undo_move = Vector2Zero(),
        .new_dir = Vector2Zero(),
    };

  Vector2 dir_to_move = Vector2Scale(ball_dir, -1);
  float horizontal_scale = horizontal_overlap / fabsf(dir_to_move.x);
  float vertical_scale = vertical_overlap / fabsf(dir_to_move.y);
  dir_to_move =
      Vector2Scale(dir_to_move, fminf(horizontal_scale, vertical_scale));

  Vector2 reflection = fabsf(horizontal_overlap) < fabsf(vertical_overlap)
                           ? (Vector2){.x = -1, .y = 1}
                           : (Vector2){.x = 1, .y = -1};
  return (CollisionResult){
      .undo_move = dir_to_move,
      .new_dir = Vector2Multiply(ball_dir, reflection),
  };
}

static CollisionResult check_block_collisions(Rectangle ball, size_t *row,
                                              size_t *col) {
  CollisionResult max = (CollisionResult){
      .undo_move = Vector2Zero(),
      .new_dir = Vector2Zero(),
  };

  int out_row = -1;
  int out_col = -1;
  // TODO optimize loops to only check 4 boxes
  for (size_t row = 0; row < GRID_HEIGHT; row++) {
    for (size_t col = 0; col < GRID_WIDTH; col++) {
      if (!grid[row][col]) {
        Rectangle block_rect = {
            .x = GAME_START + BLOCK_MARGIN + (BLOCK_MARGIN + BLOCK_WIDTH) * col,
            .y = GRID_START_Y + (BLOCK_MARGIN + BLOCK_HEIGHT) * row,
            .width = BLOCK_WIDTH,
            .height = BLOCK_HEIGHT,
        };
        CollisionResult new = check_overlap(ball, block_rect);
        if (Vector2LengthSqr(new.undo_move) > Vector2LengthSqr(max.undo_move)) {
          max = new;
          out_row = row;
          out_col = col;
        }
      }
    }
  }

  *row = out_row;
  *col = out_col;
  return max;
}

static CollisionResult check_wall_collisions(Rectangle ball) {
  CollisionResult max = (CollisionResult){
      .undo_move = Vector2Zero(),
      .new_dir = Vector2Zero(),
  };

  for (size_t i = 0; i < GAME_WALL_COUNT; i++) {
    CollisionResult new = check_overlap(ball, GAME_WALLS[i]);
    if (Vector2LengthSqr(new.undo_move) > Vector2LengthSqr(max.undo_move)) {
      max = new;
    }
  }

  return max;
}

static CollisionResult check_platform_collisions(Rectangle ball) {
  Rectangle platform_rect = {
      .x = platform_pos.x - (cur_platform_width / 2.0),
      .y = platform_pos.y - (PLATFORM_HEIGHT / 2.0),
      .width = cur_platform_width,
      .height = PLATFORM_HEIGHT,
  };

  CollisionResult result = check_overlap(ball, platform_rect);
  if (result.new_dir.y < 0.0) {
    float platform_center_x = platform_pos.x;
    float ball_center_x = ball_pos.x;
    float diff = ball_center_x - platform_center_x;
    float ratio = diff / (cur_platform_width / 2.0);
    float x_dir = ratio * PLATFORM_EDGE_X_MOV;
    float y_dir = sqrtf(1.0 - x_dir * x_dir) * result.new_dir.y < 0.0 ? -1 : 1;
    result.new_dir = (Vector2){.x = x_dir, .y = y_dir};
  }

  return result;
}

static void update_ball() {
  if (!ball_active) {
    if (balls_left == 0) {
      transition_state(END);
      return;
    }
    ball_pos = (Vector2){
        .x = platform_pos.x,
        .y = platform_pos.y - (PLATFORM_HEIGHT / 2.0) - (BALL_SIZE / 2.0) - 2,
    };
    if (IsKeyDown(KEY_W) | IsKeyDown(KEY_UP)) {
      ball_active = true;
      ball_dir = Vector2Normalize((Vector2){
          .x = platform_x_vel,
          .y = -sqrtf(1.0 - (platform_x_vel * platform_x_vel)),
      });
      balls_left--;
    } else {
      ball_dir = (Vector2){
          .x = 0,
          .y = 0,
      };
    }
    return;
  }

  float frame_time = GetFrameTime() * 1000;
  float distance_to_move =
      frame_time * (ball_speed == SLOW ? SLOW_BALL_MOVMENT_PER_MS
                                       : FAST_BALL_MOVMENT_PER_MS);
  while (distance_to_move > 0.005) {
    Vector2 ball_movement = Vector2Scale(ball_dir, distance_to_move);

    ball_pos = Vector2Add(ball_pos, ball_movement);
    Rectangle ball = {
        .x = ball_pos.x - (BALL_SIZE / 2.0),
        .y = ball_pos.y - (BALL_SIZE / 2.0),
        .width = BALL_SIZE,
        .height = BALL_SIZE,
    };
    size_t overlap_row = -1;
    size_t overlap_col = -1;
    CollisionResult block_overlap =
        check_block_collisions(ball, &overlap_row, &overlap_col);
    float block_overlap_amount = Vector2LengthSqr(block_overlap.undo_move);

    CollisionResult wall_overlap = check_wall_collisions(ball);
    float wall_overlap_amount = Vector2LengthSqr(wall_overlap.undo_move);

    CollisionResult platform_overlap = check_platform_collisions(ball);
    float platform_overlap_amount =
        Vector2LengthSqr(platform_overlap.undo_move);

    CollisionResult largest_overlap =
        block_overlap_amount > wall_overlap_amount &&
                block_overlap_amount > platform_overlap_amount
            ? block_overlap
        : wall_overlap_amount > platform_overlap_amount ? wall_overlap
                                                        : platform_overlap;

    if (Vector2LengthSqr(largest_overlap.undo_move) == 0.0) {
      break;
    }

    if (Vector2Equals(largest_overlap.undo_move, block_overlap.undo_move)) {
      assert(overlap_row != -1 && overlap_col != -1);
      assert(grid[overlap_row][overlap_col] == PRESENT);
      grid[overlap_row][overlap_col] = BROKEN;
      score += ROW_TO_POINTS[overlap_row];
      // TODO check for empty grid and move to next level
    } else if (Vector2Equals(largest_overlap.undo_move,
                             wall_overlap.undo_move)) {
    } else if (Vector2Equals(largest_overlap.undo_move,
                             platform_overlap.undo_move)) {
    }

    ball_pos = Vector2Add(ball_pos, largest_overlap.undo_move);
    distance_to_move -=
        Vector2Length(Vector2Add(ball_movement, largest_overlap.undo_move));

    ball_dir = largest_overlap.new_dir;
  }

  if (ball_pos.y > DEATH_LINE_Y) {
    ball_active = false;
    printf("KILLED_BALL\n");
  }
}

void update_game(void) {
  update_platform();
  update_ball();
}

void draw_board_info(void) {
  for (int i = 0; i < BALL_CHAR_MAX_LEN; i++) {
    if (i < balls_left) {
      balls_left_str[BALL_CHAR_START_INDEX + i] = BALL_CHAR;
    } else {
      balls_left_str[BALL_CHAR_START_INDEX + i] = '\0';
    }
  }
  DrawText(balls_left_str, 20, 50, 20, COLOR_TEXT);

  snprintf(score_str, ARRAY_LEN(score_str), "SCORE: %zu", score);
  DrawText(score_str, 20, 100, 20, COLOR_TEXT);
}

void draw_game(void) {
  DrawRectangleRec(
      (Rectangle){
          .x = GAME_START,
          .y = (WINDOW_HEIGHT - GAME_HEIGHT) / 2.0,
          .width = GAME_WIDTH,
          .height = GAME_HEIGHT,
      },
      COLOR_MENU_BG);
  Rectangle platform_rect = {
      .x = platform_pos.x - (cur_platform_width / 2.0),
      .y = platform_pos.y - (PLATFORM_HEIGHT / 2.0),
      .width = cur_platform_width,
      .height = PLATFORM_HEIGHT,
  };

  DrawRectangleRec(platform_rect, COLOR_PLATFORM);
  Rectangle ball_rect = {
      .x = ball_pos.x - (BALL_SIZE / 2.0),
      .y = ball_pos.y - (BALL_SIZE / 2.0),
      .width = BALL_SIZE,
      .height = BALL_SIZE,
  };

  DrawRectangleRec(ball_rect, COLOR_BALL);
  for (size_t row = 0; row < GRID_HEIGHT; row++) {
    for (size_t col = 0; col < GRID_WIDTH; col++) {
      if (grid[row][col] == PRESENT) {
        Rectangle block_rect = {
            .x = GAME_START + BLOCK_MARGIN + (BLOCK_MARGIN + BLOCK_WIDTH) * col,
            .y = GRID_START_Y + (BLOCK_MARGIN + BLOCK_HEIGHT) * row,
            .width = BLOCK_WIDTH,
            .height = BLOCK_HEIGHT,
        };

        DrawRectangleRec(block_rect,
                         BLOCK_COLORS[row / BLOCK_COLOR_GROUP_SIZE]);
      }
    }
  }
  draw_board_info();
}
