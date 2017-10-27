#include "init.h"

// Init state (and game)
init::init(){
  // Initializing
  allegro_init();
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();

  set_color_depth(32);

  set_window_title("Breaker");

  // Read config file
  std::ifstream read( "data/config.txt");

  std::string config;
  read >> config;
  if( config == "sound:"){
    read >> config;
    config_sound = false;
    if( config == "true")
      config_sound = true;
  }
  read >> config;
  if( config == "maxFPS:"){
    read >> config;
    config_max_FPS = convert::stringToInt(config);
  }
  read >> config;
  if( config == "screen_width:"){
    read >> config;
    config_screen_width = convert::stringToInt(config);
  }
  read >> config;
  if( config == "screen_height:"){
    read >> config;
    config_screen_height = convert::stringToInt(config);
  }
  read >> config;
  if( config == "fullscreen:"){
    read >> config;
    if( config == "true")
      config_fullscreen = true;
    else
      config_fullscreen = false;
  }
  read >> config;
  if( config == "showFPS:"){
    read >> config;
    if( config == "true")
      config_show_fps = true;
    else
      config_show_fps = false;
  }
  read.close();

  // Init sound
  if( config_sound)
    install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, ".");

  // Set screenmode
  if( config_fullscreen){
    if( set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, config_screen_width, config_screen_height, 0, 0) !=0){
      set_gfx_mode( GFX_TEXT, 0, 0, 0, 0);
      allegro_message( "Unable to go into fullscreen graphic mode\n%s\n", allegro_error);
      exit(1);
    }
  }
  else{
    if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, config_screen_width, config_screen_height, 0, 0) !=0){
      set_gfx_mode( GFX_TEXT, 0, 0, 0, 0);
      allegro_message( "Unable to set any windowed graphic mode\n%s\n", allegro_error);
      exit(1);
    }
  }

  // Seeds random generator with time
  srand(time(NULL));

  // Fonts
  FONT *f1, *f2, *f3, *f4, *f5;

  // Sets Font
  f1 = load_font( "fonts/arial_black.pcx", NULL, NULL);
  f2 = extract_font_range( f1, ' ', 'A'-1);
  f3 = extract_font_range( f1, 'A', 'Z');
  f4 = extract_font_range( f1, 'Z'+1, 'z');

  // Merge fonts
  font = merge_fonts( f4, f5 = merge_fonts(f2, f3));

  // Destroy temporary fonts
  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);

  // Background Music
  if( config_sound){
    music = logg_load( "sounds/music.ogg");
    play_sample( music, 255, 128, 1000, 1);
  }
}

// Update
void init::update(){
  // Goto splash
  set_next_state( STATE_SPLASH);
}
