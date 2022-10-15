/**
 * GAME
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef GAME_H
#define GAME_H

#define BLOCKS_WIDE 14
#define BLOCKS_HIGH 9
#define MAX_BLOCK_DIMENSION 1000

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "State.h"

#include "Block.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ScoreManager.h"
#include "ui/Button.h"
#include "ui/InputBox.h"

class Game : public State {
 public:
  using State::State;

  void init() override;
  void update() override;
  void draw() override;
  void cleanup() override {
    // Nothing to do
  }

 private:
  struct coordinate {
    coordinate(int x, int y) {
      this->x = x;
      this->y = y;
    }
    int x;
    int y;
  };

  // Init the blocks on screen
  Block MyBlocks[BLOCKS_WIDE][BLOCKS_HIGH];

  // Images
  asw::Texture background;
  asw::Texture cursor[2];
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
  int startAnimate;
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
  coordinate getBlockIndex(int screen_x, int screen_y);
  void destroySelectedBlocks();
  int countBlocks();
  int countRemainingMoves();

  // Particles
  ParticleEmitter particles;
};

#endif  // GAME_H
