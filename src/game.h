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

#include <allegro.h>

#include "state.h"

#include "ScoreManager.h"
#include "block.h"
#include "particle.h"
#include "particle_emitter.h"
#include "ui/Button.h"
#include "ui/InputBox.h"
#include "utility/Timer.h"

class game : public state {
 public:
  game();
  virtual ~game();

  virtual void update() override;
  virtual void draw() override;

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
  BITMAP* buffer;
  BITMAP* background;
  BITMAP* cursor[2];
  BITMAP* foreground;
  BITMAP* dialog_box;
  BITMAP* trans_overlay;

  // Sounds
  SAMPLE* block_break;
  SAMPLE* click;

  // Buttons
  Button done, dialog_yes, dialog_no;
  InputBox ib_name;

  // Variables
  int score;
  int startAnimate;
  int blocks_selected;
  bool game_over;
  std::string game_over_message;

  // Timers
  Timer game_time;

  // Scores
  ScoreManager highscores;

  // Select group of blocks
  void deselect_blocks();
  int select_block(int x, int y, int type);
  Block* block_at(int x, int y);
  coordinate get_block_index(int screen_x, int screen_y);
  void destroy_selected_blocks();
  int count_blocks();
  int count_remaining_moves();

  // Particles
  particle_emitter particles;
};

#endif  // GAME_H
