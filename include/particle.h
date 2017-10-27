/*
 * Particle
 * Allan Legemaate
 * 07/01/2017
 * A better particle
 */
#ifndef PARTICLE_H
#define PARTICLE_H

#define CIRCLE 0
#define RECTANGLE 1
#define PIXEL 2
#define RANDOM 3
#define IMAGE 4

#include <allegro.h>
#include <alleggl.h>
#include "vec2.h"

class particle{
  public:
    // Constructor
    particle( int x, int y, vec2 velocity, vec2 acceleration, vec2 size, int colorStart, int colorEnd, int life, char type, bool trans_life);

    // Destructor
    ~particle() {};

    // Is dead
    bool dead();

    // Set image
    void set_image( BITMAP *image);

    // Set particle to spawn on death
    void set_particle_ondeath( particle *onDeath);

    // Update
    void update( int dt);

    // Draw
    void draw(BITMAP *tempBitmap);



    // Make new color
    void mix_colors();

    // Position / size
    float x;
    float y;
    vec2 size;

    // Looks
    int colorStart;
    int colorEnd;
    int color;
    char type;
    bool trans_life;

    BITMAP* image;

    particle *onDeath;

    // Behavior
    vec2 velocity;
    vec2 acceleration;
    int life;
    int life_start;

  private:

};

#endif

