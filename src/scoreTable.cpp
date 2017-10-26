#include "scoreTable.h"

// Default CTOR
scoreTable::scoreTable(){
  filePath = "";
}

// Pass file to CTOR
scoreTable::scoreTable( const char *filePath){
  this -> filePath = filePath;
}

// Read High Scores
bool scoreTable::load(){
  // New file
  FILE *fp;
  fp = fopen( filePath, "r");

  // Exists?
  if (fp == NULL)
    return false;

  // Temps
  char name[32];
  char score[32];
  int r, line_num = 0;

  // Parse line by line
  r = fscanf( fp, "%s %s\n", name, score);
  while( r != EOF){
    if( line_num >= TABLE_SIZE)
      return true;
    else if( r == 2){
      scores[line_num][0] = name;
      scores[line_num][1] = score;
      std::cout << line_num << ":" << scores[line_num][0] << " " << scores[line_num][1] << "\n";
    }
    else
      return false;

    r = fscanf( fp, "%s %s\n", name, score);
    line_num++;
  }
  return true;
}

// Add score
void scoreTable::addScore( std::string name, int score){
  // Update table
  load();

  // Prevent crashing
  if(name == "")
    name = "Player";

  // Update List
  /*for( int i = 0; i < TABLE_SIZE; i++){
    if( score > atoi(scores[i][1].c_str())){
      for( int t = 9; t > i; t--){
        scores[t][1] = scores[t - 1][1];
        scores[t][0] = scores[t - 1][0];
      }
      scores[i][1] = intToString(score);
      scores[i][0] = name;
      break;
    }
  }

  // Save Scores
  std::ofstream saveFile;
  saveFile.open( filePath);
  for( int i = 0; i < TABLE_SIZE; i++)
    for( int t = 0; t < 2; t++)
      saveFile << scores[i][t]<< " ";
  saveFile.close();*/
}

// Get names
const char* scoreTable::scoreAt( int index){
  if( index >= 0 && index < TABLE_SIZE)
    return scores[index][0];
  return "";
}

// Get scores
const char* scoreTable::nameAt( int index){
  if( index >= 0 && index < TABLE_SIZE)
    return scores[index][1];
  return "";
}
