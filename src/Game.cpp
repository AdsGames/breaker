#include "Game.h"

#include "globals.h"
#include "utility/tools.h"

// INIT
void Game::init() {
  // Sets Cursors
  cursor[0] = asw::assets::loadTexture("assets/images/cursor1.png");
  cursor[1] = asw::assets::loadTexture("assets/images/cursor2.png");

  // Sets background
  background = asw::assets::loadTexture("assets/images/background.png");

  // Sets Foreground
  foreground = asw::assets::loadTexture("assets/images/foreground.png");

  // Sets menu
  dialog_box = asw::assets::loadTexture("assets/images/menu.png");

  // Trans overlay
  trans_overlay = asw::assets::loadTexture("assets/images/overlay.png");

  // Sets Sounds
  block_break = asw::assets::loadSample("assets/sounds/break.wav");
  click = asw::assets::loadSample("assets/sounds/click.wav");

  // Sets Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  // Sets Variables
  score = 0;
  startAnimate = 1200;
  blocks_selected = 0;
  game_over = false;
  gameOverMessage = "Game Over";

  // Sets block info
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      MyBlocks[i][t] = Block(i * 80 + 80, t * 80 + 80, random(0, difficulty));
    }
  }

  // Give score files
  highscores = ScoreManager("scores.dat");

  // Buttons
  done = Button(500, 10);
  dialog_yes = Button(41 + 300, 215 + 300);
  dialog_no = Button(461 + 300, 215 + 300);

  // Images
  done.setImages("assets/images/buttons/done.png",
                 "assets/images/buttons/done_hover.png");
  dialog_yes.setImages("assets/images/buttons/yes.png",
                       "assets/images/buttons/yes_hover.png");
  dialog_no.setImages("assets/images/buttons/no.png",
                      "assets/images/buttons/no_hover.png");

  // On clicks
  done.setOnClick([this]() { game_over = true; });

  dialog_yes.setOnClick([this]() {
    highscores.add(ib_name.getValue(), score);
    setNextState(ProgramState::STATE_GAME);
  });

  dialog_no.setOnClick([this]() {
    highscores.add(ib_name.getValue(), score);
    setNextState(ProgramState::STATE_MENU);
  });

  ib_name = InputBox(488, 405, 404, 44, font, "Player");

  particles = ParticleEmitter(vec2(0, 0), vec2(64, 64));
}

// Deselect all blocks
void Game::deselectBlocks() {
  for (int i = 0; i < BLOCKS_WIDE; i++)
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      MyBlocks[i][t].setSelected(false);
    }
}

// Select group of blocks
int Game::selectBlock(int x, int y, int type = -1) {
  if (x < BLOCKS_WIDE && y < BLOCKS_HIGH && x >= 0 && y >= 0) {
    if ((type == -1 || MyBlocks[x][y].getType() == type) &&
        !MyBlocks[x][y].getSelected() && MyBlocks[x][y].getType() != 6) {
      // Select it
      MyBlocks[x][y].setSelected(true);

      // Select 4 around it
      int btype = MyBlocks[x][y].getType();
      return 1 + selectBlock(x - 1, y, btype) + selectBlock(x + 1, y, btype) +
             selectBlock(x, y - 1, btype) + selectBlock(x, y + 1, btype);
    }
  }

  return 0;
}

// Destroy selected
void Game::destroySelectedBlocks() {
  // Remove blocks
  int num_destroyed = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (MyBlocks[i][t].getSelected()) {
        MyBlocks[i][t].setSelected(false);
        MyBlocks[i][t].explode(particles);
        MyBlocks[i][t].setType(6);
        num_destroyed++;
      }
    }
  }

  asw::sound::play(block_break, 64 + num_destroyed * 5,
                   100);  // 900 + num_destroyed * 80

  // Settle blocks downwards
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    int num_blank = BLOCKS_HIGH - 1;

    for (int t = BLOCKS_HIGH - 1; t >= 0; t--)
      if (MyBlocks[i][t].getType() != 6) {
        MyBlocks[i][num_blank--].setType(MyBlocks[i][t].getType());
      }

    while (num_blank >= 0) {
      MyBlocks[i][num_blank--].setType(6);
    }
  }

  // Settle blocks across
  int num_back = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      MyBlocks[i - num_back][t].setType(MyBlocks[i][t].getType());
    }

    if (MyBlocks[i][BLOCKS_HIGH - 1].getType() == 6) {
      num_back++;
    }
  }

  while (num_back > 0) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      MyBlocks[BLOCKS_WIDE - num_back][t].setType(6);
    }

    num_back--;
  }
}

// Block at x y
Block* Game::blockAt(int x, int y) {
  if (x < BLOCKS_WIDE && y < BLOCKS_HIGH && x >= 0 && y >= 0) {
    return &MyBlocks[x][y];
  }

  return nullptr;
}

