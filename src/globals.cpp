#include "globals.h"

#include <stdlib.h>

// Random number generator. Use int random(highest,lowest);
int random (int lowest, int highest) {
  int random_number = (rand() % ((highest + 1) - lowest)) + lowest;
  return random_number;
}

// Random float
float randomf (float lowest, float highest) {
  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = highest - lowest;
  float r = random * diff;
  return lowest + r;
}

// Config
bool config_sound = true;
int config_max_FPS = 100;
bool config_show_fps = false;
int config_screen_width = 1280;
int config_screen_height = 960;
bool config_fullscreen = false;
bool config_double_click = false;
int difficulty = 3;
