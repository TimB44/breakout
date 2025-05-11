#include "defs.h"

#define GAME_OVER_X 100
#define GAME_OVER_Y 350
#define GAME_OVER_FONT 20
#define GAME_OVER_STR "GAME OVER"

#define PLAY_AGAIN_STR "PLAY AGAIN"

static const Rectangle PLAY_AGAIN_BTN = {
    .x = 25,
    .y = 400,
    .width = 150,
    .height = 50,
};

static const Rectangle PLAY_AGAIN_BTN_BORDER = {
    .x = 22,
    .y = 397,
    .width = 156,
    .height = 56,
};

static bool play_again_btn_hovered = false;

void update_end(void) {
  int x = GetMouseX();
  int y = GetMouseX();
  play_again_btn_hovered = mouse_over(PLAY_AGAIN_BTN);
  if (mouse_pressed_on(PLAY_AGAIN_BTN)) {
    transition_state(GAME);
  }
}

void draw_end(void) {
  draw_game();
  draw_text_centered(GAME_OVER_STR, GAME_OVER_X, GAME_OVER_Y, GAME_OVER_FONT);

  DrawRectangleRounded(PLAY_AGAIN_BTN_BORDER, 0.2, 50, COLOR_MENU_BORDER);
  DrawRectangleRounded(PLAY_AGAIN_BTN, 0.2, 50,
                       play_again_btn_hovered ? COLOR_BUTTON_HOVER
                                              : COLOR_BUTTON);
  draw_text_centered(PLAY_AGAIN_STR,
                     PLAY_AGAIN_BTN.x + PLAY_AGAIN_BTN.width / 2,
                     PLAY_AGAIN_BTN.y + PLAY_AGAIN_BTN.height / 2, 15);
}
