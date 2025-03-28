/**
 * Particle
 * Allan Legemaate
 * 07/01/2017
 * A better particle
 **/
#ifndef PARTICLE_H
#define PARTICLE_H

#include <asw/asw.h>

class Particle {
 public:
  Particle() = default;

  // Chaining setters
  Particle& setPosition(const asw::Vec2<float>& position);
  Particle& setSize(const asw::Vec2<float>& size);
  Particle& setVelocity(const asw::Vec2<float>& velocity);
  Particle& setAcceleration(const asw::Vec2<float>& acceleration);
  Particle& setLife(float life);
  Particle& setImage(const asw::Texture& image);

  // Is dead
  bool isDead() const;

  // Update
  void update(float deltaTime);

  // Draw
  void draw() const;

 private:
  /// @brief Transform coordinates
  asw::Quad<float> transform;

  /// @brief Image
  asw::Texture image;

  // Behavior
  asw::Vec2<float> velocity{0, 0};
  asw::Vec2<float> acceleration{0, 0};
  float life{100.0F};
};

#endif
