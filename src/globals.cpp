#include "globals.h"

// Get distance between 2 points
float Get2dDistance( float x1, float y1, float x2, float y2){
	return sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
}

// Random number generator. Use int random(highest,lowest);
int random( int lowest, int highest){
  int random_number = (rand() % ((highest + 1) - lowest)) + lowest;
  return random_number;
}

// Function to check for collision
bool collision(float xMin1, float xMax1, float xMin2, float xMax2, float yMin1, float yMax1, float yMin2, float yMax2){
  if( xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}

// Random float
float randomf(float lowest, float highest){
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
int difficulty = 3;
