#include "defs.h"
#include "raylib.h"

int main(void) {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

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
    update_end();
    break;
  default:
    assert(false);
  }
}

void draw(void) {
  BeginDrawing();
  ClearBackground(COLOR_BG);

  switch (current_state) {
  case HOME:
    draw_home();
    break;
  case GAME:
    draw_game();
    break;
  case END:
    draw_end();
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
