#include "ScoreManager.h"

#include <fstream>

// Default CTOR
ScoreManager::ScoreManager() {
  default_table = "Allan\n400\nDanward\n350\nL.K.\n300\nDudefaceIII\n250\nJohnny\n200\nSlimeKnight\n175\nBilly\n150\nJimothy\n125\nCarter\n100\nAndrew\n10";
  path = "highscores.dat";
}

// Pass file to CTOR
ScoreManager::ScoreManager(const char *path) :
  ScoreManager() {
  this -> path = path;

  if (!read()) {
    create();
    read();
  }
}

// Create table
bool ScoreManager::create() {
  std::ofstream w (path);

  if (!w) {
    w.close();
    return false;
  }

  w << default_table;
  w.close();

  return true;
}

// Read High Scores
bool ScoreManager::read() {
  std::ifstream r (path);

  if (!r) {
    r.close();
    return false;
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    r >> names[i];
    r >> scores[i];
  }

  r.close();

  return true;
}

// Save Scores
bool ScoreManager::write() {
  std::ofstream w(path);

  if (!w) {
    w.close();
    return false;
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    w << names[i] << "\n" << scores[i] << "\n";
  }

  w.close();

  return true;
}

// Add score
void ScoreManager::add(std::string name, int score) {
  // Prevent crashing
  if (name == "") {
    name = "Player";
  }

  // Update List
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (score > scores[i]) {
      for (int t = 9; t > i; t--) {
        scores[t] = scores[t - 1];
        names[t] = names[t - 1];
      }

      scores[i] = score;
      names[i] = name;
      break;
    }
  }

  // Save
  write();
}

// Get names
int ScoreManager::getScore(int index) {
  return (index >= 0 && index < TABLE_SIZE) ? scores[index] : 0;
}

// Get scores
std::string ScoreManager::getName(int index) {
  return (index >= 0 && index < TABLE_SIZE) ? names[index] : "";
}
