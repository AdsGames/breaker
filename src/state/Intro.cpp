#include "./Intro.h"

void Intro::init() {
  // Intro
  intro = createObject<asw::game::Sprite>();
  intro->setTexture(asw::assets::loadTexture("assets/images/intro.png"));

  // Title
  title = createObject<asw::game::Sprite>();
  title->setTexture(asw::assets::loadTexture("assets/images/title.png"));

  // Start timer
  timer.start();
}

void Intro::update(float deltaTime) {
  Scene::update(deltaTime);

  auto time = timer.getElapsedTime<std::chrono::milliseconds>();

  intro->active = time < 1000;
  title->active = time > 1000;

  if (time < 200) {
    intro->alpha =
        asw::util::lerp(0.0F, 1.0F, static_cast<float>(time) / 200.0F);
  } else if (time > 800 && time < 1000) {
    intro->alpha =
        asw::util::lerp(1.0F, 0.0F, static_cast<float>(time - 800) / 200.0F);
  } else if (time > 1000 && time < 1200) {
    title->alpha =
        asw::util::lerp(0.0F, 1.0F, static_cast<float>(time - 1000) / 200.0F);
  } else if (time > 2800 && time < 3000) {
    title->alpha =
        asw::util::lerp(1.0F, 0.0F, static_cast<float>(time - 2800) / 200.0F);
  }

  if (time >= 3000 || asw::input::keyboard.anyPressed) {
    sceneManager.setNextScene(States::Menu);
  }
}
