#include "ImageButton.h"

asw::ui::Button& addImageButton(asw::ui::Widget& parent,
                                const asw::Vec2f& position,
                                const std::string& image,
                                const std::string& image_hover,
                                const std::function<void()>& on_click) {
  auto& button = parent.add_child<asw::ui::Button>();
  button.set_images(asw::assets::load_texture(image),
                    asw::assets::load_texture(image_hover));

  // Own style so the theme's text button border is not drawn over the image
  button.style = asw::ui::ButtonStyle{};
  button.transform.position = position;
  button.on_click = on_click;
  return button;
}
