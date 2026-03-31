#include "./Game.h"

#include "../globals.h"

// INIT
void Game::init() {
  // Emitters
  asw::ParticleConfig emitter_config;
  emitter_config.lifetime_min = 5.0F;
  emitter_config.lifetime_max = 1.0F;
  emitter_config.speed_min = 10.0F;
  emitter_config.speed_max = 100.0F;
  emitter_config.texture =
      asw::assets::load_texture("assets/images/fuzzball.png");
  emitter_config.size_start = 30.0F;
  emitter_config.size_end = 1.0F;
  emitter_config.gravity = {1000.0F, -1000.0F};

  emitter = asw::ParticleEmitter(emitter_config);
  emitter.start();

  // Sets background
  create_object<asw::game::Sprite>()->set_texture(
      asw::assets::load_texture("assets/images/background.png"));

  // Sets Foreground
  foreground = asw::assets::load_texture("assets/images/foreground.png");

  // Sets menu
  dialog_box = asw::assets::load_texture("assets/images/menu.png");

  // Trans overlay
  trans_overlay = asw::assets::load_texture("assets/images/overlay.png");

  // Sets Cursors
  cursor[0] = asw::assets::load_texture("assets/images/cursor1.png");
  cursor[1] = asw::assets::load_texture("assets/images/cursor2.png");

  // Sets Sounds
  block_break = asw::assets::load_sample("assets/sounds/break.wav");
  click = asw::assets::load_sample("assets/sounds/click.wav");

  // Sets Font
  font = asw::assets::load_font("assets/fonts/ariblk.ttf", 24);

  // Sets Variables
  score = 0;
  startAnimate = 1.2F;
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
                     manager.set_next_scene(States::Game);
                   });

  dialog_no = Button()
                  .setPosition({461 + 300, 215 + 300})
                  .setImages("assets/images/buttons/no.png",
                             "assets/images/buttons/no_hover.png")
                  .setOnClick([this]() {
                    highscores.add(ib_name.getValue(), score);
                    manager.set_next_scene(States::Menu);
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

        emitter.transform.position =
            block.getTransform().position + asw::Vec2<float>(40, 40);
        emitter.emit(10);

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
void Game::update(float dt) {
  Scene::update(dt);
  const auto& mouse = asw::input::get_mouse();

  // Animation for start of game
  if (startAnimate > 0.0F) {
    startAnimate -= dt;
  } else {
    startAnimate = 0;
  }

  // In Game
  if (!game_over) {
    if (startAnimate == 0) {
      game_time += dt;
    }

    // Update particles
    emitter.update(dt);

    // Update Tiles
    for (auto& row : tiles) {
      for (auto& tile : row) {
        tile.update(dt);
      }
    }

    // Select blocks
    if (asw::input::get_mouse_button_down(asw::input::MouseButton::Left)) {
      auto selected_block = getBlockIndex(mouse.position.x, mouse.position.y);

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
                              static_cast<int>(game_time));
      } else if (score <= 0) {
        score = 1;
      }

      game_over = true;

      ib_name.focus();
    }
  }

  // Game over state
  else {
    // Name input
    ib_name.update();
    dialog_yes.update(dt);
    dialog_no.update(dt);
  }
}

// Draw
void Game::draw() {
  Scene::draw();
  const auto& mouse = asw::input::get_mouse();

  // Draws Tiles
  for (const auto& row : tiles) {
    for (const auto& tile : row) {
      tile.draw(startAnimate * 1000.0F);
    }
  }

  // Draw particles
  emitter.draw();

  // Draws foreground
  asw::draw::sprite(foreground, asw::Vec2<float>(0, 0));

  // Draw done button
  done.draw();

  // Draws text
  asw::draw::text(font, std::format("Blocks Left: {}", countBlocks()),
                  asw::Vec2<float>(1240, 16), asw::Color(0, 0, 0),
                  asw::TextJustify::Right);
  asw::draw::text(font, std::format("Time: {}", static_cast<int>(game_time)),
                  asw::Vec2<float>(40, 16), asw::Color(0, 0, 0));

  // End game dialog
  if (game_over) {
    // Blur background
    asw::draw::sprite(trans_overlay, asw::Vec2<float>(0, 0));

    // Create gui
    asw::draw::sprite(dialog_box, asw::Vec2<float>(300, 300));

    asw::draw::sprite(foreground, asw::Vec2<float>(0, 0));

    asw::draw::text(font, gameOverMessage, asw::Vec2<float>(640, 310),
                    asw::Color(0, 0, 0), asw::TextJustify::Center);
    asw::draw::text(font, std::format("Score: {}", score),
                    asw::Vec2<float>(640, 360), asw::Color(0, 0, 0),
                    asw::TextJustify::Center);

    // Input rectangle
    ib_name.draw();

    // Buttons
    dialog_yes.draw();
    dialog_no.draw();
  }

  // Draws Cursor
  if (asw::input::get_mouse_button(asw::input::MouseButton::Left)) {
    asw::draw::sprite(cursor[1], mouse.position);
  } else {
    asw::draw::sprite(cursor[0], mouse.position);
  }
}
