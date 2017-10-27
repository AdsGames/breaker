/**
 * INIT
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef INIT_H
#define INIT_H

#include <allegro.h>
#include <alpng.h>
#include <fstream>
#include <logg.h>

#include "state.h"
#include "convert.h"
#include "globals.h"

class init : public state
{
  public:
    init();
    virtual ~init(){};

    void update();
    void draw(){};

  protected:

  private:
    SAMPLE *music;
};

#endif // INIT_H
