#include "./Menu.h"

#include <array>
#include <format>

#include "../globals.h"
#include "../ui/ImageButton.h"
#include "../ui/Style.h"
#include "./Game.h"

// INIT
void Menu::init() {
  // Sets main menu
  create_object<asw::game::Sprite>()->set_texture(
      asw::assets::load_texture("assets/images/mainmenu.png"));

  // Sets Cursors
  cursor[0] = asw::assets::load_texture("assets/images/cursor1.png");
  cursor[1] = asw::assets::load_texture("assets/images/cursor2.png");

  // Trans overlay
  trans_overlay = create_object<asw::game::Sprite>();
  trans_overlay->set_texture(
      asw::assets::load_texture("assets/images/overlay.png"));

  // Give score files
  highscores = ScoreManager("scores.dat");

  // Samples
  button_hover = asw::assets::load_sample("assets/sounds/button_click.wav");
  button_select = asw::assets::load_sample("assets/sounds/button_select.wav");

  // Sets Font
  font = asw::assets::load_font("assets/fonts/ariblk.ttf", 24);
  font_title = asw::assets::load_font("assets/fonts/ariblk.ttf", 64);
  font_help = asw::assets::load_font("assets/fonts/ariblk.ttf", 30);

  // Buttons play the select sound when clicked
  ui.set_size(1280, 960);
  applyTheme(ui.ctx.theme, font);
  ui.ctx.theme.sound_activate = button_select;

  btn_start = &addImageButton(ui.root, {380, 240},
                              "assets/images/buttons/start.png",
                              "assets/images/buttons/start_hover.png",
                              [this]() { menu_state = MENU_DIFFICULTY; });

  btn_scores = &addImageButton(ui.root, {380, 380},
                               "assets/images/buttons/highscores.png",
                               "assets/images/buttons/highscores_hover.png",
                               [this]() { menu_state = MENU_SCORES; });

  btn_help = &addImageButton(ui.root, {380, 520},
                             "assets/images/buttons/help.png",
                             "assets/images/buttons/help_hover.png",
                             [this]() { menu_state = MENU_HELP; });

  btn_quit = &addImageButton(
      ui.root, {380, 660}, "assets/images/buttons/quit.png",
      "assets/images/buttons/quit_hover.png", []() { asw::core::exit(); });

  btn_easy = &addImageButton(ui.root, {380, 240},
                             "assets/images/buttons/start_easy.png",
                             "assets/images/buttons/start_easy_hover.png",
                             [this]() {
                               difficulty = 3;
                               manager.set_next_scene(States::Game);
                             });

  btn_medium = &addImageButton(ui.root, {380, 380},
                               "assets/images/buttons/start_medium.png",
                               "assets/images/buttons/start_medium_hover.png",
                               [this]() {
                                 difficulty = 4;
                                 manager.set_next_scene(States::Game);
                               });

  btn_hard = &addImageButton(ui.root, {380, 520},
                             "assets/images/buttons/start_hard.png",
                             "assets/images/buttons/start_hard_hover.png",
                             [this]() {
                               difficulty = 5;
                               manager.set_next_scene(States::Game);
                             });

  btn_back = &addImageButton(ui.root, {380, 800},
                             "assets/images/buttons/back.png",
                             "assets/images/buttons/back_hover.png",
                             [this]() { menu_state = MENU_MAIN; });

  // High score table: name and score columns, one row per entry
  scores_panel = &addPanel(ui.root, asw::Quadf(318, 100, 635, 745));
  addLabel(*scores_panel, {635, 145}, "High Scores", font_title,
           asw::TextJustify::Center);

  auto& table = scores_panel->add_child<asw::ui::Grid>();
  table.transform = asw::Quadf(400, 260, 920, 500);
  table.columns = 2;
  table.gap = 0.0F;
  table.row_height = 50.0F;

  for (int i = 0; i < 10; i++) {
    addLabel(table, {0, 0}, highscores.getName(i));
    addLabel(table, {0, 0}, std::to_string(highscores.getScore(i)));
  }

  // Help: one label per line, empty strings leave a gap
  help_panel = &addPanel(ui.root, asw::Quadf(36, 98, 1201, 746));
  addLabel(*help_panel, {636, 118}, "Help", font_title,
           asw::TextJustify::Center);

  const std::array<std::string, 12> help_lines = {
      "Purpose:",
      "Clear the board by breaking groups of matching blocks.",
      "Bigger groups are worth far more points.",
      "",
      "Controls:",
      "Left click - Break a group of two or more blocks",
      "",
      "Scoring:",
      "A group of N blocks scores (N - 1) x (N - 1) points.",
      "Medium doubles all points, and Hard triples them.",
      "Clearing the whole board gives a big bonus.",
      std::format("Each block left at the end costs {} points.",
                  LEFTOVER_PENALTY),
  };

  for (std::size_t i = 0; i < help_lines.size(); i++) {
    addLabel(*help_panel, {66, 216 + (static_cast<float>(i) * 48)},
             help_lines[i], font_help);
  }

  // Menu state
  menu_state = MENU_MAIN;
}

// Update
void Menu::update(float dt) {
  // Menu
  if (menu_state == MENU_HELP || menu_state == MENU_SCORES) {
    if (asw::input::get_mouse_button_down(asw::input::MouseButton::Left)) {
      menu_state = MENU_MAIN;
    }
  }

  Scene::update(dt);

  btn_start->visible = menu_state == MENU_MAIN;
  btn_help->visible = menu_state == MENU_MAIN;
  btn_quit->visible = menu_state == MENU_MAIN;
  btn_scores->visible = menu_state == MENU_MAIN;
  btn_easy->visible = menu_state == MENU_DIFFICULTY;
  btn_medium->visible = menu_state == MENU_DIFFICULTY;
  btn_hard->visible = menu_state == MENU_DIFFICULTY;
  btn_back->visible = menu_state == MENU_DIFFICULTY;
  ui.update();
  trans_overlay->active = menu_state == MENU_HELP || menu_state == MENU_SCORES;
  help_panel->visible = menu_state == MENU_HELP;
  scores_panel->visible = menu_state == MENU_SCORES;
}

// Cleanup
void Menu::cleanup() {
  Scene::cleanup();

  // Buttons are freed on the next update
  ui.clear_focus();
  ui.root.clear_children();
}

// Draw
void Menu::draw() {
  Scene::draw();
  ui.draw();
  const auto& mouse = asw::input::get_mouse();

  // Draws Cursor
  if (asw::input::get_mouse_button(asw::input::MouseButton::Left)) {
    asw::draw::sprite(cursor[1], mouse.position);
  } else {
    asw::draw::sprite(cursor[0], mouse.position);
  }
}
