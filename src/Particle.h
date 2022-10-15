/**
 * Particle
 * Allan Legemaate
 * 07/01/2017
 * A better particle
 **/
#ifndef PARTICLE_H
#define PARTICLE_H

#define CIRCLE 0
#define RECTANGLE 1
#define PIXEL 2
#define RANDOM 3
#define IMAGE 4

#include <asw/asw.h>
#include "vec2.h"

class Particle {
 public:
  Particle(int x = 0,
           int y = 0,
           vec2 velocity = vec2(0),
           vec2 acceleration = vec2(0),
           vec2 size = vec2(1),
           asw::Color colorStart = {255, 255, 255, 0},
           asw::Color colorEnd = {255, 255, 255, 0},
           int life = 100,
           char type = PIXEL,
           bool transLife = false);

  // Is dead
  bool isDead() const;

  // Set image
  void setImage(asw::Texture image);

  // Update
  void update(int dt);

  // Draw
  void draw();

  // Make new color
  void mixColors();

  // Position / size
  float x;
  float y;
  vec2 size;

  // Looks
  asw::Color colorStart;
  asw::Color colorEnd;
  asw::Color color;
  char type;
  bool transLife;

  asw::Texture image;

  // Behavior
  vec2 velocity;
  vec2 acceleration;
  int life;
  int lifeStart;

 private:
};

#endif
