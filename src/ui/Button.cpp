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
  transform.size = asw::util::getTextureSize(image);

  return *this;
}

Button& Button::setOnClick(const std::function<void(void)>& func) {
  onClick = func;

  return *this;
}

bool Button::isHovering() const {
  return transform.contains({asw::input::mouse.x, asw::input::mouse.y});
}

void Button::update() const {
  if (onClick == nullptr) {
    return;
  }

  if (isHovering() &&
      asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
    onClick();
  }
}

void Button::draw() const {
  auto hovering = isHovering();

  if (hovering && imageHover) {
    asw::draw::sprite(imageHover, transform.position);
  } else if (!hovering && image) {
    asw::draw::sprite(image, transform.position);
  } else {
    asw::draw::rectFill(transform, asw::util::makeColor(60, 60, 60));
  }
}
