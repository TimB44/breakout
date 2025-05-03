#include "defs.h"
#include "raylib.h"

void draw_text_centered(const char *msg, size_t x, size_t y, size_t font_size) {
  size_t text_size = MeasureText(msg, font_size);
  DrawText(msg, x - text_size / 2, y - font_size / 2, font_size, COLOR_TEXT);
}

bool mouse_over(Rectangle rect) {
  size_t mx = GetMouseX();
  size_t my = GetMouseY();

  size_t low_x = rect.x;
  size_t low_y = rect.y;

  size_t high_x = rect.x + rect.width;
  size_t high_y = rect.y + rect.height;

  return !(mx < low_x || mx > high_x || my < low_y || my > high_y);
}
bool mouse_pressed_on(Rectangle rect) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse_over(rect);
}

Vector2 rect_center(Rectangle rect) {
  return (Vector2){.x = rect.x + rect.width / 2, .y = rect.y + rect.height / 2};
}
