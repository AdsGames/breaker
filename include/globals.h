/**
 * GLOBALS
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef GLOBALS_H
#define GLOBALS_H

#include <allegro.h>
#include <string>

//Random number generator
extern int random (int low, int high);

//Random number generator floats
extern float randomf (float low, float high);

// Load image
extern BITMAP *load_png_ex (std::string path);

// Load ogg
extern SAMPLE *load_ogg_ex (std::string path);

// Load sample
extern SAMPLE *load_sample_ex (std::string path);

// Load font
extern FONT *load_font_ex (std::string path);

// Abort on error
extern void abort_on_error (std::string message);

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
