#ifndef INTRO_H
#define INTRO_H

#include <asw/asw.h>
#include <asw/util/Timer.h>
#include <memory>

#include "./States.h"

// Intro screen of game
class Intro : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float deltaTime) override;

 private:
  std::shared_ptr<asw::game::Sprite> intro;
  std::shared_ptr<asw::game::Sprite> title;

  Timer timer;
};

#endif  // INTRO_H
