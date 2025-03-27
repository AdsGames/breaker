#include "./Init.h"

#include <asw/asw.h>

#include "../globals.h"

// Init state (and game)
void Init::init() {
  asw::display::setTitle("Loading...");

  asw::display::setIcon("assets/images/icon.png");

  asw::display::setTitle("Breaker");
}

// Update
void Init::update(float _deltaTime) {
  // Goto splash
  sceneManager.setNextScene(States::Intro);
}
