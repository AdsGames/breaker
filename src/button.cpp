#include "button.h"

Button::Button(){
  this -> x = 0;
	this -> y = 0;

	this -> images[0] = NULL;
	this -> images[1] = NULL;

	this -> sample_hover = NULL;
	this -> sample_select = NULL;

	hovering = false;
}

Button::~Button(){
  destroy_bitmap( images[0]);
  destroy_bitmap( images[1]);
}

// Load images from file
void Button::SetImages( const char *image1, const char *image2){
  images[0] = load_bitmap( image1, NULL);
  images[1] = load_bitmap( image2, NULL);
}

// Check if being hovered over
bool Button::Hover(){
  bool newHover = false;
  if( mouse_x >= x && mouse_x < (x + images[0] -> w) && mouse_y >= y  && mouse_y < (y + images[0] -> h))
    newHover = true;

  if( sample_hover != NULL && newHover == true && hovering == false)
    play_sample( sample_hover, 64, 128, 1000, 0);

  hovering = newHover;

  return hovering;
}

// Select button
void Button::Select(){
  if( sample_select != NULL)
    play_sample( sample_select, 128, 128, 1000, 0);
}

// Set position on screen
void Button::SetPosition( int x, int y){
  this -> x = x;
  this -> y = y;
}

// Set samples
void Button::SetSamples( SAMPLE *hover, SAMPLE *select){
  this -> sample_hover = hover;
  this -> sample_select = select;
}

// Draw the button
void Button::draw( BITMAP* tempBitmap){
  draw_sprite( tempBitmap, images[Hover()], x, y);
}
