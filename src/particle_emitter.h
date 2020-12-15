/**
 * Particle Emitter
 * Allan Legemaate
 * 07/01/2017
 * A better particle system
 **/
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <vector>

#include "particle.h"
#include "vec2.h"

class particle_emitter {
 public:
  // Constructor
  particle_emitter();
  particle_emitter(vec2 position, vec2 size);

  // Destructor
  virtual ~particle_emitter();

  // Create particle
  void create_particle();

  // Set position
  void set_position(vec2 position);

  // Update
  void update(int dt);

  // Draw
  void draw(BITMAP* buffer);

 private:
  // Emitter area
  vec2 position;
  vec2 size;

  // Velocity
  vec2 launch_velocity;

  // Type
  int type;

  // Fuzzy image
  BITMAP* images[3];

  // Create particle
  void create_particle(int x, int y);

  // Particles
  std::vector<particle> particles;
};

#endif  // PARTICLE_EMITTER_H
