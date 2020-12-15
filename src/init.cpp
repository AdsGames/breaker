#include "init.h"

#include <logg.h>

#include "globals.h"
#include "utility/MouseListener.h"
#include "utility/tools.h"

// Init state (and game)
init::init() {
  // Initializing
  allegro_init();

  install_timer();
  install_mouse();
  install_keyboard();

  set_color_depth(32);

  set_window_title("Breaker");

  // Load config
  int count;
  push_config_state();
  set_config_file("config.ini");
  char** data = nullptr;

  data = get_config_argv("audio", "sound", &count);
  config_sound = count == 1 ? atoi(data[0]) : 0;

  data = get_config_argv("graphics", "fpscap", &count);
  config_max_FPS = count == 1 ? atoi(data[0]) : 100;

  data = get_config_argv("graphics", "width", &count);
  config_screen_width = count == 1 ? atoi(data[0]) : 1280;

  data = get_config_argv("graphics", "height", &count);
  config_screen_height = count == 1 ? atoi(data[0]) : 960;

  data = get_config_argv("graphics", "fullscreen", &count);
  config_fullscreen = count == 1 ? atoi(data[0]) : false;

  data = get_config_argv("graphics", "showfps", &count);
  config_show_fps = count == 1 ? atoi(data[0]) : false;

  data = get_config_argv("gameplay", "doubleclick", &count);
  config_double_click = count == 1 ? atoi(data[0]) : false;

  pop_config_state();

  // Init sound
  if (config_sound) {
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ".");
  }

  // Set screenmode
  if (config_fullscreen) {
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, config_screen_width,
                     config_screen_height, 0, 0) != 0) {
      if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1280, 960, 0, 0) != 0) {
        if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) != 0) {
          set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          allegro_message("Unable to go into fullscreen graphic mode\n%s\n",
                          allegro_error);
          exit(1);
        }
      }
    }
  } else {
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, config_screen_width,
                     config_screen_height, 0, 0) != 0) {
      if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0) != 0) {
        if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
          set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          allegro_message("Unable to go into windowed graphic mode\n%s\n",
                          allegro_error);
          exit(1);
        }
      }
    }
  }

  // Run in background
  set_display_switch_mode(SWITCH_BACKGROUND);

  // Sets Font
  font = load_font_ex("fonts/arial_black.pcx");

  // Background Music
  if (config_sound) {
    music = load_ogg_ex("sounds/music.ogg");
    play_sample(music, 255, 128, 1000, 1);
  }
}

// Update
void init::update() {
  // Goto splash
  set_next_state(STATE_SPLASH);
}
