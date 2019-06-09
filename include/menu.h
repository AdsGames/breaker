/**
 * MENU
 * Allan Legemaate
 * 26/10/2017
**/
#ifndef MENU_H
#define MENU_H

#include <allegro.h>

#include "state.h"
#include "ui/Button.h"
#include "ScoreManager.h"
#include "utility/MouseListener.h"

class menu : public state {
  public:
    menu();
    virtual ~menu();

    virtual void update() override;
    virtual void draw() override;

  private:
    // Score table
    ScoreManager highscores;

    // Manu state
    char menu_state;

    // States
    enum menu_states {
      MENU_MAIN,
      MENU_DIFFICULTY,
      MENU_SCORES,
      MENU_HELP
    };

    // Init buttons
    Button btn_start, btn_easy, btn_medium, btn_hard, btn_back, btn_help, btn_quit, btn_scores;

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
};

#endif // INIT_H
