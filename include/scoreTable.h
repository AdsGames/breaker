/**
 * Score Table
 * Allan Legemaate
 * 26/10/2017
 * Easy highscore management
**/

#ifndef SCORETABLE_H
#define SCORETABLE_H

#define TABLE_SIZE 10

#include <fstream>
#include <string>
#include <stdlib.h>

#include "convert.h"

class scoreTable{
  public:
    scoreTable();
    scoreTable( const char *filePath);
    virtual ~scoreTable() {};

    bool load();
    void addScore( std::string name, int score);

    std::string scoreAt( int index);
    std::string nameAt( int index);

  private:
    std::string scores[TABLE_SIZE][2];
    const char *filePath;
};

#endif // SCORETABLE_H
