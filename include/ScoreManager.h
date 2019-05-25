/**
 * Score Table
 * Allan Legemaate
 * 26/10/2017
 * Easy highscore management
**/

#ifndef SCORETABLE_H
#define SCORETABLE_H

#include <string>

class ScoreManager {
  public:
    ScoreManager();
    ScoreManager(const char *path);

    virtual ~ScoreManager() {};

    void add(std::string name, int score);

    int getScore(int index);
    std::string getName(int index);

  private:
    static const char TABLE_SIZE = 10;

    const char *path;
    std::string default_table;

    bool create();
    bool read();
    bool write();

    std::string names[TABLE_SIZE];
    int scores[TABLE_SIZE];
};

#endif // SCORETABLE_H
