#include "defs.h"

#include "raylib.h"

int main(void) {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    update();
    draw();
  }

  CloseWindow();
  return 0;
}

void update(void) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    switch (current_state) {
    case HOME:
      current_state = GAME;
      break;
    case GAME:
      current_state = END;
      break;
    case END:
      current_state = HOME;
      break;
    default:
      assert(false);
    }
}

void draw(void) {
  BeginDrawing();
  ClearBackground(BLACK);

  switch (current_state) {
  case HOME:
    DrawText("HOME", 0, 0, 20, RAYWHITE);
    break;
  case GAME:
    DrawText("GAME", 0, 0, 20, RAYWHITE);
    break;
  case END:
    DrawText("END", 0, 0, 20, RAYWHITE);
    break;
  }
  EndDrawing();
}
