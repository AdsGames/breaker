/**
 * GAME
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef GAME_H
#define GAME_H

#include <asw/asw.h>
#include <asw/util/Timer.h>
#include <array>

#include "../Block.h"
#include "../Particle.h"
#include "../ParticleEmitter.h"
#include "../ScoreManager.h"
#include "../ui/Button.h"
#include "../ui/InputBox.h"
#include "./States.h"

constexpr int BLOCKS_WIDE = 14;
constexpr int BLOCKS_HIGH = 9;
constexpr int MAX_BLOCK_DIMENSION = 1000;

class Game : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float deltaTime) override;

  void draw() override;

 private:
  // Init the blocks on screen
  std::array<std::array<Block, BLOCKS_HIGH>, BLOCKS_WIDE> tiles;

  // Images
  std::array<asw::Texture, 2> cursor;
  asw::Texture foreground;
  asw::Texture dialog_box;
  asw::Texture trans_overlay;

  // Sounds
  asw::Sample block_break;
  asw::Sample click;

  asw::Font font;

  // Buttons
  Button done, dialog_yes, dialog_no;
  InputBox ib_name;

  // Variables
  int score;
  float startAnimate;
  int blocks_selected;
  bool game_over;
  std::string gameOverMessage;

  // Timers
  Timer game_time;

  // Scores
  ScoreManager highscores;

  // Select group of blocks
  void deselectBlocks();
  int selectBlock(int x, int y, int type);
  Block* blockAt(int x, int y);
  asw::Vec2<int> getBlockIndex(float screen_x, float screen_y);
  void destroySelectedBlocks();
  int countBlocks();
  bool hasRemainingMoves();

  // Particles
  ParticleEmitter particles;
};

#endif  // GAME_H
