/**
 * Image button helper
 * Adds an asw UI button with normal and hover images
 **/
#ifndef UI_IMAGE_BUTTON_H
#define UI_IMAGE_BUTTON_H

#include <asw/asw.h>
#include <functional>
#include <string>

// Add an image button to parent, sized to its image
asw::ui::Button& addImageButton(asw::ui::Widget& parent,
                                const asw::Vec2f& position,
                                const std::string& image,
                                const std::string& image_hover,
                                const std::function<void()>& on_click);

#endif  // UI_IMAGE_BUTTON_H
