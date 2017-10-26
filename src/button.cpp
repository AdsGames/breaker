#include "button.h"

Button::Button(){
  this -> x = 0;
	this -> y = 0;

	this -> images[0] = NULL;
	this -> images[1] = NULL;
}

Button::~Button(){
  /*if( images[0])
    destroy_bitmap( images[0]);
  if( images[1])
    destroy_bitmap( images[1]);*/
}

// Load images from file
void Button::SetImages( const char *image1, const char *image2){
  images[0] = load_bitmap(image1, NULL);
  images[1] = load_bitmap(image2, NULL);
}

// Check if being hovered over
bool Button::Hover(){
  if( mouse_x > x && mouse_x < x + images[0] -> w && mouse_y > y  && mouse_y < y + images[0] -> h)
    return true;
  return false;
}

// Set position on screen
void Button::SetPosition( int x, int y){
  this -> x = x;
  this -> y = y;
}

// Draw the button
void Button::draw( BITMAP* tempBitmap){
  draw_sprite( tempBitmap, images[Hover()], x, y);
}
