#ifndef INTRO_H
#define INTRO_H

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "./States.h"

// Intro screen of game
class Intro : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float deltaTime) override;

  void draw() override;

  void cleanup() override {
    // Nothing to do
  };

 private:
  asw::Texture intro;
  asw::Texture title;

  Timer timer;
};

#endif  // INTRO_H
