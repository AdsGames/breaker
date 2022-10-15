#include "InputBox.h"

InputBox::InputBox(int x,
                   int y,
                   int width,
                   int height,
                   asw::Font font,
                   const std::string& value,
                   const std::string& type)
    : x(x),
      y(y),
      width(width),
      height(height),
      font(font),
      text(value),
      type(type) {}

void InputBox::focus() {
  focused = true;
}

std::string InputBox::getValue() const {
  return text;
}

bool InputBox::hover() const {
  return asw::input::mouse.x > x && asw::input::mouse.x < x + width &&
         asw::input::mouse.y > y && asw::input::mouse.y < y + height;
}

void InputBox::update() {
  // Focus
  if (asw::input::mouse.pressed[1]) {
    focused = hover();

    if (focused) {
      int closest = width;

      for (unsigned int i = 0; i <= text.length(); i++) {
        int textSize = asw::util::getTextSize(font, text.substr(0, i)).x;

        int distance = abs(textSize + x + 6 - asw::input::mouse.x);

        if (distance < closest) {
          textIterator = i;
          closest = distance;
        }
      }
    }
  }

  int lastKey = asw::input::keyboard.lastPressed;

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
  asw::draw::rectFill(x, y, width, height, asw::util::makeColor(12, 12, 12));

  asw::Color col = (hover() || focused) ? asw::util::makeColor(230, 230, 230)
                                        : asw::util::makeColor(245, 245, 245);

  if (focused) {
    asw::draw::rectFill(x + 2, y + 2, width - 4, height - 4, col);
  } else {
    asw::draw::rectFill(x + 1, y + 1, width - 2, height - 2, col);
  }

  // Output the string to the screen
  asw::draw::text(font, text, x + 6, y, asw::util::makeColor(22, 22, 22));

  // Draw the caret
  if (focused) {
    int textSize = asw::util::getTextSize(font, text.substr(0, textIterator)).x;

    asw::draw::rectFill(textSize + x + 6, y + 8, 1, height - 16,
                        asw::util::makeColor(0, 0, 0));
  }
}
