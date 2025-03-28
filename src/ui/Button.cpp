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
  image = asw::assets::loadTexture(image1);
  imageHover = asw::assets::loadTexture(image2);
  setTexture(image);

  return *this;
}

Button& Button::setOnClick(const std::function<void(void)>& func) {
  onClick = func;

  return *this;
}

bool Button::isHovering() const {
  return transform.contains({asw::input::mouse.x, asw::input::mouse.y});
}

void Button::update(float deltaTime) {
  Sprite::update(deltaTime);

  auto hovering = isHovering();

  if (hovering) {
    setTexture(imageHover);
  } else {
    setTexture(image);
  }

  if (onClick != nullptr && hovering &&
      asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
    onClick();
  }
}
