#ifndef UTIL_BUTTON_H
#define UTIL_BUTTON_H

#include <asw/asw.h>
#include <functional>
#include <string>

class Button {
 public:
  Button();
  Button(int x, int y);

  void update() const;

  void setImages(const std::string& image1, const std::string& image2);

  int getX() const;
  int getY() const;

  void setOnClick(std::function<void()> func);

  void draw() const;

  bool isHovering() const;

 private:
  std::function<void(void)> onClick{nullptr};

  int x;
  int y;

  int width{10};
  int height{10};

  asw::Texture image{nullptr};
  asw::Texture imageHover{nullptr};
};

#endif
