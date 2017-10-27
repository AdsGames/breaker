/*
 * Particle Emitter
 * Allan Legemaate
 * 07/01/2017
 * A better particle system
 */
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <vector>

#include "vec2.h"
#include "particle.h"
#include "globals.h"

class particle_emitter{
  public:
    // Constructor
    particle_emitter();
    particle_emitter( vec2 position, vec2 size);

    // Destructor
    virtual ~particle_emitter();

    // Create particle
    void create_particle( int type);
    void create_particle( particle newPart);

    // Move
    void move_to( vec2 position);

    // Size
    void set_size( vec2 size){ this -> size = size; };

    // Clear
    void clear_all(){ particles.clear(); };

    // Update
    void update( int dt);

    // Get size
    int get_size(){ return particles.size(); }

    // Draw
    void draw( BITMAP *temp_image);
  protected:

  private:
    // Fuzzy image
    BITMAP *image;
    BITMAP *image2;
    BITMAP *image3;

    // Velocity
    vec2 launch_velocity;

    // Emitter area
    vec2 position;
    vec2 size;

    // Create particle
    void create_particle( int x, int y);

    int type;

    // Particles
    std::vector <particle> particles;
};

#endif // PARTICLE_EMITTER_H
