#include "Init.h"

#include <asw/asw.h>

#include "globals.h"
#include "utility/tools.h"

// Init state (and game)
void Init::init() {
  asw::display::setTitle("Loading...");

  asw::display::setIcon("assets/images/icon.png");

  asw::display::setTitle("Breaker");
}

// Update
void Init::update() {
  // Goto splash
  setNextState(ProgramState::STATE_INTRO);
}
