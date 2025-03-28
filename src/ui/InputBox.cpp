#include "InputBox.h"

#include <cmath>

InputBox::InputBox(const asw::Quad<float>& position,
                   const asw::Font& font,
                   const std::string& value,
                   const std::string& type)
    : transform(position), font(font), text(value), type(type) {}

void InputBox::focus() {
  focused = true;
}

std::string InputBox::getValue() const {
  return text;
}

void InputBox::update() {
  auto hovering =
      transform.contains({asw::input::mouse.x, asw::input::mouse.y});

  // Focus
  if (asw::input::isButtonDown(asw::input::MouseButton::LEFT)) {
    focused = hovering;

    if (focused) {
      int closest = transform.size.x;

      for (unsigned int i = 0; i <= text.length(); i++) {
        const int textSize = asw::util::getTextSize(font, text.substr(0, i)).x;

        const int distance =
            std::abs(textSize + transform.position.x + 6 - asw::input::mouse.x);

        if (distance < closest) {
          textIterator = i;
          closest = distance;
        }
      }
    }
  }

  const int lastKey = asw::input::keyboard.lastPressed;

  if (!focused || lastKey == -1) {
    return;
  }

  // a character key was pressed; add it to the string
  if (type == "number" || type == "text") {
    // Numeric only
    if (lastKey >= 30 && lastKey <= 38) {
      text.insert(text.begin() + textIterator, lastKey + 19);
      textIterator++;
    }

    if (lastKey == 39) {
      text.insert(text.begin() + textIterator, lastKey + 9);
      textIterator++;
    }
  }

  if (type == "text" && lastKey >= 4 && lastKey <= 29) {
    if (asw::input::keyboard.down[SDL_SCANCODE_LSHIFT] ||
        asw::input::keyboard.down[SDL_SCANCODE_RSHIFT]) {
      text.insert(text.begin() + textIterator, 'A' - 4 + lastKey);
    } else {
      text.insert(text.begin() + textIterator, 'a' - 4 + lastKey);
    }

    textIterator++;
  }

  // some other, "special" key was pressed; handle it here
  if (asw::input::keyboard.pressed[SDL_SCANCODE_BACKSPACE] &&
      textIterator != 0) {
    textIterator--;
    text.erase(text.begin() + textIterator);
  }

  if (asw::input::keyboard.pressed[SDL_SCANCODE_RIGHT] &&
      textIterator != text.size()) {
    textIterator++;
  }

  if (asw::input::keyboard.pressed[SDL_SCANCODE_LEFT] && textIterator != 0) {
    textIterator--;
  }
}

// Draw box
void InputBox::draw() const {
  asw::draw::rectFill(transform, asw::util::makeColor(12, 12, 12));

  const auto hovering =
      transform.contains({asw::input::mouse.x, asw::input::mouse.y});

  const auto col = (hovering || focused) ? asw::util::makeColor(230, 230, 230)
                                         : asw::util::makeColor(245, 245, 245);

  if (focused) {
    asw::draw::rectFill(transform + asw::Quad<float>(2, 2, -4, -4), col);
  } else {
    asw::draw::rectFill(transform + asw::Quad<float>(1, 1, -2, -2), col);
  }

  // Output the string to the screen
  asw::draw::text(font, text, transform.position + asw::Vec2<float>(6, 0),
                  asw::util::makeColor(22, 22, 22));

  // Draw the caret
  if (focused) {
    const int textSize =
        asw::util::getTextSize(font, text.substr(0, textIterator)).x;

    auto caratPosition =
        asw::Quad<float>(textSize + transform.position.x + 6,
                         transform.position.y + 8, 1, transform.size.y - 16);

    asw::draw::rectFill(caratPosition, asw::util::makeColor(0, 0, 0));
  }
}
