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
  void draw( BITMAP* tempBitmap);

  // Set position on screen
  void SetPosition( int x, int y);

  // Check if being hovered over
  bool Hover();

  // Select
  void Select();

  // Get the x/y position of button
  int GetX(){ return x; }
  int GetY(){ return y; }

  // Set samples
  void SetSamples( SAMPLE *hover, SAMPLE *select);

private:
  // Positioning
  int x;
  int y;

  // Samples
  SAMPLE *sample_hover;
  SAMPLE *sample_select;

  // Was hovering before
  bool hovering;

  // Images
  BITMAP *images[2];
};

#endif

