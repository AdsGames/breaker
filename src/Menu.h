/**
 * MENU
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef MENU_H
#define MENU_H

#include <asw/asw.h>

#include "ScoreManager.h"
#include "State.h"
#include "ui/Button.h"

class Menu : public State {
 public:
  using State::State;

  void init() override;
  void update() override;
  void draw() override;
  void cleanup() override {
    // Nothing to do
  }

 private:
  // Score table
  ScoreManager highscores;

  // Manu state
  char menu_state;

  // States
  enum menu_states { MENU_MAIN, MENU_DIFFICULTY, MENU_SCORES, MENU_HELP };

  // Init buttons
  Button btn_start, btn_easy, btn_medium, btn_hard, btn_back, btn_help,
      btn_quit, btn_scores;

  // Images
  asw::Texture mainmenu;
  asw::Texture menuHelp;
  asw::Texture highScoresTable;
  asw::Texture cursor[2];
  asw::Texture trans_overlay;

  // Button sounds
  asw::Sample button_hover;
  asw::Sample button_select;

  // Fonts
  asw::Font font;
};

#endif  // INIT_H
