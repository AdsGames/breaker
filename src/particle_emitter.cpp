#include "particle_emitter.h"

#include <allegro.h>

#include "utility/tools.h"

// Constructor basic
particle_emitter::particle_emitter()
  : particle_emitter(vec2(0, 0), vec2(0, 0)) {
}

// Constructor with parameters
particle_emitter::particle_emitter (vec2 position, vec2 size)
  : position(position),
    size(size),
    launch_velocity(vec2 (-10, 10)),
    type(0) {

  // Load image
  images[0] = load_png_ex ("images/fuzzball.png");
  images[1] = load_png_ex ("images/fuzzball2.png");
  images[2] = load_png_ex ("images/fuzzball_blue.png");
}

// Destructor
particle_emitter::~particle_emitter() {

}

// Create particles
void particle_emitter::create_particle () {
  particle newPart (random (position.x, position.x + size.x), random (position.y, position.y + size.y),
                    vec2 (randomf (-0.1, 0.2), randomf (-0.4, -0.9)), vec2 (0.008, -0.008),
                    vec2 (random (5, 10)), 0xFFFFFF, 0x000000, random (500, 1000), IMAGE, true);
  newPart.set_image (images[0]);
  particles.push_back (newPart);
}

// Set emitter position
void particle_emitter::set_position(vec2 position) {
  this -> position = position;
}

// Update
void particle_emitter::update (int dt) {
  // Update each particle
  for (unsigned int i = 0; i < particles.size(); i++) {
    if (particles.at (i).dead() || key[KEY_BACKSPACE]) {
      particles.erase (particles.begin() + i);
    }
    else {
      particles.at (i).update (dt / 4);
    }
  }
}

// Draw
void particle_emitter::draw (BITMAP *buffer) {
  for (unsigned int i = 0; i < particles.size(); i++) {
    particles.at (i).draw (buffer);
  }
}

