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
#include "./States.h"

class Menu : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float dt) override;

  void draw() override;

  void cleanup() override;

 private:
  // Score table
  ScoreManager highscores;

  // Manu state
  char menu_state;

  // States
  enum menu_states { MENU_MAIN, MENU_DIFFICULTY, MENU_SCORES, MENU_HELP };

  // Buttons
  asw::ui::Root ui;
  asw::ui::Button* btn_start{nullptr};
  asw::ui::Button* btn_easy{nullptr};
  asw::ui::Button* btn_medium{nullptr};
  asw::ui::Button* btn_hard{nullptr};
  asw::ui::Button* btn_back{nullptr};
  asw::ui::Button* btn_help{nullptr};
  asw::ui::Button* btn_quit{nullptr};
  asw::ui::Button* btn_scores{nullptr};

  // High score table and help
  asw::ui::Panel* scores_panel{nullptr};
  asw::ui::Panel* help_panel{nullptr};

  // Images
  std::array<asw::Texture, 2> cursor;
  std::shared_ptr<asw::game::Sprite> trans_overlay;

  // Button sounds
  asw::Sample button_hover;
  asw::Sample button_select;

  // Fonts
  asw::Font font;
  asw::Font font_title;
  asw::Font font_help;
};

#endif  // INIT_H
