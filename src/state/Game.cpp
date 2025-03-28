#include "./Game.h"

#include "../globals.h"

// INIT
void Game::init() {
  // Sets background
  createObject<asw::game::Sprite>()->setTexture(
      asw::assets::loadTexture("assets/images/background.png"));

  // Sets Foreground
  foreground = asw::assets::loadTexture("assets/images/foreground.png");

  // Sets menu
  dialog_box = asw::assets::loadTexture("assets/images/menu.png");

  // Trans overlay
  trans_overlay = asw::assets::loadTexture("assets/images/overlay.png");

  // Sets Cursors
  cursor[0] = asw::assets::loadTexture("assets/images/cursor1.png");
  cursor[1] = asw::assets::loadTexture("assets/images/cursor2.png");

  // Sets Sounds
  block_break = asw::assets::loadSample("assets/sounds/break.wav");
  click = asw::assets::loadSample("assets/sounds/click.wav");

  // Sets Font
  font = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  // Sets Variables
  score = 0;
  startAnimate = 1200.0F;
  blocks_selected = 0;
  game_over = false;
  gameOverMessage = "Game Over";

  // Sets block info
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      auto position = asw::Vec2<float>((i * 80) + 80, (t * 80) + 80);
      tiles[i][t] = Block(position, asw::random::between(0, difficulty));
    }
  }

  // Give score files
  highscores = ScoreManager("scores.dat");

  // Buttons
  done = Button()
             .setPosition({500, 10})
             .setImages("assets/images/buttons/done.png",
                        "assets/images/buttons/done_hover.png")
             .setOnClick([this]() { game_over = true; });

  dialog_yes = Button()
                   .setPosition({41 + 300, 215 + 300})
                   .setImages("assets/images/buttons/yes.png",
                              "assets/images/buttons/yes_hover.png")
                   .setOnClick([this]() {
                     highscores.add(ib_name.getValue(), score);
                     sceneManager.setNextScene(States::Game);
                   });

  dialog_no = Button()
                  .setPosition({461 + 300, 215 + 300})
                  .setImages("assets/images/buttons/no.png",
                             "assets/images/buttons/no_hover.png")
                  .setOnClick([this]() {
                    highscores.add(ib_name.getValue(), score);
                    sceneManager.setNextScene(States::Menu);
                  });

  ib_name = InputBox({488, 405, 404, 44}, font, "Player");
}

// Deselect all blocks
void Game::deselectBlocks() {
  for (auto& row : tiles) {
    for (auto& block : row) {
      block.setSelected(false);
    }
  }
}

// Select group of blocks
int Game::selectBlock(int x, int y, int type = -1) {
  if (x >= BLOCKS_WIDE || y >= BLOCKS_HIGH || x < 0 || y < 0) {
    return 0;
  }

  auto& tile = tiles[x][y];

  if ((type == -1 || tile.getType() == type) && !tile.getSelected() &&
      tile.getType() != 6) {
    // Select it
    tile.setSelected(true);

    // Select 4 around it
    const int btype = tile.getType();

    return 1 + selectBlock(x - 1, y, btype) + selectBlock(x + 1, y, btype) +
           selectBlock(x, y - 1, btype) + selectBlock(x, y + 1, btype);
  }

  return 0;
}

// Destroy selected
void Game::destroySelectedBlocks() {
  // Remove blocks
  int num_destroyed = 0;

  for (auto& row : tiles) {
    for (auto& block : row) {
      if (block.getSelected()) {
        block.setSelected(false);
        block.explode(particles);
        block.setType(6);
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
      if (tiles[i][t].getType() != 6) {
        tiles[i][num_blank--].setType(tiles[i][t].getType());
      }

    while (num_blank >= 0) {
      tiles[i][num_blank--].setType(6);
    }
  }

  // Settle blocks across
  int num_back = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      tiles[i - num_back][t].setType(tiles[i][t].getType());
    }

    if (tiles[i][BLOCKS_HIGH - 1].getType() == 6) {
      num_back++;
    }
  }

  while (num_back > 0) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      tiles[BLOCKS_WIDE - num_back][t].setType(6);
    }

    num_back--;
  }
}

