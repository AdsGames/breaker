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

    virtual void update() override;
    virtual void draw() override {};

  private:
    SAMPLE *music;
};

#endif // INIT_H
