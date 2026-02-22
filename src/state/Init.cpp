#include "./Init.h"

#include <asw/asw.h>

#include "../globals.h"

// Init state (and game)
void Init::init() {
  asw::display::set_title("Loading...");

  asw::display::set_icon("assets/images/icon.png");

  asw::display::set_title("Breaker");
}

// Update
void Init::update(float _dt) {
  // Goto splash
  manager.set_next_scene(States::Intro);
}
