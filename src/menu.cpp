#include "menu.h"

#include "globals.h"
#include "utility/tools.h"

// INIT
menu::menu() {
  // Sets Cursors
  cursor[0] = load_png_ex("images/cursor1.png");
  cursor[1] = load_png_ex("images/cursor2.png");

  // Creates a buffer
  buffer = create_bitmap(NATIVE_SCREEN_W, NATIVE_SCREEN_H);

  // Sets help
  menuHelp = load_png_ex("images/help.png");

  // Sets main menu
  mainmenu = load_png_ex("images/mainmenu.png");

  // Sets the high score table image
  highScoresTable = load_png_ex("images/highScoresTable.png");

  // Trans overlay
  trans_overlay = load_png_ex("images/overlay.png");

  // Give score files
  highscores = ScoreManager("scores.dat");

  // Samples
  button_hover = load_sample_ex("sounds/button_click.wav");
  button_select = load_sample_ex("sounds/button_select.wav");

  // Sets button positions
  btn_start = Button(380, 240);
  btn_scores = Button(380, 380);
  btn_help = Button(380, 520);
  btn_quit = Button(380, 660);

  btn_easy = Button(380, 240);
  btn_medium = Button(380, 380);
  btn_hard = Button(380, 520);
  btn_back = Button(380, 800);

  // Sets button images
  btn_start.SetImages("images/buttons/start.png",
                      "images/buttons/start_hover.png");
  btn_scores.SetImages("images/buttons/highscores.png",
                       "images/buttons/highscores_hover.png");
  btn_help.SetImages("images/buttons/help.png",
                     "images/buttons/help_hover.png");
  btn_quit.SetImages("images/buttons/quit.png",
                     "images/buttons/quit_hover.png");

  btn_easy.SetImages("images/buttons/start_easy.png",
                     "images/buttons/start_easy_hover.png");
  btn_medium.SetImages("images/buttons/start_medium.png",
                       "images/buttons/start_medium_hover.png");
  btn_hard.SetImages("images/buttons/start_hard.png",
                     "images/buttons/start_hard_hover.png");
  btn_back.SetImages("images/buttons/back.png",
                     "images/buttons/back_hover.png");

  // On clicks
  // Start
  btn_start.SetOnClick([this]() {
    menu_state = MENU_DIFFICULTY;

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // View high scores
  btn_scores.SetOnClick([this]() {
    menu_state = MENU_SCORES;

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Start game
  btn_help.SetOnClick([this]() {
    menu_state = MENU_HELP;

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Exit
  btn_quit.SetOnClick([this]() {
    set_next_state(STATE_EXIT);

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Easy
  btn_easy.SetOnClick([this]() {
    difficulty = 3;
    set_next_state(STATE_GAME);

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Medium
  btn_medium.SetOnClick([this]() {
    difficulty = 4;
    set_next_state(STATE_GAME);

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Hard
  btn_hard.SetOnClick([this]() {
    difficulty = 5;
    set_next_state(STATE_GAME);

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Back
  btn_back.SetOnClick([this]() {
    menu_state = MENU_MAIN;

    if (config_sound)
      play_sample(button_select, 255, 127, 1000, false);
  });

  // Menu state
  menu_state = MENU_MAIN;
}

// DESTORY
menu::~menu() {
  destroy_bitmap(buffer);
  destroy_bitmap(mainmenu);
  destroy_bitmap(menuHelp);
  destroy_bitmap(highScoresTable);
  destroy_bitmap(cursor[0]);
  destroy_bitmap(cursor[1]);
  destroy_bitmap(trans_overlay);

  destroy_sample(button_hover);
  destroy_sample(button_select);
}

// Update
void menu::update() {
  // Menu
  if (menu_state == MENU_MAIN) {
    btn_start.Update();
    btn_scores.Update();
    btn_help.Update();
    btn_quit.Update();
  } else if (menu_state == MENU_DIFFICULTY) {
    btn_back.Update();
    btn_easy.Update();
    btn_medium.Update();
    btn_hard.Update();
  } else if (menu_state == MENU_HELP || menu_state == MENU_SCORES) {
    if (MouseListener::mouse_pressed & 1 || button_down() || key_down()) {
      menu_state = MENU_MAIN;
    }
  }
}

// Draw
void menu::draw() {
  // Draws menu
  draw_sprite(buffer, mainmenu, 0, 0);

  // Main menu
  if (menu_state == MENU_MAIN) {
    btn_start.Draw(buffer);
    btn_help.Draw(buffer);
    btn_quit.Draw(buffer);
    btn_scores.Draw(buffer);
  } else if (menu_state == MENU_DIFFICULTY) {
    btn_easy.Draw(buffer);
    btn_medium.Draw(buffer);
    btn_hard.Draw(buffer);
    btn_back.Draw(buffer);
  } else if (menu_state == MENU_HELP) {
    set_alpha_blender();
    draw_trans_sprite(buffer, trans_overlay, 0, 0);
    draw_sprite(buffer, menuHelp, 36, 98);
  } else if (menu_state == MENU_SCORES) {
    set_alpha_blender();
    draw_trans_sprite(buffer, trans_overlay, 0, 0);

    draw_sprite(buffer, highScoresTable, 318, 100);

    for (int i = 0; i < 10; i++) {
      textout_ex(buffer, font, highscores.getName(i).c_str(), 400,
                 (i * 50) + 260, makecol(0, 0, 0), -1);
      textprintf_right_ex(buffer, font, 860, (i * 50) + 260, makecol(0, 0, 0),
                          -1, "%d", highscores.getScore(i));
    }
  }

  // Draws Cursor
  draw_sprite(buffer, cursor[(mouse_b & 1)], MouseListener::x,
              MouseListener::y);

  // Draw buffer
  stretch_sprite(screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}