// Remaining blocks
int Game::countBlocks() {
  int blocks_left = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++)
    for (int t = 0; t < BLOCKS_HIGH; t++)
      if (MyBlocks[i][t].getType() != 6) {
        blocks_left++;
      }

  return blocks_left;
}

// Moves left
int Game::countRemainingMoves() {
  int matchesLeft = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++)
    for (int t = 0; t < BLOCKS_HIGH; t++)
      if (MyBlocks[i][t].getType() != 6)
        if ((i < 13 &&
             MyBlocks[i][t].getType() == MyBlocks[i + 1][t].getType()) ||
            (i > 0 &&
             MyBlocks[i][t].getType() == MyBlocks[i - 1][t].getType()) ||
            (t < 8 &&
             MyBlocks[i][t].getType() == MyBlocks[i][t + 1].getType()) ||
            (t > 0 &&
             MyBlocks[i][t].getType() == MyBlocks[i][t - 1].getType())) {
          matchesLeft++;
        }

  return matchesLeft;
}

// Block index
Game::coordinate Game::getBlockIndex(int screen_x, int screen_y) {
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (MyBlocks[i][t].getX() < screen_x &&
          MyBlocks[i][t].getY() < screen_y &&
          MyBlocks[i][t].getX() + MyBlocks[i][t].getWidth() > screen_x &&
          MyBlocks[i][t].getY() + MyBlocks[i][t].getHeight() > screen_y) {
        return coordinate(i, t);
      }
    }
  }

  return coordinate(-1, -1);
}

// Update
void Game::update() {
  // Animation for start of game
  if (startAnimate > 10) {
    startAnimate -= 10;
  } else {
    startAnimate = 0;

    if (!game_time.isRunning())
      game_time.start();
  }

  // In Game
  if (!game_over) {
    // Update particles
    particles.update(16);
    done.update();

    // Select blocks
    if (asw::input::mouse.pressed[1]) {
      coordinate selected_block =
          getBlockIndex(asw::input::mouse.x, asw::input::mouse.y);

      if (selected_block.x != -1) {  // Ensure existing block
        if (blocks_selected > 1 &&
            blockAt(selected_block.x, selected_block.y)->getSelected()) {
          destroySelectedBlocks();
          blocks_selected = 0;
        } else {
          deselectBlocks();
          blocks_selected = selectBlock(
              selected_block.x, selected_block.y);  // Select and count blocks

          if (!config_double_click && blocks_selected > 1 &&
              blockAt(selected_block.x, selected_block.y)->getSelected()) {
            destroySelectedBlocks();
            blocks_selected = 0;
          }
        }
      }

      // Quit
      if (done.isHovering()) {
        game_over = true;
        gameOverMessage = "Game Over";
      }
    }

    // Lose
    if (countRemainingMoves() == 0) {
      if (countBlocks() == 0) {
        gameOverMessage = "You Win!";
      } else {
        gameOverMessage = "Out of moves";
      }

      game_over = true;
    }
  }

  // Game over state
  else {
    if (game_time.isRunning())
      game_time.stop();

    // Set score
    if (score == 0) {
      score = difficulty * (((BLOCKS_WIDE * BLOCKS_HIGH) - countBlocks()) -
                            game_time.getElapsedTime<std::chrono::seconds>());
    } else if (score <= 0) {
      score = 1;
    }

    // Name input
    ib_name.focus();
    ib_name.update();
    dialog_yes.update();
    dialog_no.update();
  }
}

// Draw
void Game::draw() {
  // Draws background
  asw::draw::sprite(background, 0, 0);

  // Draws Blocks
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      MyBlocks[i][t].draw(startAnimate);
    }
  }

  // Draw particles
  particles.draw();

  // Draws foreground
  asw::draw::sprite(foreground, 0, 0);

  // Draw done button
  done.draw();

  // Draws text
  asw::draw::textRight(font, "Blocks Left: " + std::to_string(countBlocks()),
                       1240, 16, asw::util::makeColor(0, 0, 0));
  asw::draw::text(
      font,
      "Time: " +
          std::to_string(game_time.getElapsedTime<std::chrono::seconds>()),
      40, 16, asw::util::makeColor(0, 0, 0));

  // End game dialog
  if (game_over) {
    // Blur background
    asw::draw::sprite(trans_overlay, 0, 0);

    // Create gui
    asw::draw::sprite(dialog_box, 300, 300);

    asw::draw::sprite(foreground, 0, 0);

    asw::draw::textCenter(font, gameOverMessage, 640, 310,
                          asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font, "Score: " + std::to_string(score), 640, 360,
                          asw::util::makeColor(0, 0, 0));

    // Input rectangle
    ib_name.draw();

    // Buttons
    dialog_yes.draw();
    dialog_no.draw();
  }

  // Draws Cursor
  if (asw::input::mouse.down[1]) {
    asw::draw::sprite(cursor[1], asw::input::mouse.x, asw::input::mouse.y);
  } else {
    asw::draw::sprite(cursor[0], asw::input::mouse.x, asw::input::mouse.y);
  }
}
