#include "button.h"

Button::Button(){

}

Button::~Button(){
  delete images;
}

// Load images from file
void Button::SetImages( char image1[], char image2[]){
  images[0] = load_bitmap(image1, NULL);
  images[1] = load_bitmap(image2, NULL);
}

// Check if being hovered over
bool Button::CheckHover(){
  if( mouse_x > x && mouse_x < x + images[0] -> w && mouse_y > y  && mouse_y < y + images[0] -> h)
    return true;
  return false;
}

// Set position on screen
void Button::SetPosition(int newX, int newY){
  x = newX;
  y = newY;
}

// Draw the button
void Button::draw(BITMAP* tempBitmap){
  draw_sprite( tempBitmap, images[CheckHover()], x, y);
}
