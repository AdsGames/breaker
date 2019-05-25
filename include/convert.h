/**
 * Convert
 * Allan Legemaate
 * 26/10/2017
 * Converts stuff, strings mainly
**/
#ifndef CONVERT_H
#define CONVERT_H

#include <string>
#include <sstream>

class convert {
  public:
    // Convert int to string
    static std::string intToString (int number);

    // Convert string to int
    static int stringToInt (std::string newString);
  protected:

  private:
    convert() {};
    ~convert() {};
};

#endif // CONVERT_H
