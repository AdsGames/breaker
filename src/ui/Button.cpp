#include "Button.h"

Button& Button::setPosition(const asw::Vec2<float>& position) {
  transform.position = position;
  return *this;
}

Button& Button::setSize(const asw::Vec2<float>& size) {
  transform.size = size;
  return *this;
}

Button& Button::setImages(const std::string& image1,
                          const std::string& image2) {
  image = asw::assets::load_texture(image1);
  imageHover = asw::assets::load_texture(image2);
  set_texture(image);

  return *this;
}

Button& Button::setOnClick(const std::function<void(void)>& func) {
  onClick = func;

  return *this;
}

bool Button::isHovering() const {
  const auto& mouse = asw::input::get_mouse();
  return transform.contains(mouse.position);
}

void Button::update(float dt) {
  Sprite::update(dt);

  auto hovering = isHovering();

  if (hovering) {
    set_texture(imageHover);
  } else {
    set_texture(image);
  }

  if (onClick != nullptr && hovering &&
      asw::input::get_mouse_button_down(asw::input::MouseButton::Left)) {
    onClick();
  }
}
