#ifndef UTIL_BUTTON_H
#define UTIL_BUTTON_H

#include <asw/asw.h>
#include <functional>
#include <string>

class Button : public asw::game::Sprite {
 public:
  Button() = default;

  // Chaining
  Button& setOnClick(const std::function<void()>& func);
  Button& setPosition(const asw::Vec2<float>& position);
  Button& setSize(const asw::Vec2<float>& size);
  Button& setImages(const std::string& image1, const std::string& image2);

  // Methods
  void update(float deltaTime) override;

  bool isHovering() const;

 private:
  std::function<void(void)> onClick{nullptr};

  asw::Texture image{nullptr};
  asw::Texture imageHover{nullptr};
};

#endif
