#include "ParticleEmitter.h"

#include <asw/asw.h>
#include <algorithm>

ParticleEmitter& ParticleEmitter::setPosition(
    const asw::Vec2<float>& position) {
  transform.position = position;
  return *this;
}

ParticleEmitter& ParticleEmitter::setSize(const asw::Vec2<float>& size) {
  transform.size = size;
  return *this;
}

// Create particles
void ParticleEmitter::createParticle() {
  auto part_size = asw::random::between(5.0F, 10.0F);
  auto random_x = asw::random::between(transform.position.x,
                                       transform.position.x + transform.size.x);
  auto random_y = asw::random::between(transform.position.y,
                                       transform.position.y + transform.size.y);

  const Particle particle =
      Particle()
          .setPosition({random_x, random_y})
          .setSize({part_size, part_size})
          .setVelocity(asw::Vec2<float>(asw::random::between(-0.1F, 0.2F),
                                        asw::random::between(-0.4F, -0.9F)))
          .setAcceleration(asw::Vec2<float>(0.008, -0.008))
          .setLife(asw::random::between(500.0F, 1000.0F))
          .setImage(image);

  particles.push_back(particle);
}

// Update
void ParticleEmitter::update(float deltaTime) {
  // Erase dead particles
  particles.erase(std::remove_if(particles.begin(), particles.end(),
                                 [](const Particle& particle) {
                                   return particle.isDead();
                                 }),
                  particles.end());

  // Update particles
  for (auto& particle : particles) {
    particle.update(deltaTime / 4.0F);
  }
}

// Draw
void ParticleEmitter::draw() const {
  for (const auto& particle : particles) {
    particle.draw();
  }
}
