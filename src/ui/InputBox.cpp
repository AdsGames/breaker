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
  const auto& mouse = asw::input::get_mouse();
  const auto& keyboard = asw::input::get_keyboard();
  auto hovering = transform.contains(mouse.position);

  // Focus
  if (asw::input::get_mouse_button_down(asw::input::MouseButton::Left)) {
    focused = hovering;

    if (focused) {
      int closest = transform.size.x;

      for (unsigned int i = 0; i <= text.length(); i++) {
        const int textSize =
            asw::util::get_text_size(font, text.substr(0, i)).x;

        const int distance =
            std::abs(textSize + transform.position.x + 6 - mouse.position.x);

        if (distance < closest) {
          textIterator = i;
          closest = distance;
        }
      }
    }
  }

  const int lastKey = keyboard.last_pressed;

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
    if (asw::input::get_key(asw::input::Key::LShift) ||
        asw::input::get_key(asw::input::Key::RShift)) {
      text.insert(text.begin() + textIterator, 'A' - 4 + lastKey);
    } else {
      text.insert(text.begin() + textIterator, 'a' - 4 + lastKey);
    }

    textIterator++;
  }

  // some other, "special" key was pressed; handle it here
  if (asw::input::get_key_down(asw::input::Key::Backspace) &&
      textIterator != 0) {
    textIterator--;
    text.erase(text.begin() + textIterator);
  }

  if (asw::input::get_key_down(asw::input::Key::Right) &&
      textIterator != text.size()) {
    textIterator++;
  }

  if (asw::input::get_key_down(asw::input::Key::Left) && textIterator != 0) {
    textIterator--;
  }
}

// Draw box
void InputBox::draw() const {
  asw::draw::rect_fill(transform, asw::Color(12, 12, 12));
  const auto& mouse = asw::input::get_mouse();

  const auto hovering = transform.contains(mouse.position);

  const auto col = (hovering || focused) ? asw::Color(230, 230, 230)
                                         : asw::Color(245, 245, 245);

  if (focused) {
    asw::draw::rect_fill(transform + asw::Quad<float>(2, 2, -4, -4), col);
  } else {
    asw::draw::rect_fill(transform + asw::Quad<float>(1, 1, -2, -2), col);
  }

  // Output the string to the screen
  asw::draw::text(font, text, transform.position + asw::Vec2<float>(6, 0),
                  asw::Color(22, 22, 22));

  // Draw the caret
  if (focused) {
    const int textSize =
        asw::util::get_text_size(font, text.substr(0, textIterator)).x;

    auto caratPosition =
        asw::Quad<float>(textSize + transform.position.x + 6,
                         transform.position.y + 8, 1, transform.size.y - 16);

    asw::draw::rect_fill(caratPosition, asw::Color(0, 0, 0));
  }
}
