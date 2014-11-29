#include "button.h"

using namespace std;

Button::Button(){
  button_height = 120;
  button_width = 520;
}

Button::~Button(){
  delete [] images[0][0], images[0][1];
}

void Button::SetImages( char image1[], char image2[]){  
  images[0][0]= load_bitmap(image1, NULL);
  images[0][1]= load_bitmap(image2, NULL); 
}

void Button::SetHover(bool newHover){
  hover = newHover;
}

void Button::setResDiv(int newResDiv){
  resDiv = newResDiv;
}

bool Button::GetHover(){
  return hover;
}

bool Button::CheckHover(){
  if(mouse_x*resDiv>GetX() && mouse_x*resDiv<GetX()+button_width && mouse_y*resDiv>GetY() && mouse_y*resDiv<GetY()+button_height){
    return true;
  }
  else{
    return false;
  }
}

void Button::SetPosition(int newX, int newY){
  x = newX;
  y = newY;
}

int Button::GetX(){
  return x;
}
       
int Button::GetY(){
  return y;
}

void Button::draw(BITMAP* tempBitmap){  
  if(CheckHover()){
    DrawNewSprite( tempBitmap, images[0][1]);
  }
  else{
    DrawNewSprite( tempBitmap, images[0][0]);
  }                              
}

void Button::DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw){
  draw_sprite(tempBitmap, spriteToDraw, GetX(), GetY());  
}
