#include "./Menu.h"

#include "../globals.h"

// INIT
void Menu::init() {
  // Sets main menu
  createObject<asw::game::Sprite>()->setTexture(
      asw::assets::loadTexture("assets/images/mainmenu.png"));

  // Sets Cursors
  cursor[0] = asw::assets::loadTexture("assets/images/cursor1.png");
  cursor[1] = asw::assets::loadTexture("assets/images/cursor2.png");

  // Sets help
  menu_help = createObject<asw::game::Sprite>();
  menu_help->setTexture(asw::assets::loadTexture("assets/images/help.png"));
  menu_help->transform.position = asw::Vec2<float>(36, 98);

  // Sets the high score table image
  high_scores_table = createObject<asw::game::Sprite>();
  high_scores_table->setTexture(
      asw::assets::loadTexture("assets/images/highScoresTable.png"));
  high_scores_table->transform.position = asw::Vec2<float>(318, 100);

  // Trans overlay
  trans_overlay = createObject<asw::game::Sprite>();
  trans_overlay->setTexture(
      asw::assets::loadTexture("assets/images/overlay.png"));

  // Give score files
  highscores = ScoreManager("scores.dat");

  // Samples
  button_hover = asw::assets::loadSample("assets/sounds/button_click.wav");
  button_select = asw::assets::loadSample("assets/sounds/button_select.wav");

  // Sets Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  // Sets button positions
  btn_start = createObject<Button>();
  btn_start->setPosition({380, 240})
      .setImages("assets/images/buttons/start.png",
                 "assets/images/buttons/start_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_DIFFICULTY;
        asw::sound::play(button_select);
      });

  btn_scores = createObject<Button>();
  btn_scores->setPosition({380, 380})
      .setImages("assets/images/buttons/highscores.png",
                 "assets/images/buttons/highscores_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_SCORES;
        asw::sound::play(button_select);
      });

  btn_help = createObject<Button>();
  btn_help->setPosition({380, 520})
      .setImages("assets/images/buttons/help.png",
                 "assets/images/buttons/help_hover.png")
      .setOnClick([this]() {
        menu_state = MENU_HELP;
        asw::sound::play(button_select);
      });

  btn_quit = createObject<Button>();
  btn_quit->setPosition({380, 660})
      .setImages("assets/images/buttons/quit.png",
                 "assets/images/buttons/quit_hover.png")
      .setOnClick([this]() {
        asw::sound::play(button_select);
        asw::core::exit = true;
      });

  btn_easy = createObject<Button>();
  btn_easy->setPosition({380, 240})
      .setImages("assets/images/buttons/start_easy.png",
                 "assets/images/buttons/start_easy_hover.png")
      .setOnClick([this]() {
        difficulty = 3;
        sceneManager.setNextScene(States::Game);
        asw::sound::play(button_select);
      });

  btn_medium = createObject<Button>();
  btn_medium->setPosition({380, 380})
      .setImages("assets/images/buttons/start_medium.png",
                 "assets/images/buttons/start_medium_hover.png")
      .setOnClick([this]() {
        difficulty = 4;
        sceneManager.setNextScene(States::Game);
        asw::sound::play(button_select);
      });

  btn_hard = createObject<Button>();
  btn_hard->setPosition({380, 520})
      .setImages("assets/images/buttons/start_hard.png",
                 "assets/images/buttons/start_hard_hover.png")
      .setOnClick([this]() {
        difficulty = 5;
        sceneManager.setNextScene(States::Game);
        asw::sound::play(button_select);
      });

  btn_back = createObject<Button>();
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
void Menu::update(float deltaTime) {
  // Menu
  if (menu_state == MENU_HELP || menu_state == MENU_SCORES) {
    if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
      menu_state = MENU_MAIN;
    }
  }

  Scene::update(deltaTime);

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
                      asw::util::makeColor(0, 0, 0));

      asw::draw::text(font, std::to_string(highscores.getScore(i)),
                      asw::Vec2<float>(860, (i * 50) + 260),
                      asw::util::makeColor(0, 0, 0));
    }
  }

  // Draws Cursor
  if (asw::input::isButtonDown(asw::input::MouseButton::LEFT)) {
    asw::draw::sprite(
        cursor[1], asw::Vec2<float>(asw::input::mouse.x, asw::input::mouse.y));
  } else {
    asw::draw::sprite(
        cursor[0], asw::Vec2<float>(asw::input::mouse.x, asw::input::mouse.y));
  }
}
