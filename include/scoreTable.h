#ifndef SCORETABLE_H
#define SCORETABLE_H

#define TABLE_SIZE 10

#include <fstream>
#include <iostream>

class scoreTable{
  public:
    scoreTable();
    scoreTable(const char *filePath);
    virtual ~scoreTable() {};

    bool load();
    void addScore(std::string name, int score);

    const char* scoreAt( int index);
    const char* nameAt( int index);

  private:
    const char *scores[TABLE_SIZE][2];
    const char *filePath;
};

#endif // SCORETABLE_H
