/**
 * SPLASH
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef SPLASH_H
#define SPLASH_H

#include <allegro.h>
#include <time.h>

#include "state.h"

class splash : public state {
  public:
    splash();
    virtual ~splash();

    void update();
    void draw();

  protected:

  private:
    // Title images
    BITMAP *intro;
    BITMAP *title;

    // Time
    clock_t intro_time;
};

#endif // SPLASH_H
