/**
 * Mouse Listener
 * Allan Legemaate
 * Listens for mouse buttons JUST pressed or JUST released
 *   since the last tick
 * 02/01/2017
**/


#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#define MAX_MOUSE_BUTTONS 3

#include <allegro.h>
//#include <iostream>

class mouseListener{
  public:
    mouseListener();
    virtual ~mouseListener();

    void update();

    static bool buttonPressed[MAX_MOUSE_BUTTONS];
    static bool buttonReleased[MAX_MOUSE_BUTTONS];
    static bool lastTicksButton[MAX_MOUSE_BUTTONS];

  protected:

  private:
};

#endif // MOUSELISTENER_H
