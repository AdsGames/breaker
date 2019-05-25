/**
 * Mouse Listener
 * Allan Legemaate
 * Listens for mouse buttons JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
**/

#include <allegro.h>

#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#define MAX_MOUSE_BUTTONS 3

class mouseListener {
  public:
    mouseListener() {};
    virtual ~mouseListener() {};

    void update();

    static unsigned char mouse_button;
    static unsigned char mouse_pressed;
    static unsigned char mouse_released;

    static int res_mouse_x;
    static int res_mouse_y;

    static float res_x_multiplier;
    static float res_y_multiplier;
  protected:

  private:
    static unsigned char mouse_old;
};

#endif // MOUSELISTENER_H
