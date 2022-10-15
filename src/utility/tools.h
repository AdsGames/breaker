#ifndef TOOLS_H
#define TOOLS_H

// Collision
extern bool collision(int xMin1,
                      int xMax1,
                      int xMin2,
                      int xMax2,
                      int yMin1,
                      int yMax1,
                      int yMin2,
                      int yMax2);

// Random number generator
extern int random(int low, int high);

// Random number generator floats
extern float randomf(float low, float high);

#endif  // TOOLS_H
