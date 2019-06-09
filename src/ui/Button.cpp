#include "ui/Button.h"

#include "utility/tools.h"
#include "utility/MouseListener.h"

Button::Button ()
  : Button(0, 0) {

}

Button::Button (int x, int y)
  : OnClick(nullptr),
    x(x),
    y(y),
    width(10),
    height(10) {

  images[0] = nullptr;
  images[1] = nullptr;
}

Button::~Button() {
  destroy_bitmap (images[0]);
  destroy_bitmap (images[1]);
}

void Button::SetOnClick (std::function<void(void)> func) {
  OnClick = func;
}

// Load images from file
void Button::SetImages (const char *image1, const char *image2) {
  images[0] = load_png_ex (image1);
  images[1] = load_png_ex (image2);

  // Size
  height = images[0] -> h;
  width = images[0] -> w;
}

bool Button::Hover() {
  return collision (MouseListener::x, MouseListener::x, x, x + width,
                    MouseListener::y, MouseListener::y, y, y + height);
}

void Button::Update() {
  if (Hover() && MouseListener::mouse_pressed & 1 && OnClick != nullptr)
    OnClick();
}

void Button::Draw (BITMAP *buffer) {
  if (images[Hover()]) {
    draw_sprite (buffer, images[Hover()], x, y);
  }
  else {
    rectfill (buffer, x, y, x + width, y + height, 0x999999);
  }
}
