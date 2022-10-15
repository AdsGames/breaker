#include "Block.h"

#include <math.h>

#include "utility/tools.h"

// Images
asw::Texture Block::images[8] = {nullptr};

// Default Constructor
Block::Block() : Block(0, 0, 0) {}

// Constructor
Block::Block(int x, int y, int type)
    : x(x), y(y), type(type), frame(0), selected(false) {
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
void Block::explode(ParticleEmitter& emitter) {
  // Number of particles to expode into
  int num_particles = 10;
  emitter.setPosition(vec2(x, y));

  for (int i = 0; i < num_particles; i++) {
    emitter.createParticle();
  }
}

// Draw block to screen
void Block::draw(int offset) {
  // Draw overlay if selected
  if (selected && int(floor(frame / 8)) == 1) {
    asw::draw::sprite(Block::images[7], x, y - offset);
  } else {
    asw::draw::sprite(Block::images[type], x, y - offset);
  }

  // Increase frame counter
  frame = (frame + 1) % 16;
}

// Get position
int Block::getX() const {
  return x;
}
int Block::getY() const {
  return y;
}

// Get width
int Block::getWidth() {
  auto size = asw::util::getTextureSize(images[0]);
  return images[0] ? size.x : 0;
}

int Block::getHeight() {
  auto size = asw::util::getTextureSize(images[0]);
  return images[0] ? size.y : 0;
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
