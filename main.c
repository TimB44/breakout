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
  switch (current_state) {
  case HOME:
    update_home();
    break;
  case GAME:
    update_game();
    break;
  case END:
    // current_state = HOME;
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
    draw_home();
    break;
  case GAME:

    draw_game();
    break;
  case END:
    DrawText("END", 0, 0, 20, RAYWHITE);
    break;
  }
  EndDrawing();
}
void transition_state(GameState new_state) {
  switch (new_state) {
  case GAME:
    init_game();
    break;
  case HOME:
  case END:
    break;
  }
  current_state = new_state;
}
