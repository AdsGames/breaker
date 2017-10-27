#include "vec2.h"

// Standard constructor
vec2::vec2(){
  this -> x = 0;
  this -> y = 0;
}

// With parameters
vec2::vec2( float new_x, float new_y){
  this -> x = new_x;
  this -> y = new_y;
}

// Same in both!
vec2::vec2( float x_y){
  this -> x = x_y;
  this -> y = x_y;
}
