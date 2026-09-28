#include "Block.h"

#include <algorithm>
#include <cmath>

namespace {
// Gravity applied to falling blocks, in px/s^2.
constexpr float FALL_GRAVITY = 6000.0F;
// Exponential ease rate for the horizontal slide.
constexpr float SLIDE_SPEED = 12.0F;
}  // namespace

// Images
std::array<asw::Texture, Block::NUM_TYPES> Block::images = {nullptr};

// Constructor
Block::Block(const asw::Vec2<float>& position, int type)
    : transform(position, {SIZE, SIZE}), type(type) {
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
  images[TYPE_EMPTY] = asw::assets::load_texture("assets/images/blocks/none.png");
  images[TYPE_FLASH] = asw::assets::load_texture("assets/images/blocks/flash.png");
}

// Get position on screen
const asw::Quad<float>& Block::getTransform() const {
  return transform;
}

// Update
void Block::update(float dt) {
  acc += dt;

  // Increase frame counter
  if (acc > 0.032) {
    frame = (frame + 1) % 16;
    acc -= 0.032;
  }

  // Ease the visual offset back to rest. Vertical uses gravity so blocks
  // accelerate as they fall; horizontal uses an exponential ease.
  if (visual_offset.y < 0.0F) {
    fall_velocity += FALL_GRAVITY * dt;
    visual_offset.y += fall_velocity * dt;

    if (visual_offset.y >= 0.0F) {
      visual_offset.y = 0.0F;
      fall_velocity = 0.0F;
    }
  }

  if (visual_offset.x != 0.0F) {
    visual_offset.x -= visual_offset.x * std::min(1.0F, dt * SLIDE_SPEED);

    if (std::abs(visual_offset.x) < 0.5F) {
      visual_offset.x = 0.0F;
    }
  }
}

// Draw block to screen
void Block::draw(float offset) const {
  auto position =
      transform.position + visual_offset - asw::Vec2<float>(0, offset);

  // Draw overlay if selected
  if (selected && int(floor(frame / 8)) == 1) {
    asw::draw::sprite(Block::images[TYPE_FLASH], position);
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

// Displace from resting position (reset fall velocity for a fresh fall)
void Block::setVisualOffset(const asw::Vec2<float>& offset) {
  visual_offset = offset;
  fall_velocity = 0.0F;
}

const asw::Vec2<float>& Block::getVisualOffset() const {
  return visual_offset;
}
