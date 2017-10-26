#include "convert.h"

// Convert int to string
std::string convert::intToString( int number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

// Convert string to int
int convert::stringToInt( std::string newString){
  int result;
  std::stringstream(newString) >> result;
  return result;
}
