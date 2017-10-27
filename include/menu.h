/**
 * MENU
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef MENU_H
#define MENU_H

#include <allegro.h>
#include <alpng.h>

#include "state.h"
#include "button.h"
#include "scoreTable.h"
#include "mouseListener.h"
#include "globals.h"

class menu : public state
{
  public:
    menu();
    virtual ~menu();

    void update();
    void draw();

  protected:

  private:
    // Score table
    scoreTable highscores;

    // Init buttons
    Button start_game;
    Button start_easy;
    Button start_medium;
    Button start_hard;
    Button back;
    Button help;
    Button quit;
    Button viewHighScores;

    // Images
    BITMAP *buffer;
    BITMAP *mainmenu;
    BITMAP *menuHelp;
    BITMAP *highScoresTable;
    BITMAP *cursor[2];
    BITMAP *trans_overlay;

    // Button sounds
    SAMPLE *button_hover;
    SAMPLE *button_select;

    // Variables
    bool viewHelp;
    bool viewScores;
    bool viewDifficulty;
};

#endif // INIT_H
