#include "block.h"

#include <loadpng.h>

// Images
BITMAP *Block::images[8] = { nullptr };
int Block::block_count = 0;

// Default Constructor
Block::Block() {
  this -> x = 0;
  this -> y = 0;

  this -> frame = 0;
  this -> type = 0;

  this -> selected = false;

  if (!images[0]) {
    loadImages();
  }
  block_count++;
}

// Constructor
Block::Block(int x, int y, int type) :
  Block() {
  this -> x = x;
  this -> y = y;

  this -> type = type;
}

// Deconstructor
Block::~Block() {
  block_count--;
  if (block_count == 0) {
    for (int i = 0; i < 8; i++) {
      destroy_bitmap(images[i]);
      images[i] = nullptr;
    }
  }
}

// Sets block images
void Block::loadImages() {
  images[0] = load_png ("images/blocks/red.png", NULL);
  images[1] = load_png ("images/blocks/orange.png", NULL);
  images[2] = load_png ("images/blocks/yellow.png", NULL);
  images[3] = load_png ("images/blocks/green.png", NULL);
  images[4] = load_png ("images/blocks/blue.png", NULL);
  images[5] = load_png ("images/blocks/purple.png", NULL);
  images[6] = load_png ("images/blocks/none.png", NULL);
  images[7] = load_png ("images/blocks/flash.png", NULL);
}

// Explode that block
void Block::explode (particle_emitter &emitter) {
  // Number of particles to expode into
  int numberDivision = 5;

  // Select pixels to make into particles
  for (int i = 0; i < Block::images[type] -> w; i += numberDivision) {
    for (int t = 0; t < Block::images[type] -> h; t += numberDivision) {
      particle newPart (i + x, t + y,
                        vec2 (randomf (-0.2, 0.2), randomf (-0.2, 0.2)), vec2 (randomf (-0.2, 0.2), randomf (-0.2, 0.2)),
                        vec2 (random (2, 8)), getpixel (Block::images[type], i, t), getpixel (Block::images[type], i, t), random (5, 30), RECTANGLE, false);
      emitter.create_particle (newPart);
    }
  }
}

// Draw block to screen
void Block::draw (BITMAP *buffer, int offset) {
  // Draw overlay if selected
  if (selected && floor (frame / 8) == 1) {
    draw_sprite (buffer, Block::images[7], x, y - offset);
  }
  else {
    draw_sprite (buffer, Block::images[type], x, y - offset);
  }

  // Increase frame counter
  frame = (frame + 1) % 16;
}


// Get position
int Block::getX() {
  return x;
}
int Block::getY() {
  return y;
}

// Get width
int Block::getWidth() {
  return images[0] -> w;
}
int Block::getHeight() {
  return images[0] -> h;
}

// Get type
int Block::getType() {
  return type;
}

// Set type
void Block::setType(int type) {
  this -> type = type;
}

// Check if its selected
bool Block::getSelected() {
  return selected;
}

// Set wheather block is selected or not
void Block::setSelected (bool selected) {
  this -> selected = selected;
}
