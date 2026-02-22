#include "Block.h"

// Images
std::array<asw::Texture, 8> Block::images = {nullptr};

// Constructor
Block::Block(const asw::Vec2<float>& position, int type)
    : transform(position, {80, 80}), type(type) {
  if (!images[0]) {
    loadImages();
  }
}

// Sets block images
void Block::loadImages() {
  images[0] = asw::assets::load_texture("assets/images/blocks/red.png");
  images[1] = asw::assets::load_texture("assets/images/blocks/orange.png");
  images[2] = asw::assets::load_texture("assets/images/blocks/yellow.png");
  images[3] = asw::assets::load_texture("assets/images/blocks/green.png");
  images[4] = asw::assets::load_texture("assets/images/blocks/blue.png");
  images[5] = asw::assets::load_texture("assets/images/blocks/purple.png");
  images[6] = asw::assets::load_texture("assets/images/blocks/none.png");
  images[7] = asw::assets::load_texture("assets/images/blocks/flash.png");
}

// Get position on screen
const asw::Quad<float>& Block::getTransform() const {
  return transform;
}

// Update
void Block::update(float dt) {
  acc += dt;

  // Increase frame counter
  if (acc > 32) {
    frame = (frame + 1) % 16;
    acc -= 32;
  }
}

// Draw block to screen
void Block::draw(float offset) const {
  auto position = transform.position - asw::Vec2<float>(0, offset);

  // Draw overlay if selected
  if (selected && int(floor(frame / 8)) == 1) {
    asw::draw::sprite(Block::images[7], position);
  } else {
    asw::draw::sprite(Block::images[type], position);
  }
}

// Get type
int Block::getType() const {
  return type;
}

// Set type
void Block::setType(int type) {
  this->type = type;
}

// Check if its selected
bool Block::getSelected() const {
  return selected;
}

// Set wheather block is selected or not
void Block::setSelected(bool selected) {
  this->selected = selected;
}
