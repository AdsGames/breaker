/**
 * INIT
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef INIT_H
#define INIT_H

#include <allegro.h>

#include "state.h"

class init : public state {
  public:
    init();
    virtual ~init() {};

    void update();
    void draw() {};

  protected:

  private:
    SAMPLE *music;
};

#endif // INIT_H
