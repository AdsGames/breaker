#ifndef TOOLS_H
#define TOOLS_H

#include <allegro.h>
#include <loadpng.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Key or joy button pressed
extern bool key_down();
extern bool button_down();

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

// Error reporting
extern void abort_on_error(std::string message);

// Load image
extern BITMAP* load_png_ex(std::string path);

// Load ogg
extern SAMPLE* load_ogg_ex(std::string path);

// Load sample
extern SAMPLE* load_sample_ex(std::string path);

// Load font
extern FONT* load_font_ex(std::string path);

#endif  // TOOLS_H
