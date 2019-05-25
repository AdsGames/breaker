#include "splash.h"

#include <loadpng.h>

splash::splash() {
  // Sets Starting Images
  title = load_png ("images/title.png", NULL);
  intro = load_png ("images/intro.png", NULL);

  // Start timer
  intro_time = clock();
}

splash::~splash() {
  destroy_bitmap (title);
  destroy_bitmap (intro);
}


// Update
void splash::update() {
  if (clock() - intro_time >= 3400 || keypressed()) {
    set_next_state (STATE_MENU);
  }
}

// Draw
void splash::draw() {
  if (clock() - intro_time < 1700) {
    stretch_sprite (screen, title, 0, 0, SCREEN_W, SCREEN_H);
  }
  else {
    stretch_sprite (screen, intro, 0, 0, SCREEN_W, SCREEN_H);
  }
}
