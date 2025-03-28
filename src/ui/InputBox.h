#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <asw/asw.h>
#include <string>

class InputBox {
 public:
  explicit InputBox(const asw::Quad<float>& position = {0, 0, 100, 20},
                    const asw::Font& font = nullptr,
                    const std::string& value = "",
                    const std::string& type = "text");

  void focus();

  std::string getValue() const;
  void update();
  void draw() const;

 private:
  asw::Quad<float> transform;

  asw::Font font;

  std::string text;
  std::string type;

  unsigned int textIterator{0};
  bool focused{false};
};

#endif  // INPUTBOX_H
