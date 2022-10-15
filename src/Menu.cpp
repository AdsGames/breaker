#include "Menu.h"

#include "globals.h"
#include "utility/tools.h"

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
  btn_start = Button(380, 240);
  btn_scores = Button(380, 380);
  btn_help = Button(380, 520);
  btn_quit = Button(380, 660);

  btn_easy = Button(380, 240);
  btn_medium = Button(380, 380);
  btn_hard = Button(380, 520);
  btn_back = Button(380, 800);

  // Sets button images
  btn_start.setImages("assets/images/buttons/start.png",
                      "assets/images/buttons/start_hover.png");
  btn_scores.setImages("assets/images/buttons/highscores.png",
                       "assets/images/buttons/highscores_hover.png");
  btn_help.setImages("assets/images/buttons/help.png",
                     "assets/images/buttons/help_hover.png");
  btn_quit.setImages("assets/images/buttons/quit.png",
                     "assets/images/buttons/quit_hover.png");

  btn_easy.setImages("assets/images/buttons/start_easy.png",
                     "assets/images/buttons/start_easy_hover.png");
  btn_medium.setImages("assets/images/buttons/start_medium.png",
                       "assets/images/buttons/start_medium_hover.png");
  btn_hard.setImages("assets/images/buttons/start_hard.png",
                     "assets/images/buttons/start_hard_hover.png");
  btn_back.setImages("assets/images/buttons/back.png",
                     "assets/images/buttons/back_hover.png");

  // On clicks
  // Start
  btn_start.setOnClick([this]() {
    menu_state = MENU_DIFFICULTY;
    asw::sound::play(button_select);
  });

  // View high scores
  btn_scores.setOnClick([this]() {
    menu_state = MENU_SCORES;
    asw::sound::play(button_select);
  });

  // Start game
  btn_help.setOnClick([this]() {
    menu_state = MENU_HELP;
    asw::sound::play(button_select);
  });

  // Exit
  btn_quit.setOnClick([this]() {
    setNextState(ProgramState::STATE_EXIT);
    asw::sound::play(button_select);
  });

  // Easy
  btn_easy.setOnClick([this]() {
    difficulty = 3;
    setNextState(ProgramState::STATE_GAME);
    asw::sound::play(button_select);
  });

  // Medium
  btn_medium.setOnClick([this]() {
    difficulty = 4;
    setNextState(ProgramState::STATE_GAME);
    asw::sound::play(button_select);
  });

  // Hard
  btn_hard.setOnClick([this]() {
    difficulty = 5;
    setNextState(ProgramState::STATE_GAME);
    asw::sound::play(button_select);
  });

  // Back
  btn_back.setOnClick([this]() {
    menu_state = MENU_MAIN;
    asw::sound::play(button_select);
  });

  // Menu state
  menu_state = MENU_MAIN;
}

// Update
void Menu::update() {
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
    if (asw::input::mouse.pressed[1] || asw::input::mouse.anyPressed ||
        asw::input::keyboard.anyPressed) {
      menu_state = MENU_MAIN;
    }
  }
}

// Draw
void Menu::draw() {
  // Draws menu
  asw::draw::sprite(mainmenu, 0, 0);

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
    asw::draw::sprite(trans_overlay, 0, 0);
    asw::draw::sprite(menuHelp, 36, 98);
  } else if (menu_state == MENU_SCORES) {
    asw::draw::sprite(trans_overlay, 0, 0);

    asw::draw::sprite(highScoresTable, 318, 100);

    for (int i = 0; i < 10; i++) {
      asw::draw::text(font, highscores.getName(i), 400, (i * 50) + 260,
                      asw::util::makeColor(0, 0, 0));
      asw::draw::text(font, std::to_string(highscores.getScore(i)), 860,
                      (i * 50) + 260, asw::util::makeColor(0, 0, 0));
    }
  }

  // Draws Cursor
  if (asw::input::mouse.down[1]) {
    asw::draw::sprite(cursor[1], asw::input::mouse.x, asw::input::mouse.y);
  } else {
    asw::draw::sprite(cursor[0], asw::input::mouse.x, asw::input::mouse.y);
  }
}
