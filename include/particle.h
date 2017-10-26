/*
 * Particle
 * A simple particle for use in particle systems
 * Allan Legemaate
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#define PIXEL 0
#define SQUARE 1
#define CIRCLE 2
#define RANDOM 3

#include <allegro.h>

#include "globals.h"

class particle{
  public:
    particle(int newX, int newY, int newColor, int newXMin, int newXMax, int newYMin, int newYMax, char newType, int newSize);
    ~particle();

    void logic();
    void draw(BITMAP* tempBitmap);

  private:
    BITMAP* image[2];

    int x;
    int y;

    int particleColor;
    int particleRed;
    int particleGreen;
    int particleBlue;

    char type;
    int size;

    int xMax;
    int xMin;
    int yMin;
    int yMax;
};

#endif

