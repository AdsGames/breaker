#include "menu.h"

// INIT
menu::menu(){
  // Sets button images
  start_game.SetImages( "images/buttons/start.png", "images/buttons/start_hover.png");
  start_easy.SetImages( "images/buttons/start_easy.png", "images/buttons/start_easy_hover.png");
  start_medium.SetImages( "images/buttons/start_medium.png", "images/buttons/start_medium_hover.png");
  start_hard.SetImages( "images/buttons/start_hard.png", "images/buttons/start_hard_hover.png");
  back.SetImages( "images/buttons/back.png", "images/buttons/back_hover.png");
  help.SetImages( "images/buttons/help.png", "images/buttons/help_hover.png");
  quit.SetImages( "images/buttons/quit.png", "images/buttons/quit_hover.png");
  viewHighScores.SetImages( "images/buttons/highscores.png", "images/buttons/highscores_hover.png");

  // Sets button positions
  start_game.SetPosition( 380, 240);
  viewHighScores.SetPosition( 380, 380);
  help.SetPosition( 380, 520);
  quit.SetPosition( 380, 660);

  start_easy.SetPosition( 380, 240);
  start_medium.SetPosition( 380, 380);
  start_hard.SetPosition( 380, 520);
  back.SetPosition( 380, 800);

  // Sets Cursors
  cursor[0] = load_bitmap( "images/cursor1.png", NULL);
  cursor[1] = load_bitmap( "images/cursor2.png", NULL);

  // Creates a buffer
  buffer = create_bitmap( 1280, 960);

  // Sets help
  menuHelp = load_bitmap( "images/help.png", NULL);

  // Sets main menu
  mainmenu = load_bitmap( "images/mainmenu.png", NULL);

  // Sets the high score table image
  highScoresTable = load_bitmap( "images/highScoresTable.png", NULL);

  // Give score files
  highscores = scoreTable( "data/scores.dat");
  highscores.load();

  viewHelp = false;
  viewScores = false;
  viewDifficulty = false;
}

// DESTORY
menu::~menu(){
  destroy_bitmap( buffer);
  destroy_bitmap( mainmenu);
  destroy_bitmap( menuHelp);
  destroy_bitmap( highScoresTable);
  destroy_bitmap( cursor[0]);
  destroy_bitmap( cursor[1]);
}

// Update
void menu::update(){
  // Menu
  // Checks for mouse press
  if( mouseListener::mouse_pressed & 1){
    // Main menu
    if( !viewDifficulty){
      // Help if necessary
      if( viewHelp){
        viewHelp = false;
      }
      // Scores if necessary
      else if(viewScores){
        highscores.load();
        viewScores = false;
      }
      // Buttons
      else if( start_game.Hover()){
        viewDifficulty = true;
      }
      else if( viewHighScores.Hover()){
        viewScores = true;
      }
      else if( help.Hover()){
        viewHelp = true;
      }
      else if( quit.Hover()){
        set_next_state( STATE_EXIT);
      }
    }

    // Difficulty Select
    else{
      if( start_easy.Hover()){
        difficulty = 3;
        set_next_state( STATE_GAME);
      }
      else if( start_medium.Hover()){
        difficulty = 4;
        set_next_state( STATE_GAME);
      }
      else if( start_hard.Hover()){
        difficulty = 5;
        set_next_state( STATE_GAME);
      }
      else if( back.Hover()){
        viewDifficulty = false;
      }
    }
  }
}

// Draw
void menu::draw(){
  // Draws menu
  draw_sprite( buffer, mainmenu, 0, 0);

  // Main menu
  if( !viewDifficulty){
    // Draws Buttons
    start_game.draw( buffer);
    help.draw( buffer);
    quit.draw( buffer);
    viewHighScores.draw( buffer);

    // Draw help if nessisary
    if( viewHelp){
      set_alpha_blender();
      draw_trans_sprite( buffer, menuHelp, 0, 0);
    }

    // Draw scores if nessisary
    if( viewScores){
      set_alpha_blender();
      draw_trans_sprite( buffer, highScoresTable, 0, 0);
      for( int i = 0; i < 10; i++){
        textout_ex( buffer, font, highscores.nameAt(i).c_str(), 400, (i * 50) + 260, makecol(0,0,0), -1);
        textout_right_ex( buffer, font, highscores.scoreAt(i).c_str(), 860, (i * 50) + 260, makecol(0,0,0), -1);
      }
    }
  }
  else{
    // Draws Buttons
    start_easy.draw(buffer);
    start_medium.draw(buffer);
    start_hard.draw(buffer);
    back.draw(buffer);
  }

  // Draws Cursor
  draw_sprite( buffer, cursor[(mouse_b & 1)], mouse_x, mouse_y);

  // Draw buffer
  draw_sprite( screen, buffer, 0, 0);
}
