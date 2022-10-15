#ifndef INTRO_H
#define INTRO_H

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "State.h"

// Intro screen of game
class Intro : public State {
 public:
  using State::State;

  void init() override;
  void update() override;
  void draw() override;
  void cleanup() override{
      // Nothing to do
  };

 private:
  asw::Texture intro;
  asw::Texture title;

  Timer timer;
};

#endif  // INTRO_H
