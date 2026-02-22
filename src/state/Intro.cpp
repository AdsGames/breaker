#include "./Intro.h"

void Intro::init() {
  // Intro
  intro_ = create_object<asw::game::Sprite>();
  intro_->set_texture(asw::assets::load_texture("assets/images/intro.png"));

  // Title
  title_ = create_object<asw::game::Sprite>();
  title_->set_texture(asw::assets::load_texture("assets/images/title.png"));
}

void Intro::update(float dt) {
  Scene::update(dt);
  timer_ += dt;

  intro_->active = timer_ < 1.0F;
  title_->active = timer_ > 1.0F;

  if (timer_ < 0.2F) {
    intro_->alpha =
        asw::util::lerp(0.0F, 1.0F, static_cast<float>(timer_) / 0.2F);
  } else if (timer_ > 0.8F && timer_ < 1.0F) {
    intro_->alpha =
        asw::util::lerp(1.0F, 0.0F, static_cast<float>(timer_ - 0.8F) / 0.2F);
  } else if (timer_ > 1.0F && timer_ < 1.2F) {
    title_->alpha =
        asw::util::lerp(0.0F, 1.0F, static_cast<float>(timer_ - 1.0F) / 0.2F);
  } else if (timer_ > 2.8F && timer_ < 3.0F) {
    title_->alpha =
        asw::util::lerp(1.0F, 0.0F, static_cast<float>(timer_ - 2.8F) / 0.2F);
  }

  if (timer_ >= 3.0F || asw::input::keyboard.any_pressed) {
    manager.set_next_scene(States::Menu);
  }
}
