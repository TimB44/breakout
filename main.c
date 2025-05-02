#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

const size_t SREEN_HIGHT = 600;
const size_t SCREEN_WIDTH = 800;
const size_t PLATFORM_SIZE = 100;
const size_t BLOCK_SIZE = 25;
const size_t GRID_WIDTH = 20;
const size_t GRID_HEIGHT = 20;

typedef enum GameState {
  HOME,
  GAME,
  END,
} GameState;

GameState current_state = HOME;

void transition_state(GameState gs) {}

int main(void) {
  InitWindow(SCREEN_WIDTH, SREEN_HIGHT, "Pong Game");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (current_state) {

    case HOME:
      break;
    case GAME:
      break;
    case END:
      break;
    default:
      assert(false);
    }
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Hello Pong!", 350, 280, 20, RAYWHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
