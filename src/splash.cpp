#include "splash.h"

splash::splash(){
  // Sets Starting Images
  title = load_bitmap( "images/title.png", NULL);
  intro = load_bitmap( "images/intro.png", NULL);

  // Draw
  stretch_sprite( screen, title, 0, 0, SCREEN_W, SCREEN_H);
  //rest( 400);
  stretch_sprite( screen, intro, 0, 0, SCREEN_W, SCREEN_H);
  //rest( 400);
}

splash::~splash(){
  destroy_bitmap( title);
  destroy_bitmap( intro);
}


// Update
void splash::update(){
  // Goto menu
  set_next_state( STATE_MENU);
}
