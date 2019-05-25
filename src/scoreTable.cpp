#include "scoreTable.h"

// Default CTOR
scoreTable::scoreTable() {
  filePath = "";
}

// Pass file to CTOR
scoreTable::scoreTable (const char *filePath) {
  this -> filePath = filePath;
}

// Read High Scores
bool scoreTable::load() {
  std::ifstream read (filePath);

  if (!read) {
    read.close();

    std::ofstream write (filePath);
    write << "Allan 400\nDanward 350\nL.K. 300\nDudefaceIII 250\nJohnny 200\nSlimeKnight 175\nBilly 150\nJimothy 125\nCarter 100\nAndrew 10";
    write.close();

    read.open (filePath);
  }

  for (int i = 0; i < 10; i++) {
    for (int t = 0; t < 2; t++) {
      read >> scores[i][t];
    }
  }

  read.close();

  return true;
}

// Add score
void scoreTable::addScore (std::string name, int score) {
  // Update table
  load();

  // Prevent crashing
  if (name == "") {
    name = "Player";
  }

  // Update List
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (score > atoi (scores[i][1].c_str())) {
      for (int t = 9; t > i; t--) {
        scores[t][1] = scores[t - 1][1];
        scores[t][0] = scores[t - 1][0];
      }

      scores[i][1] = convert::intToString (score).c_str();
      scores[i][0] = name;
      break;
    }
  }

  // Save Scores
  std::ofstream saveFile;
  saveFile.open (filePath);

  for (int i = 0; i < TABLE_SIZE; i++) {
    for (int t = 0; t < 2; t++) {
      if (t == 0) {
        saveFile << scores[i][t] << " ";
      }
      else {
        saveFile << scores[i][t] << "\n";
      }
    }
  }

  saveFile.close();
}

// Get names
std::string scoreTable::scoreAt (int index) {
  if (index >= 0 && index < TABLE_SIZE) {
    return scores[index][0];
  }

  return "";
}

// Get scores
std::string scoreTable::nameAt (int index) {
  if (index >= 0 && index < TABLE_SIZE) {
    return scores[index][1];
  }

  return "";
}
