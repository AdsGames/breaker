#include "Particle.h"

Particle& Particle::setPosition(const asw::Vec2<float>& position) {
  transform.position = position;
  return *this;
}

Particle& Particle::setSize(const asw::Vec2<float>& size) {
  transform.size = size;
  return *this;
}

Particle& Particle::setVelocity(const asw::Vec2<float>& velocity) {
  this->velocity = velocity;
  return *this;
}

Particle& Particle::setAcceleration(const asw::Vec2<float>& acceleration) {
  this->acceleration = acceleration;
  return *this;
}

Particle& Particle::setLife(float life) {
  this->life = life;
  return *this;
}

Particle& Particle::setImage(const asw::Texture& image) {
  this->image = image;
  return *this;
}

// Is dead
bool Particle::isDead() const {
  return life <= 0;
}

// Logic
void Particle::update(float deltaTime) {
  if (isDead()) {
    return;
  }

  life -= deltaTime;
  transform.position.x += velocity.x * deltaTime;
  transform.position.y += velocity.y * deltaTime;
  velocity.x += acceleration.x * deltaTime;
  velocity.y += acceleration.y * deltaTime;
}

// Draw
void Particle::draw() const {
  if (isDead()) {
    return;
  }

  auto image_size = asw::util::getTextureSize(image) / 2;
  asw::draw::sprite(image, transform.position + image_size);
}
