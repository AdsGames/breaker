/**
 * MENU
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef MENU_H
#define MENU_H

#include <asw/asw.h>
#include <array>
#include <memory>

#include "../ScoreManager.h"
#include "../ui/Button.h"
#include "./States.h"

class Menu : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float deltaTime) override;

  void draw() override;

 private:
  // Score table
  ScoreManager highscores;

  // Manu state
  char menu_state;

  // States
  enum menu_states { MENU_MAIN, MENU_DIFFICULTY, MENU_SCORES, MENU_HELP };

  // Init buttons
  std::shared_ptr<Button> btn_start;
  std::shared_ptr<Button> btn_easy;
  std::shared_ptr<Button> btn_medium;
  std::shared_ptr<Button> btn_hard;
  std::shared_ptr<Button> btn_back;
  std::shared_ptr<Button> btn_help;
  std::shared_ptr<Button> btn_quit;
  std::shared_ptr<Button> btn_scores;

  // Images
  std::array<asw::Texture, 2> cursor;
  std::shared_ptr<asw::game::Sprite> menu_help;
  std::shared_ptr<asw::game::Sprite> high_scores_table;
  std::shared_ptr<asw::game::Sprite> trans_overlay;

  // Button sounds
  asw::Sample button_hover;
  asw::Sample button_select;

  // Fonts
  asw::Font font;
};

#endif  // INIT_H
