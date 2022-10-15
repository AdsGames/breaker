#include "ParticleEmitter.h"

#include <asw/asw.h>

#include "utility/tools.h"

// Constructor basic
ParticleEmitter::ParticleEmitter() : ParticleEmitter(vec2(0, 0), vec2(0, 0)) {}

// Constructor with parameters
ParticleEmitter::ParticleEmitter(vec2 position, vec2 size)
    : position(position), size(size), launchVelocity(vec2(-10, 10)), type(0) {
  // Load image
  images[0] = asw::assets::loadTexture("assets/images/fuzzball.png");
  images[1] = asw::assets::loadTexture("assets/images/fuzzball2.png");
  images[2] = asw::assets::loadTexture("assets/images/fuzzball_blue.png");
}

// Create particles
void ParticleEmitter::createParticle() {
  Particle newPart(
      random(position.x, position.x + size.x),
      random(position.y, position.y + size.y),
      vec2(randomf(-0.1, 0.2), randomf(-0.4, -0.9)), vec2(0.008, -0.008),
      vec2(random(5, 10)), asw::util::makeColor(255, 255, 255),
      asw::util::makeColor(0, 0, 0), random(500, 1000), IMAGE, true);
  newPart.setImage(images[0]);
  particles.push_back(newPart);
}

// Set emitter position
void ParticleEmitter::setPosition(vec2 position) {
  this->position = position;
}

// Update
void ParticleEmitter::update(int dt) {
  // Update each particle
  for (unsigned int i = 0; i < particles.size(); i++) {
    if (particles.at(i).isDead() ||
        asw::input::keyboard.down[SDL_SCANCODE_BACKSPACE]) {
      particles.erase(particles.begin() + i);
    } else {
      particles.at(i).update(dt / 4);
    }
  }
}

// Draw
void ParticleEmitter::draw() {
  for (unsigned int i = 0; i < particles.size(); i++) {
    particles.at(i).draw();
  }
}
