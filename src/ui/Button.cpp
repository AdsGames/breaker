#include "Button.h"

Button::Button() : Button(0, 0) {}

Button::Button(int x, int y) : x(x), y(y) {}

void Button::setOnClick(std::function<void(void)> func) {
  onClick = func;
}

// Load images from file
void Button::setImages(const std::string& image1, const std::string& image2) {
  image = asw::assets::loadTexture(image1);
  imageHover = asw::assets::loadTexture(image2);

  // Size
  auto size = asw::util::getTextureSize(image);
  height = size.y;
  width = size.x;
}

bool Button::isHovering() const {
  return asw::input::mouse.x > x && asw::input::mouse.x < x + width &&
         asw::input::mouse.y > y && asw::input::mouse.y < y + height;
}

void Button::update() const {
  if (isHovering() && asw::input::mouse.pressed[1] && onClick != nullptr) {
    onClick();
  }
}

int Button::getX() const {
  return x;
}

int Button::getY() const {
  return y;
}

void Button::draw() const {
  if (isHovering() && imageHover) {
    asw::draw::sprite(imageHover, x, y);
  } else if (!isHovering() && image) {
    asw::draw::sprite(image, x, y);
  } else {
    asw::draw::rectFill(x, y, x + width, y + height,
                        asw::util::makeColor(60, 60, 60));
  }
}
