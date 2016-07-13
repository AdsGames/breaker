#ifndef GLOBALS_H
#define GLOBALS_H

// Import libraries
#include <math.h>
#include <allegro.h>

float Get2dDistance(float x1, float y1, float x2, float y2);

int random(int newLowest, int newHighest);

bool collision(float xMin1, float xMax1, float xMin2, float xMax2, float yMin1, float yMax1, float yMin2, float yMax2);

void highcolor_fade_in(BITMAP* bmp_orig, int speed);
void highcolor_fade_out(int speed);

#endif
