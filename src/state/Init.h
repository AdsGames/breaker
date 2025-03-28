/**
 * INIT
 * Allan Legemaate
 * 26/10/2017
 **/
#ifndef INIT_H
#define INIT_H

#include <asw/asw.h>

#include "./States.h"

class Init : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float deltaTime) override;
};

#endif  // INIT_H
