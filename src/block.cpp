#include "block.h"

// Images
BITMAP *Block::images[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

// Constructor
Block::Block() {
  this -> x = 0;
  this -> y = 0;

  this -> frame = 0;
  this -> type = 0;

  this -> selected = false;
}

// Deconstructor
Block::~Block() {

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
