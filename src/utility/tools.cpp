#include "tools.h"
#include "../globals.h"

#include <logg.h>
#include <random>

// Random device
std::mt19937 rng(time(nullptr));

// Iterates through the number of buttons in a joystick and returns true if any
// keys are pressed
bool key_down() {
  for (int i = 0; i < KEY_MAX; i++) {
    if (key[i]) {
      return true;
    }
  }

  return false;
}

// Iterates through the number of buttons in a joystick and returns true if any
// buttons are pressed
bool button_down() {
  for (int i = 0; i < joy[0].num_buttons; i++) {
    if (joy[0].button[i].b) {
      return true;
    }
  }

  return false;
}

// Collision
bool collision(int xMin1,
               int xMax1,
               int xMin2,
               int xMax2,
               int yMin1,
               int yMax1,
               int yMin2,
               int yMax2) {
  return (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1);
}

// Random number generator. Use int random(lowest,highest);
int random(int low, int high) {
  std::uniform_int_distribution<int> dist6(
      low, high);  // distribution in range [1, 6]
  return dist6(rng);
}

// Random float
float randomf(float low, float high) {
  std::uniform_real_distribution<double> dist(low, high);
  return dist(rng);
}

// Error reporting
void abort_on_error(std::string message) {
  if (screen != NULL) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  }

  allegro_message("%s.\n %s\n", message.c_str(), allegro_error);
  exit(-1);
}

// Load image
BITMAP* load_png_ex(std::string path) {
  BITMAP* temp = nullptr;

  if (!(temp = load_png(path.c_str(), nullptr)))
    abort_on_error("Cannot find image (" + path +
                   ") \n Please check your files and try again");

  return temp;
}

// Load ogg
SAMPLE* load_ogg_ex(std::string path) {
  SAMPLE* temp = nullptr;

  if (!(temp = logg_load(path.c_str())))
    abort_on_error("Cannot find image (" + path +
                   ") \n Please check your files and try again");

  return temp;
}

// Load sample
SAMPLE* load_sample_ex(std::string path) {
  SAMPLE* temp = nullptr;

  if (!(temp = load_sample(path.c_str())))
    abort_on_error("Cannot find image (" + path +
                   ") \n Please check your files and try again");

  return temp;
}

// Load font
FONT* load_font_ex(std::string path) {
  FONT* temp = nullptr;

  if (!(temp = load_font(path.c_str(), nullptr, nullptr)))
    abort_on_error("Cannot find image (" + path +
                   ") \n Please check your files and try again");

  return temp;
}