// Block at x y
Block* Game::blockAt(int x, int y) {
  if (x < BLOCKS_WIDE && y < BLOCKS_HIGH && x >= 0 && y >= 0) {
    return &tiles[x][y];
  }

  return nullptr;
}

// Remaining blocks
int Game::countBlocks() {
  int blocks_left = 0;

  for (auto& row : tiles) {
    for (auto& block : row) {
      if (block.getType() != 6) {
        blocks_left++;
      }
    }
  }

  return blocks_left;
}

// Moves left
bool Game::hasRemainingMoves() {
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (tiles[i][t].getType() == 6) {
        continue;
      }

      if (i < BLOCKS_WIDE - 1 &&
          tiles[i][t].getType() == tiles[i + 1][t].getType()) {
        return true;
      }

      if (t < BLOCKS_HIGH - 1 &&
          tiles[i][t].getType() == tiles[i][t + 1].getType()) {
        return true;
      }
    }
  }

  return false;
}

// Block index
asw::Vec2<int> Game::getBlockIndex(float screen_x, float screen_y) {
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (tiles[i][t].getTransform().contains({screen_x, screen_y})) {
        return {i, t};
      }
    }
  }

  return {-1, -1};
}

// Update
void Game::update(float deltaTime) {
  Scene::update(deltaTime);

  // Animation for start of game
  if (startAnimate > 10) {
    startAnimate -= deltaTime;
  } else {
    startAnimate = 0;
    if (!game_time.isRunning()) {
      game_time.start();
    }
  }

  // In Game
  if (!game_over) {
    // Update particles
    particles.update(deltaTime);
    // done.update();

    // Update Tiles
    for (auto& row : tiles) {
      for (auto& tile : row) {
        tile.update(deltaTime);
      }
    }

    // Select blocks
    if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
      auto selected_block =
          getBlockIndex(asw::input::mouse.x, asw::input::mouse.y);

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

      // Quit
      if (done.isHovering()) {
        game_over = true;
        gameOverMessage = "Game Over";
      }
    }

    // Lose
    if (!hasRemainingMoves()) {
      if (countBlocks() == 0) {
        gameOverMessage = "You Win!";
      } else {
        gameOverMessage = "Out of moves";
      }

      // Set score
      if (score == 0) {
        score = difficulty * (((BLOCKS_WIDE * BLOCKS_HIGH) - countBlocks()) -
                              game_time.getElapsedTime<std::chrono::seconds>());
      } else if (score <= 0) {
        score = 1;
      }

      game_over = true;

      ib_name.focus();
    }
  }

  // Game over state
  else {
    if (game_time.isRunning()) {
      game_time.stop();
    }

    // Name input
    ib_name.update();
    dialog_yes.update(deltaTime);
    dialog_no.update(deltaTime);
  }
}

// Draw
void Game::draw() {
  Scene::draw();

  // Draws Tiles
  for (auto& row : tiles) {
    for (auto& tile : row) {
      tile.draw(startAnimate);
    }
  }

  // Draw particles
  particles.draw();

  // Draws foreground
  asw::draw::sprite(foreground, asw::Vec2<float>(0, 0));

  // Draw done button
  done.draw();

  // Draws text
  asw::draw::textRight(font, "Blocks Left: " + std::to_string(countBlocks()),
                       asw::Vec2<float>(1240, 16),
                       asw::util::makeColor(0, 0, 0));
  asw::draw::text(
      font,
      "Time: " + std::to_string(static_cast<int>(
                     game_time.getElapsedTime<std::chrono::seconds>())),
      asw::Vec2<float>(40, 16), asw::util::makeColor(0, 0, 0));

  // End game dialog
  if (game_over) {
    // Blur background
    asw::draw::sprite(trans_overlay, asw::Vec2<float>(0, 0));

    // Create gui
    asw::draw::sprite(dialog_box, asw::Vec2<float>(300, 300));

    asw::draw::sprite(foreground, asw::Vec2<float>(0, 0));

    asw::draw::textCenter(font, gameOverMessage, asw::Vec2<float>(640, 310),
                          asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font, "Score: " + std::to_string(score),
                          asw::Vec2<float>(640, 360),
                          asw::util::makeColor(0, 0, 0));

    // Input rectangle
    ib_name.draw();

    // Buttons
    dialog_yes.draw();
    dialog_no.draw();
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
