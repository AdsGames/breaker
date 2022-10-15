/**
 * INIT
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef INIT_H
#define INIT_H

#include <asw/asw.h>

#include "State.h"

class Init : public State {
 public:
  using State::State;

  void init() override;
  void update() override;
  void draw() override{
      // Nothing to do
  };
  void cleanup() override {
    // Nothing to do
  }
};

#endif  // INIT_H
