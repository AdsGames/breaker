#include "Intro.h"

#include "utility/tools.h"

void Intro::init() {
  // Sets Starting Images
  title = asw::assets::loadTexture("assets/images/title.png");
  intro = asw::assets::loadTexture("assets/images/intro.png");

  // Start timer
  timer.Start();
}

void Intro::update() {
  auto time = timer.GetElapsedTime<milliseconds>();

  if (time >= 3000 || asw::input::keyboard.anyPressed) {
    setNextState(ProgramState::STATE_MENU);
  }
}

void Intro::draw() {
  auto time = timer.GetElapsedTime<milliseconds>();

  if (time < 1500) {
    asw::draw::sprite(intro, 0, 0);
  } else {
    asw::draw::sprite(title, 0, 0);
  }
}
