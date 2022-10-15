#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <asw/asw.h>
#include <string>

class InputBox {
 public:
  explicit InputBox(int x = 0,
                    int y = 0,
                    int width = 100,
                    int height = 20,
                    asw::Font font = nullptr,
                    const std::string& value = "",
                    const std::string& type = "text");

  void focus();

  std::string getValue() const;
  void update();
  void draw() const;
  bool hover() const;

 private:
  int x;
  int y;

  int width;
  int height;

  asw::Font font;

  std::string text;
  std::string type;

  unsigned int textIterator{0};
  bool focused{false};
};

#endif  // INPUTBOX_H
