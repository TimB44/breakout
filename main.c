#include "defs.h"

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HIGHT, "Pong Game");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    update_game();
    draw_game();
  }

  CloseWindow();
  return 0;
}

void update_game(void) {
  switch (current_state) {
  case GAME:
    // TOOD:
    break;
  case HOME:
  case END:
    break;
  default:
    assert(false);
  }
}
void draw_game(void) {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Hello Pong!", 350, 280, 20, RAYWHITE);
  EndDrawing();
}
