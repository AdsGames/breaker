/**
 * GLOBALS
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef GLOBALS_H
#define GLOBALS_H

// Import libraries
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>

float Get2dDistance(float x1, float y1, float x2, float y2);

int random(int lowest, int highest);

bool collision(float xMin1, float xMax1, float xMin2, float xMax2, float yMin1, float yMax1, float yMin2, float yMax2);

//Random number generator
extern float randomf(float lowest, float highest);

// Config
extern bool config_sound;
extern int config_max_FPS;
extern bool config_show_fps;
extern int config_screen_width;
extern int config_screen_height;
extern bool config_fullscreen;
extern bool config_double_click;
extern int difficulty;

#endif
