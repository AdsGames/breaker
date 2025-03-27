#include "./Menu.h"

#include "../globals.h"

// INIT
void Menu::init() {
  // Sets Cursors
  cursor[0] = asw::assets::loadTexture("assets/images/cursor1.png");
  cursor[1] = asw::assets::loadTexture("assets/images/cursor2.png");

  // Sets help
  menuHelp = asw::assets::loadTexture("assets/images/help.png");

  // Sets main menu
  mainmenu = asw::assets::loadTexture("assets/images/mainmenu.png");

  // Sets the high score table image
  highScoresTable =
      asw::assets::loadTexture("assets/images/highScoresTable.png");

  // Trans overlay
  trans_overlay = asw::assets::loadTexture("assets/images/overlay.png");

  // Give score files
  highscores = ScoreManager("scores.dat");

  // Samples
  button_hover = asw::assets::loadSample("assets/sounds/button_click.wav");
  button_select = asw::assets::loadSample("assets/sounds/button_select.wav");

  // Sets Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  // Sets button positions
  btn_start = Button()
                  .setPosition({380, 240})
                  .setImages("assets/images/buttons/start.png",
                             "assets/images/buttons/start_hover.png")
                  .setOnClick([this]() {
                    menu_state = MENU_DIFFICULTY;
                    asw::sound::play(button_select);
                  });

  btn_scores = Button()
                   .setPosition({380, 380})
                   .setImages("assets/images/buttons/highscores.png",
                              "assets/images/buttons/highscores_hover.png")
                   .setOnClick([this]() {
                     menu_state = MENU_SCORES;
                     asw::sound::play(button_select);
                   });

  btn_help = Button()
                 .setPosition({380, 520})
                 .setImages("assets/images/buttons/help.png",
                            "assets/images/buttons/help_hover.png")
                 .setOnClick([this]() {
                   menu_state = MENU_HELP;
                   asw::sound::play(button_select);
                 });

  btn_quit = Button()
                 .setPosition({380, 660})
                 .setImages("assets/images/buttons/quit.png",
                            "assets/images/buttons/quit_hover.png")
                 .setOnClick([this]() {
                   asw::sound::play(button_select);
                   asw::core::exit = true;
                 });

  btn_easy = Button()
                 .setPosition({380, 240})
                 .setImages("assets/images/buttons/start_easy.png",
                            "assets/images/buttons/start_easy_hover.png")
                 .setOnClick([this]() {
                   difficulty = 3;
                   sceneManager.setNextScene(States::Game);
                   asw::sound::play(button_select);
                 });

  btn_medium = Button()
                   .setPosition({380, 380})
                   .setImages("assets/images/buttons/start_medium.png",
                              "assets/images/buttons/start_medium_hover.png")
                   .setOnClick([this]() {
                     difficulty = 4;
                     sceneManager.setNextScene(States::Game);
                     asw::sound::play(button_select);
                   });

  btn_hard = Button()
                 .setPosition({380, 520})
                 .setImages("assets/images/buttons/start_hard.png",
                            "assets/images/buttons/start_hard_hover.png")
                 .setOnClick([this]() {
                   difficulty = 5;
                   sceneManager.setNextScene(States::Game);
                   asw::sound::play(button_select);
                 });

  btn_back = Button()
                 .setPosition({380, 800})
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
void Menu::update(float _deltaTime) {
  // Menu
  if (menu_state == MENU_MAIN) {
    btn_start.update();
    btn_scores.update();
    btn_help.update();
    btn_quit.update();
  } else if (menu_state == MENU_DIFFICULTY) {
    btn_back.update();
    btn_easy.update();
    btn_medium.update();
    btn_hard.update();
  } else if (menu_state == MENU_HELP || menu_state == MENU_SCORES) {
    if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT) ||
        asw::input::mouse.anyPressed || asw::input::keyboard.anyPressed) {
      menu_state = MENU_MAIN;
    }
  }
}

// Draw
void Menu::draw() {
  // Draws menu
  asw::draw::sprite(mainmenu, asw::Vec2<float>(0, 0));

  // Main menu
  if (menu_state == MENU_MAIN) {
    btn_start.draw();
    btn_help.draw();
    btn_quit.draw();
    btn_scores.draw();
  } else if (menu_state == MENU_DIFFICULTY) {
    btn_easy.draw();
    btn_medium.draw();
    btn_hard.draw();
    btn_back.draw();
  } else if (menu_state == MENU_HELP) {
    asw::draw::sprite(trans_overlay, asw::Vec2<float>(0, 0));
    asw::draw::sprite(menuHelp, asw::Vec2<float>(36, 98));
  } else if (menu_state == MENU_SCORES) {
    asw::draw::sprite(trans_overlay, asw::Vec2<float>(0, 0));
    asw::draw::sprite(highScoresTable, asw::Vec2<float>(318, 100));

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
