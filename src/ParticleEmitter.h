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

class ParticleEmitter {
 public:
  // Constructor
  ParticleEmitter() {
    image = asw::assets::loadTexture("assets/images/fuzzball.png");
  };

  // Set transform
  ParticleEmitter& setPosition(const asw::Vec2<float>& position);
  ParticleEmitter& setSize(const asw::Vec2<float>& size);

  // Create particle
  void createParticle();

  // Update
  void update(float deltaTime);

  // Draw
  void draw() const;

 private:
  // Emitter area
  asw::Quad<float> transform;

  // Create particle
  void createParticle(asw::Vec2<float> position);

  // Particles
  std::vector<Particle> particles;

  // Texture
  asw::Texture image;
};

#endif  // PARTICLE_EMITTER_H
