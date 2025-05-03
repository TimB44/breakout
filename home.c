#include "defs.h"
#include "raylib.h"

static const float MENU_BORDER_WIDTH = 10;

static const Rectangle MENU = {.x = 200, .y = 100, .width = 400, .height = 400};

static const Rectangle MENU_BORDER = {
    .x = 197, .y = 97, .width = 406, .height = 406};

static const Rectangle START_BTN = {
    .x = 300, .y = 350, .width = 200, .height = 125};

static const Rectangle START_BTN_BORDER = {
    .x = 297, .y = 347, .width = 206, .height = 131};

static bool start_btn_hovered = false;

void update_home(void) {
  int x = GetMouseX();
  int y = GetMouseX();
  start_btn_hovered = mouse_over(START_BTN);
  if (mouse_pressed_on(START_BTN)) {
    transition_state(GAME);
  }
}
void draw_home(void) {
  DrawRectangleRounded(MENU_BORDER, 0.2, 50, COLOR_MENU_BORDER);
  DrawRectangleRounded(MENU, 0.2, 50, COLOR_MENU_BG);

  draw_text_centered("BREAKOUT", 400, 180, 30);
  draw_text_centered("CLICK START TO BEGIN", 400, 230, 15);

  DrawRectangleRounded(START_BTN_BORDER, MENU_ROUNDESS, 50, COLOR_MENU_BORDER);
  DrawRectangleRounded(START_BTN, MENU_ROUNDESS, 50,
                       start_btn_hovered ? COLOR_BUTTON_HOVER : COLOR_BUTTON);
  Vector2 center = rect_center(START_BTN);
  draw_text_centered("START", center.x, center.y, 20);
}
