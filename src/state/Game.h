/**
 * GAME
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef GAME_H
#define GAME_H

#include <asw/asw.h>
#include <array>
#include <string>
#include <vector>

#include "../Block.h"
#include "../ScoreManager.h"
#include "./States.h"

constexpr int BLOCKS_WIDE = 14;
constexpr int BLOCKS_HIGH = 9;
constexpr int MAX_BLOCK_DIMENSION = 1000;

// Points added for clearing the whole board, per difficulty step
constexpr int CLEAR_BONUS = 1000;

// Points lost for each block left on the board
constexpr int LEFTOVER_PENALTY = 10;

// Floating "+N" text shown where a group broke
struct ScorePopup {
  asw::Vec2f position;
  std::string text;
  asw::Color color;
  bool big{false};
  float life{0.0F};
};

class Game : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float dt) override;

  void draw() override;

  void cleanup() override;

 private:
  // Init the blocks on screen
  std::array<std::array<Block, BLOCKS_HIGH>, BLOCKS_WIDE> tiles;

  // Images
  std::array<asw::Texture, 2> cursor;
  asw::Texture foreground;
  asw::Texture trans_overlay;

  // Sounds
  asw::Sample block_break;
  asw::Sample click;

  // Particles: debris in each block colour, and white sparks
  std::array<asw::ParticleEmitter, Block::TYPE_EMPTY> debris;
  asw::ParticleEmitter sparks;

  asw::Font font;
  asw::Font font_label;
  asw::Font font_big;

  // Camera, used only for screen shake
  asw::Camera camera;

  // Score popups
  std::vector<ScorePopup> popups;

  // Done button and game over dialog
  asw::ui::Root ui;
  asw::ui::Button* done{nullptr};
  asw::ui::Panel* dialog{nullptr};
  asw::ui::Label* dialog_message{nullptr};
  asw::ui::Label* dialog_detail{nullptr};
  asw::ui::Label* dialog_score{nullptr};
  asw::ui::InputBox* ib_name{nullptr};

  // Variables
  int score;
  float startAnimate;
  int blocks_selected;
  bool game_over;
  std::string gameOverMessage;

  // Timers
  float game_time{0.0F};

  // Scores
  ScoreManager highscores;

  // Select group of blocks
  void deselectBlocks();
  int selectBlock(int x, int y, int type);
  Block* blockAt(int x, int y);
  asw::Vec2i getBlockIndex(float screen_x, float screen_y);
  void destroySelectedBlocks();
  int countBlocks();
  bool hasRemainingMoves();
  int pointsForGroup(int size) const;
  int difficultyMultiplier() const;
  void endGame(const std::string& message);
  void showDialog(bool show);
};

#endif  // GAME_H
