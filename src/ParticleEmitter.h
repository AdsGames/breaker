/**
 * Particle Emitter
 * Allan Legemaate
 * 07/01/2017
 * A better particle system
 **/
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <vector>

#include "Particle.h"
#include "vec2.h"

class ParticleEmitter {
 public:
  // Constructor
  ParticleEmitter();
  ParticleEmitter(vec2 position, vec2 size);

  // Create particle
  void createParticle();

  // Set position
  void setPosition(vec2 position);

  // Update
  void update(int dt);

  // Draw
  void draw();

 private:
  // Emitter area
  vec2 position;
  vec2 size;

  // Velocity
  vec2 launchVelocity;

  // Type
  int type;

  // Fuzzy image
  asw::Texture images[3];

  // Create particle
  void createParticle(int x, int y);

  // Particles
  std::vector<Particle> particles;
};

#endif  // PARTICLE_EMITTER_H
