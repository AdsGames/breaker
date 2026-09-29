#include "./Game.h"

#include <algorithm>

#include "../globals.h"
#include "../ui/ImageButton.h"
#include "../ui/Style.h"

namespace {
// Group size that gets the big popup, and that starts the screen shake
constexpr int BIG_GROUP = 10;
constexpr int SHAKE_GROUP = 5;

// Group size where popups turn fully gold
constexpr int GOLD_GROUP = 15;
const asw::Color POPUP_WHITE{255, 255, 255};
const asw::Color POPUP_GOLD{255, 200, 40};

// Largest shake offset, in pixels
constexpr float MAX_SHAKE = 20.0F;

// Debris per broken block grows with group size, up to a cap so huge
// groups stay smooth. Sparks per broken block.
constexpr int MIN_DEBRIS = 6;
constexpr int MAX_DEBRIS = 16;
constexpr int SPARKS = 3;
constexpr uint32_t PARTICLE_POOL = 2048;

// Debris colours, matching the block images
const std::array<asw::Color, Block::TYPE_EMPTY> BLOCK_COLORS = {
    asw::Color(255, 8, 0),   asw::Color(255, 127, 0), asw::Color(255, 233, 0),
    asw::Color(20, 173, 0),  asw::Color(0, 128, 255), asw::Color(119, 0, 255)};

// Popup life, fade time at the end of it, and rise speed in px/s
constexpr float POPUP_LIFE = 1.0F;
constexpr float POPUP_FADE = 0.4F;
constexpr float POPUP_RISE = 80.0F;
}  // namespace

// INIT
void Game::init() {
  // Emitters
  // Debris flies out of each block in its colour, then falls and fades
  for (int i = 0; i < Block::TYPE_EMPTY; i++) {
    asw::ParticleConfig config;
    config.lifetime_min = 0.5F;
    config.lifetime_max = 1.1F;
    config.speed_min = 120.0F;
    config.speed_max = 480.0F;
    config.color_start = BLOCK_COLORS[i];
    config.color_end = BLOCK_COLORS[i].lerp(asw::Color(0, 0, 0), 0.4F);
    config.size_start = 16.0F;
    config.size_end = 4.0F;
    config.gravity = {0.0F, 1600.0F};
    debris[i] = asw::ParticleEmitter(config, PARTICLE_POOL);
  }

  // Short white sparks for a flash on each break
  asw::ParticleConfig spark_config;
  spark_config.lifetime_min = 0.15F;
  spark_config.lifetime_max = 0.4F;
  spark_config.speed_min = 300.0F;
  spark_config.speed_max = 900.0F;
  spark_config.color_start = asw::Color(255, 255, 230);
  spark_config.color_end = asw::Color(255, 200, 80);
  spark_config.size_start = 6.0F;
  spark_config.size_end = 1.0F;
  spark_config.gravity = {0.0F, 400.0F};
  sparks = asw::ParticleEmitter(spark_config, PARTICLE_POOL);

  // Sets background
  create_object<asw::game::Sprite>()->set_texture(
      asw::assets::load_texture("assets/images/background.png"));

  // Sets Foreground
  foreground = asw::assets::load_texture("assets/images/foreground.png");

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
  font_label = asw::assets::load_font("assets/fonts/ariblk.ttf", 32);
  font_big = asw::assets::load_font("assets/fonts/ariblk.ttf", 40);

  // Sets Variables
  score = 0;
  startAnimate = 1.2F;
  blocks_selected = 0;
  game_over = false;
  gameOverMessage = "Game Over";
  popups.clear();

  // Sets block info
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      auto position = asw::Vec2f((i * Block::SIZE) + Block::SIZE,
                                 (t * Block::SIZE) + Block::SIZE);
      tiles[i][t] = Block(position, asw::random::between(0, difficulty));
    }
  }

  // Give score files
  highscores = ScoreManager("scores.dat");

  // UI, styled to match the game art
  ui.set_size(1280, 960);
  applyTheme(ui.ctx.theme, font);
  ui.ctx.theme.input.bg = asw::Color(245, 245, 245);
  ui.ctx.theme.input.text = asw::Color(22, 22, 22);
  ui.ctx.theme.input.caret = asw::Color(22, 22, 22);
  ui.ctx.theme.input.placeholder = asw::Color(120, 120, 120);
  ui.ctx.theme.input.border = asw::Color(12, 12, 12);
  ui.ctx.theme.input.border_hover = asw::Color(12, 12, 12);

  done = &addImageButton(ui.root, {500, 10}, "assets/images/buttons/done.png",
                         "assets/images/buttons/done_hover.png",
                         [this]() { endGame("Game Over"); });

  // Game over dialog
  dialog = &addPanel(ui.root, asw::Quadf(299, 270, 682, 370));

  dialog_message = &addLabel(*dialog, {640, 288}, gameOverMessage, font_label,
                             asw::TextJustify::Center);
  dialog_detail =
      &addLabel(*dialog, {640, 336}, "", font, asw::TextJustify::Center);
  dialog_score =
      &addLabel(*dialog, {640, 372}, "", font, asw::TextJustify::Center);

  addLabel(*dialog, {340, 424}, "Name:", font_label);

  ib_name = &dialog->add_child<asw::ui::InputBox>();
  ib_name->transform = asw::Quadf(488, 425, 452, 44);
  ib_name->font = font;
  ib_name->value = "Player";
  ib_name->placeholder = "Player";

  addLabel(*dialog, {640, 482}, "Play Again?", font_label,
           asw::TextJustify::Center);

  auto& yes = dialog->add_child<asw::ui::Button>();
  yes.transform = asw::Quadf(340, 548, 180, 52);
  yes.font = font_label;
  yes.set_text("Yes");
  yes.on_click = [this]() {
    highscores.add(ib_name->value, score);
    manager.set_next_scene(States::Game);
  };

  auto& no = dialog->add_child<asw::ui::Button>();
  no.transform = asw::Quadf(760, 548, 180, 52);
  no.font = font_label;
  no.set_text("No");
  no.on_click = [this]() {
    highscores.add(ib_name->value, score);
    manager.set_next_scene(States::Menu);
  };

  showDialog(false);
}

