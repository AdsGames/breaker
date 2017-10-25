/*
 * Button
 * A clickable button
 * Allan Legemaate
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <allegro.h>

class Button{

public:
  Button();
  ~Button();

  // Load images from file
  void SetImages( const char *image1, const char *image2);

  // Draw the button
  void draw(BITMAP* tempBitmap);

  // Set position on screen
  void SetPosition( int newX, int newY);

  // Check if being hovered over
  bool CheckHover();

  // Get the x/y position of button
  int GetX(){ return x; }
  int GetY(){ return y; }

private:
  // Positioning
  int x;
  int y;

  // Images
  BITMAP *images[2];
};

#endif

