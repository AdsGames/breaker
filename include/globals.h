/**
 * GLOBALS
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef GLOBALS_H
#define GLOBALS_H

int random (int lowest, int highest);

//Random number generator
extern float randomf (float lowest, float highest);

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
