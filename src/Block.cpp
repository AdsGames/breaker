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
  images[0] = asw::assets::loadTexture("assets/images/blocks/red.png");
  images[1] = asw::assets::loadTexture("assets/images/blocks/orange.png");
  images[2] = asw::assets::loadTexture("assets/images/blocks/yellow.png");
  images[3] = asw::assets::loadTexture("assets/images/blocks/green.png");
  images[4] = asw::assets::loadTexture("assets/images/blocks/blue.png");
  images[5] = asw::assets::loadTexture("assets/images/blocks/purple.png");
  images[6] = asw::assets::loadTexture("assets/images/blocks/none.png");
  images[7] = asw::assets::loadTexture("assets/images/blocks/flash.png");
}

// Explode that block
void Block::explode(ParticleEmitter& emitter) const {
  // Number of particles to expode into
  const int num_particles = 10;
  emitter.setPosition(transform.position);
  emitter.setSize(transform.size);

  for (int i = 0; i < num_particles; i++) {
    emitter.createParticle();
  }
}

// Get position on screen
const asw::Quad<float>& Block::getTransform() const {
  return transform;
}

// Update
void Block::update(float deltaTime) {
  acc += deltaTime;

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
