#include "Particle.h"

// Constructor
Particle::Particle(int x,
                   int y,
                   vec2 velocity,
                   vec2 acceleration,
                   vec2 size,
                   asw::Color colorStart,
                   asw::Color colorEnd,
                   int life,
                   char type,
                   bool transLife) {
  this->x = x;
  this->y = y;

  this->velocity = velocity;
  this->acceleration = acceleration;

  this->colorStart = colorStart;
  this->colorEnd = colorEnd;
  this->color = colorStart;

  this->size = size;

  this->life = life;
  this->lifeStart = life;

  this->image = nullptr;

  this->type = type;
  this->transLife = transLife;

  mixColors();
}

// Make new color
void Particle::mixColors() {
  // For readability
  int c_red =
      (colorStart.r * life + colorEnd.r * (lifeStart - life)) / lifeStart;
  int c_green =
      (colorStart.g * life + colorEnd.g * (lifeStart - life)) / lifeStart;
  int c_blue =
      (colorStart.b * life + colorEnd.b * (lifeStart - life)) / lifeStart;
  int c_alpha =
      (colorStart.a * life + colorEnd.a * (lifeStart - life)) / lifeStart;

  this->color = asw::util::makeColor(c_red, c_green, c_blue, c_alpha);
}

// Is dead
bool Particle::isDead() const {
  return (life <= 0);
}

// Set image
void Particle::setImage(asw::Texture image) {
  this->image = image;
  this->type = IMAGE;
}

// Logic
void Particle::update(int dt) {
  if (!isDead()) {
    life -= dt;
    x += velocity.x * dt;
    y += velocity.y * dt;
    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;
    mixColors();
  }
}

// Draw
void Particle::draw() {
  if (!isDead()) {
    // if (transLife) {
    //   set_trans_blender(255, 255, 255, int(float(life) / lifeStart * 255));
    // }

    if (type == IMAGE && image != nullptr) {
      auto imageSize = asw::util::getTextureSize(image);
      asw::draw::sprite(image, x - imageSize.x / 2, y - imageSize.y / 2);
    } else if (type == PIXEL) {
      asw::draw::point(x, y, color);
    } else if (type == CIRCLE) {
      asw::draw::circleFill(x, y, size.x / 2, color);
    } else if (type == RECTANGLE) {
      asw::draw::rectFill(x, y, size.x, size.y, color);
    }
  }
}