// Show the game over dialog, or the in game done button
void Game::showDialog(bool show) {
  done->visible = !show;
  dialog->visible = show;
}

// Cleanup
void Game::cleanup() {
  Scene::cleanup();

  // Losing focus stops text input. The box is freed on the next update
  ui.clear_focus();
  ui.root.clear_children();
  ib_name = nullptr;
  done = nullptr;
  dialog = nullptr;
  dialog_message = nullptr;
  dialog_detail = nullptr;
  dialog_score = nullptr;
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
      tile.getType() != Block::TYPE_EMPTY) {
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
  asw::Vec2f group_centre{0.0F, 0.0F};

  // Count first, so bigger groups can throw more particles per block
  for (const auto& row : tiles) {
    for (const auto& block : row) {
      if (block.getSelected()) {
        num_destroyed++;
      }
    }
  }

  const int debris_count =
      std::min(MAX_DEBRIS, MIN_DEBRIS + (num_destroyed / 2));
  constexpr float HALF = Block::SIZE / 2;

  for (auto& row : tiles) {
    for (auto& block : row) {
      if (block.getSelected()) {
        block.setSelected(false);

        const auto centre = block.getTransform().position +
                            asw::Vec2f(Block::SIZE / 2, Block::SIZE / 2);
        group_centre = group_centre + centre;

        // Spread the debris over the block, not from a single point
        auto& block_debris = debris[block.getType()];
        for (int i = 0; i < debris_count; i++) {
          block_debris.transform.position =
              centre + asw::Vec2f(asw::random::between(-HALF, HALF),
                                  asw::random::between(-HALF, HALF));
          block_debris.emit(1);
        }

        sparks.transform.position = centre;
        sparks.emit(SPARKS);

        block.setType(Block::TYPE_EMPTY);
      }
    }
  }

  // Score the group, and float the points up from its centre
  if (num_destroyed > 0) {
    const int points = pointsForGroup(num_destroyed);
    score += points;

    ScorePopup popup;
    popup.position = group_centre / static_cast<float>(num_destroyed);
    popup.text = std::format("+{}", points);
    popup.color = POPUP_WHITE.lerp(
        POPUP_GOLD, std::min(1.0F, static_cast<float>(num_destroyed - 2) /
                                       static_cast<float>(GOLD_GROUP - 2)));
    popup.big = num_destroyed >= BIG_GROUP;
    popup.life = POPUP_LIFE;
    popups.push_back(popup);

    // Shake harder for bigger groups
    if (num_destroyed >= SHAKE_GROUP) {
      camera.shake(std::min(MAX_SHAKE, static_cast<float>(num_destroyed)));
    }
  }

  // Louder for bigger groups; slight pitch variation so repeats sound natural.
  // Panned towards the side of the screen the group was on.
  asw::sound::PlayOptions break_options;
  break_options.volume = static_cast<float>(64 + num_destroyed * 5) / 255.0F;
  break_options.pitch_variation = 0.05F;
  const float break_x =
      num_destroyed > 0 ? group_centre.x / static_cast<float>(num_destroyed)
                        : 640.0F;
  asw::sound::play_at(block_break, break_x, break_options);

  // Settle blocks downwards
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    int num_blank = BLOCKS_HIGH - 1;

    for (int t = BLOCKS_HIGH - 1; t >= 0; t--) {
      if (tiles[i][t].getType() != Block::TYPE_EMPTY) {
        // Block drops from row t to row num_blank; offset it upward by the
        // distance fallen so update() eases it back down.
        const float fall = static_cast<float>(num_blank - t) * Block::SIZE;
        tiles[i][num_blank].setType(tiles[i][t].getType());
        tiles[i][num_blank].setVisualOffset({0.0F, -fall});
        num_blank--;
      }
    }

    while (num_blank >= 0) {
      tiles[i][num_blank--].setType(Block::TYPE_EMPTY);
    }
  }

  // Settle blocks across
  int num_back = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++) {
    if (num_back > 0) {
      const float slide = static_cast<float>(num_back) * Block::SIZE;

      for (int t = 0; t < BLOCKS_HIGH; t++) {
        // Column slides num_back cells left; carry any in-progress fall
        // offset and add the horizontal slide distance.
        const auto carried = tiles[i][t].getVisualOffset();
        tiles[i - num_back][t].setType(tiles[i][t].getType());
        tiles[i - num_back][t].setVisualOffset(carried +
                                               asw::Vec2f(slide, 0.0F));
      }
    }

    if (tiles[i][BLOCKS_HIGH - 1].getType() == Block::TYPE_EMPTY) {
      num_back++;
    }
  }

  while (num_back > 0) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      tiles[BLOCKS_WIDE - num_back][t].setType(Block::TYPE_EMPTY);
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
      if (block.getType() != Block::TYPE_EMPTY) {
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
      if (tiles[i][t].getType() == Block::TYPE_EMPTY) {
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

// Easy 1x, medium 2x, hard 3x
int Game::difficultyMultiplier() const {
  return std::max(1, difficulty - 2);
}

// Points grow with the square of the group size, so big groups pay off
int Game::pointsForGroup(int size) const {
  return (size - 1) * (size - 1) * difficultyMultiplier();
}

// Apply end bonus or penalty and show the dialog
void Game::endGame(const std::string& message) {
  if (game_over) {
    return;
  }

  const int left = countBlocks();
  gameOverMessage = message;

  if (left == 0) {
    const int bonus = CLEAR_BONUS * difficultyMultiplier();
    score += bonus;
    dialog_detail->text = std::format("Board cleared: +{} bonus", bonus);
  } else {
    const int penalty = left * LEFTOVER_PENALTY;
    score = std::max(0, score - penalty);
    dialog_detail->text = std::format("{} block{} left: -{} points", left,
                                      left == 1 ? "" : "s", penalty);
  }

  deselectBlocks();
  blocks_selected = 0;
  game_over = true;
  dialog_message->text = gameOverMessage;
  dialog_score->text = std::format("Score: {}", score);
  showDialog(true);
  ui.ctx.focus.set_focus(ui.ctx, ib_name);
}

// Block index
asw::Vec2i Game::getBlockIndex(float screen_x, float screen_y) {
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

  // Buttons and name input. Blocks ignore clicks the UI used
  const bool ui_used = ui.update();

  // Animation for start of game
  if (startAnimate > 0.0F) {
    startAnimate -= dt;
  } else {
    startAnimate = 0;
  }

  // Particles and shake finish even after the game ends
  for (auto& emitter : debris) {
    emitter.update(dt);
  }
  sparks.update(dt);
  camera.update(dt);

  // In Game
  if (!game_over) {
    if (startAnimate == 0) {
      game_time += dt;
    }

    // Update Tiles
    for (auto& row : tiles) {
      for (auto& tile : row) {
        tile.update(dt);
      }
    }

    // Select blocks
    if (!ui_used &&
        asw::input::get_mouse_button_down(asw::input::MouseButton::Left)) {
      auto selected_block = getBlockIndex(mouse.position.x, mouse.position.y);
      auto* clicked = blockAt(selected_block.x, selected_block.y);

      if (blocks_selected > 1 && clicked != nullptr && clicked->getSelected()) {
        destroySelectedBlocks();
        blocks_selected = 0;
      } else {
        deselectBlocks();
        blocks_selected = selectBlock(
            selected_block.x, selected_block.y);  // Select and count blocks

        if (!config_double_click && blocks_selected > 1 && clicked != nullptr &&
            clicked->getSelected()) {
          destroySelectedBlocks();
          blocks_selected = 0;
        }
      }
    }

    // Lose
    if (!game_over && !hasRemainingMoves()) {
      endGame(countBlocks() == 0 ? "You Win!" : "Out of moves");
    }
  }

  // Float popups upward and drop expired ones
  for (auto& popup : popups) {
    popup.life -= dt;
    popup.position.y -= POPUP_RISE * dt;
  }

  std::erase_if(popups, [](const ScorePopup& p) { return p.life <= 0.0F; });
}

// Draw
void Game::draw() {
  Scene::draw();
  const auto& mouse = asw::input::get_mouse();

  // Draws Tiles, dropped in from above at start and moved by shake
  const auto tile_offset = camera.world_to_screen({0.0F, 0.0F}) -
                           asw::Vec2f(0.0F, startAnimate * 1000.0F);

  for (const auto& row : tiles) {
    for (const auto& tile : row) {
      tile.draw(tile_offset);
    }
  }

  // Draw particles, moved by shake like the tiles
  for (auto& emitter : debris) {
    emitter.draw(camera);
  }
  sparks.draw(camera);

  // Draws foreground
  asw::draw::sprite(foreground, asw::Vec2f(0, 0));

  // Score popups, fading out as they rise
  for (const auto& popup : popups) {
    const auto alpha =
        static_cast<uint8_t>(255.0F * std::min(1.0F, popup.life / POPUP_FADE));
    asw::draw::text_shadow(
        popup.big ? font_big : font, popup.text, popup.position,
        asw::Color(popup.color.r, popup.color.g, popup.color.b, alpha),
        asw::Color(0, 0, 0, alpha), SHADOW_OFFSET, asw::TextJustify::Center);
  }

  // Draw done button
  if (!game_over) {
    ui.draw();
  }

  // Draws text
  asw::draw::text_shadow(font, std::format("Blocks Left: {}", countBlocks()),
                         asw::Vec2f(1240, 16), asw::Color(0, 0, 0), TEXT_SHADOW,
                         SHADOW_OFFSET, asw::TextJustify::Right);
  asw::draw::text_shadow(
      font, std::format("Time: {}", static_cast<int>(game_time)),
      asw::Vec2f(40, 16), asw::Color(0, 0, 0), TEXT_SHADOW, SHADOW_OFFSET);
  asw::draw::text_shadow(font, std::format("Score: {}", score),
                         asw::Vec2f(220, 16), asw::Color(0, 0, 0), TEXT_SHADOW,
                         SHADOW_OFFSET);

  // End game dialog
  if (game_over) {
    // Blur background
    asw::draw::sprite(trans_overlay, asw::Vec2f(0, 0));

    asw::draw::sprite(foreground, asw::Vec2f(0, 0));

    // Dialog
    ui.draw();
  }

  // Draws Cursor
  if (asw::input::get_mouse_button(asw::input::MouseButton::Left)) {
    asw::draw::sprite(cursor[1], mouse.position);
  } else {
    asw::draw::sprite(cursor[0], mouse.position);
  }
}
