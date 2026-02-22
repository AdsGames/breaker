#include "./Menu.h"

#include "../globals.h"

// INIT
void Menu::init() {
  // Sets main menu
  create_object<asw::game::Sprite>()->set_texture(
      asw::assets::load_texture("assets/images/mainmenu.png"));

  // Sets Cursors
  cursor[0] = asw::assets::load_texture("assets/images/cursor1.png");
  cursor[1] = asw::assets::load_texture("assets/images/cursor2.png");

  // Sets help
  menu_help = create_object<asw::game::Sprite>();
  menu_help->set_texture(asw::assets::load_texture("assets/images/help.png"));
  menu_help->transform.position = asw::Vec2<float>(36, 98);

  // Sets the high score table image
  high_scores_table = create_object<asw::game::Sprite>();
  high_scores_table->set_texture(
      asw::assets::load_texture("assets/images/highScoresTable.png"));
  high_scores_table->transform.position = asw::Vec2<float>(318, 100);

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

  // Sets button positions
  btn_start = create_object<Button>();
  btn_start->setPosition({380, 240})
      .setImages("assets/images/buttons/start.png",
                 "assets/images/buttons/start_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_DIFFICULTY;
        asw::sound::play(button_select);
      });

  btn_scores = create_object<Button>();
  btn_scores->setPosition({380, 380})
      .setImages("assets/images/buttons/highscores.png",
                 "assets/images/buttons/highscores_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_SCORES;
        asw::sound::play(button_select);
      });

  btn_help = create_object<Button>();
  btn_help->setPosition({380, 520})
      .setImages("assets/images/buttons/help.png",
                 "assets/images/buttons/help_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_HELP;
        asw::sound::play(button_select);
      });

  btn_quit = create_object<Button>();
  btn_quit->setPosition({380, 660})
      .setImages("assets/images/buttons/quit.png",
                 "assets/images/buttons/quit_hover.png")
      .setOnClick([this]() {
        asw::sound::play(button_select);
        asw::core::exit = true;
      });

  btn_easy = create_object<Button>();
  btn_easy->setPosition({380, 240})
      .setImages("assets/images/buttons/start_easy.png",
                 "assets/images/buttons/start_easy_hover.png")
      .setOnClick([this]() {
        difficulty = 3;
        manager.set_next_scene(States::Game);
        asw::sound::play(button_select);
      });

  btn_medium = create_object<Button>();
  btn_medium->setPosition({380, 380})
      .setImages("assets/images/buttons/start_medium.png",
                 "assets/images/buttons/start_medium_hover.png")
      .setOnClick([this]() {
        difficulty = 4;
        manager.set_next_scene(States::Game);
        asw::sound::play(button_select);
      });

  btn_hard = create_object<Button>();
  btn_hard->setPosition({380, 520})
      .setImages("assets/images/buttons/start_hard.png",
                 "assets/images/buttons/start_hard_hover.png")
      .setOnClick([this]() {
        difficulty = 5;
        manager.set_next_scene(States::Game);
        asw::sound::play(button_select);
      });

  btn_back = create_object<Button>();
  btn_back->setPosition({380, 800})
      .setImages("assets/images/buttons/back.png",
                 "assets/images/buttons/back_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_MAIN;
        asw::sound::play(button_select);
      });

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

  btn_start->active = menu_state == MENU_MAIN;
  btn_help->active = menu_state == MENU_MAIN;
  btn_quit->active = menu_state == MENU_MAIN;
  btn_scores->active = menu_state == MENU_MAIN;
  btn_easy->active = menu_state == MENU_DIFFICULTY;
  btn_medium->active = menu_state == MENU_DIFFICULTY;
  btn_hard->active = menu_state == MENU_DIFFICULTY;
  btn_back->active = menu_state == MENU_DIFFICULTY;
  trans_overlay->active = menu_state == MENU_HELP || menu_state == MENU_SCORES;
  menu_help->active = menu_state == MENU_HELP;
  high_scores_table->active = menu_state == MENU_SCORES;
}

// Draw
void Menu::draw() {
  Scene::draw();

  // Main menu
  if (menu_state == MENU_SCORES) {
    for (int i = 0; i < 10; i++) {
      asw::draw::text(font, highscores.getName(i),
                      asw::Vec2<float>(400, (i * 50) + 260),
                      asw::Color(0, 0, 0));

      asw::draw::text(font, std::to_string(highscores.getScore(i)),
                      asw::Vec2<float>(860, (i * 50) + 260),
                      asw::Color(0, 0, 0));
    }
  }

  // Draws Cursor
  if (asw::input::get_mouse_button(asw::input::MouseButton::Left)) {
    asw::draw::sprite(cursor[1], asw::input::mouse.position);
  } else {
    asw::draw::sprite(cursor[0], asw::input::mouse.position);
  }
}
