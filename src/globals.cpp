#include "globals.h"

#include <loadpng.h>
#include <logg.h>
#include <random>

// Random device
std::random_device rdev;

//Random number generator. Use int random(lowest,highest);
int random (int low, int high) {
  std::mt19937 rng(rdev());
  std::uniform_int_distribution<int> dist(low, high); // distribution in range [1, 6]
  return dist(rng);
}


// Random float
float randomf (float low, float high) {
  std::mt19937 rng(rdev());
  std::uniform_real_distribution<double> dist(low, high);
  return dist(rng);
}

// Error reporting
void abort_on_error (std::string message) {
  if (screen != NULL) {
    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  }

  allegro_message ("%s.\n %s\n", message.c_str(), allegro_error);
  exit (-1);
}

// Load image
BITMAP *load_png_ex (std::string path) {
  BITMAP *temp = nullptr;

  if (!(temp = load_png (path.c_str(), nullptr)))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load ogg
SAMPLE *load_ogg_ex (std::string path) {
  SAMPLE *temp = nullptr;

  if (!(temp = logg_load (path.c_str())))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load sample
SAMPLE *load_sample_ex (std::string path) {
  SAMPLE *temp = nullptr;

  if (!(temp = load_sample (path.c_str())))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load font
FONT *load_font_ex (std::string path) {
  FONT *temp = nullptr;

  if (!(temp = load_font (path.c_str(), nullptr, nullptr)))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
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
