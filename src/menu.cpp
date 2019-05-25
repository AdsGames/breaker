#include "menu.h"

#include <loadpng.h>

#include "globals.h"

// INIT
menu::menu() {
  // Sets button images
  start_game.SetImages ("images/buttons/start.png", "images/buttons/start_hover.png");
  start_easy.SetImages ("images/buttons/start_easy.png", "images/buttons/start_easy_hover.png");
  start_medium.SetImages ("images/buttons/start_medium.png", "images/buttons/start_medium_hover.png");
  start_hard.SetImages ("images/buttons/start_hard.png", "images/buttons/start_hard_hover.png");
  back.SetImages ("images/buttons/back.png", "images/buttons/back_hover.png");
  help.SetImages ("images/buttons/help.png", "images/buttons/help_hover.png");
  quit.SetImages ("images/buttons/quit.png", "images/buttons/quit_hover.png");
  viewHighScores.SetImages ("images/buttons/highscores.png", "images/buttons/highscores_hover.png");

  // Sets button positions
  start_game.SetPosition (380, 240);
  viewHighScores.SetPosition (380, 380);
  help.SetPosition (380, 520);
  quit.SetPosition (380, 660);

  start_easy.SetPosition (380, 240);
  start_medium.SetPosition (380, 380);
  start_hard.SetPosition (380, 520);
  back.SetPosition (380, 800);

  // Sets Cursors
  cursor[0] = load_png ("images/cursor1.png", NULL);
  cursor[1] = load_png ("images/cursor2.png", NULL);

  // Creates a buffer
  buffer = create_bitmap (1280, 960);

  // Sets help
  menuHelp = load_png ("images/help.png", NULL);

  // Sets main menu
  mainmenu = load_png ("images/mainmenu.png", NULL);

  // Sets the high score table image
  highScoresTable = load_png ("images/highScoresTable.png", NULL);

  // Trans overlay
  trans_overlay = load_png ("images/overlay.png", NULL);

  if (config_sound) {
    // Samples
    button_hover = load_sample ("sounds/button_click.wav");
    button_select = load_sample ("sounds/button_select.wav");

    // Sounds
    start_game.SetSamples (button_hover, button_select);
    viewHighScores.SetSamples (button_hover, button_select);
    help.SetSamples (button_hover, button_select);
    quit.SetSamples (button_hover, button_select);
    start_easy.SetSamples (button_hover, button_select);
    start_medium.SetSamples (button_hover, button_select);
    start_hard.SetSamples (button_hover, button_select);
    back.SetSamples (button_hover, button_select);
  }

  // Give score files
  highscores = ScoreManager("scores.dat");

  viewHelp = false;
  viewScores = false;
  viewDifficulty = false;
}

// DESTORY
menu::~menu() {
  destroy_bitmap (buffer);
  destroy_bitmap (mainmenu);
  destroy_bitmap (menuHelp);
  destroy_bitmap (highScoresTable);
  destroy_bitmap (cursor[0]);
  destroy_bitmap (cursor[1]);
  destroy_bitmap (trans_overlay);

  destroy_sample (button_hover);
  destroy_sample (button_select);
}

// Update
void menu::update() {
  // Menu
  // Checks for mouse press
  if (mouseListener::mouse_pressed & 2) {
    set_next_state (STATE_MENU);
  }

  if (mouseListener::mouse_pressed & 1) {
    // Main menu
    if (!viewDifficulty) {
      // Help if necessary
      if (viewHelp) {
        viewHelp = false;
      }
      // Scores if necessary
      else if (viewScores) {
        viewScores = false;
      }
      // Buttons
      else if (start_game.Hover()) {
        viewDifficulty = true;
        start_game.Select();
      }
      else if (viewHighScores.Hover()) {
        viewScores = true;
        viewHighScores.Select();
      }
      else if (help.Hover()) {
        viewHelp = true;
        help.Select();
      }
      else if (quit.Hover()) {
        set_next_state (STATE_EXIT);
        quit.Select();
      }
    }

    // Difficulty Select
    else {
      if (start_easy.Hover()) {
        difficulty = 3;
        set_next_state (STATE_GAME);
        start_easy.Select();
      }
      else if (start_medium.Hover()) {
        difficulty = 4;
        set_next_state (STATE_GAME);
        start_medium.Select();
      }
      else if (start_hard.Hover()) {
        difficulty = 5;
        set_next_state (STATE_GAME);
        start_hard.Select();
      }
      else if (back.Hover()) {
        viewDifficulty = false;
        back.Select();
      }
    }
  }
}

// Draw
void menu::draw() {
  // Draws menu
  draw_sprite (buffer, mainmenu, 0, 0);

  // Main menu
  if (!viewDifficulty) {
    // Draws Buttons
    start_game.draw (buffer);
    help.draw (buffer);
    quit.draw (buffer);
    viewHighScores.draw (buffer);

    // Draw help if necessary
    if (viewHelp) {
      set_alpha_blender();
      draw_trans_sprite (buffer, trans_overlay, 0, 0);
      draw_sprite (buffer, menuHelp, 36, 98);
    }

    // Draw scores if necessary
    if (viewScores) {
      set_alpha_blender();
      draw_trans_sprite (buffer, trans_overlay, 0, 0);

      draw_sprite (buffer, highScoresTable, 318, 100);

      for (int i = 0; i < 10; i++) {
        textout_ex (buffer, font, highscores.getName (i).c_str(), 400, (i * 50) + 260, makecol (0, 0, 0), -1);
        textprintf_right_ex (buffer, font, 860, (i * 50) + 260, makecol (0, 0, 0), -1, "%d", highscores.getScore (i));
      }
    }
  }
  else {
    // Draws Buttons
    start_easy.draw (buffer);
    start_medium.draw (buffer);
    start_hard.draw (buffer);
    back.draw (buffer);
  }

  // Draws Cursor
  draw_sprite (buffer, cursor[ (mouse_b & 1)], mouseListener::res_mouse_x, mouseListener::res_mouse_y);

  // Draw buffer
  stretch_sprite (screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}
