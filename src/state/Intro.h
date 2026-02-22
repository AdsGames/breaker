#ifndef INTRO_H
#define INTRO_H

#include <asw/asw.h>
#include <memory>

#include "./States.h"

// Intro screen of game
class Intro : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float dt) override;

 private:
  std::shared_ptr<asw::game::Sprite> intro_;
  std::shared_ptr<asw::game::Sprite> title_;

  float timer_{0.0F};
};

#endif  // INTRO_H
