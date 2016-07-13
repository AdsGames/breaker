#include <allegro.h>
#include <alpng.h>
#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#include <sstream>
#include <fstream>
#include <time.h>

#include "globals.h"
#include "block.h"
#include "button.h"
#include "particle.h"

// Close button handling
volatile int close_button_pressed = FALSE;

// Init the blocks on screen
Block MyBlocks[14][9];

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
BITMAP* buffer;
BITMAP* intro;
BITMAP* background;
BITMAP* cursor[2];
BITMAP* bimages[11];
BITMAP* foreground;
BITMAP* menu;
BITMAP* mainmenu;
BITMAP* menuHelp;
BITMAP* highScoresTable;

// Title images
BITMAP* title;

// Sounds
SAMPLE* block_break;
SAMPLE* click;
FSOUND_STREAM* music;

// Fonts
FONT *f1, *f2, *f3, *f4, *f5;

// Variables
int done;
int blocks_left;
int elaspedTime;
int difficulty;
int gameScreen;
int score;
int startAnimate;

std::string scores[10][2];

bool mousedown;
bool helpOn;
bool viewScores;
bool closeGame;

// Config file
bool showFPS;

// Timers
clock_t startTime;
clock_t currentTime;

// Text input
std::string edittext = "Player";
std::string::iterator iter = edittext.end();

// FPS System
volatile int ticks = 0;
int updates_per_second = 100;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

// Functions
void draw( bool toScreen);

void ticker(){
    ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
    game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Convert int to string
std::string convertInt(int number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

// Convert bool to string
std::string convertBool(bool boolean){
  std::stringstream ss;
  ss << boolean;
  return ss.str();
}

// Convert string to int
int convertString(std::string newString){
  int result;
  std::stringstream(newString) >> result;
  return result;
}

// Read High Scores
void updateScores(){
  std::ifstream read("data/scores.dat");

  for (int i = 0; i < 10; i++){
    for( int t = 0; t < 2; t++){
      read >> scores[i][t];
    }
  }
  read.close();
}

// Add score
void addScore(std::string name){
  // Update table
  updateScores();

  // Prevent crashing
  if(name == ""){
    name = "Player";
  }

  // Update List
  for (int i = 0; i < 10; i++){
    if(score > atoi(scores[i][1].c_str())){
      for (int t = 9; t > i; t--){
        scores[t][1] = scores[t - 1][1];
        scores[t][0] = scores[t - 1][0];
      }
      scores[i][1] = convertInt(score);
      scores[i][0] = name;
      break;
    }
  }

  // Save Scores
  std::ofstream saveFile;
  saveFile.open("data/scores.dat");

  for (int i = 0; i < 10; i++){
    for( int t = 0; t < 2; t++){
      saveFile<<scores[i][t]<<" ";
    }
  }
  saveFile.close();
}

// Sets up game
void setup(bool first){
  // Runs only for full setup
  if(first){
    // Difficulty
    difficulty = 1;

    // Read config file
    bool sound = true;
    int maxFPS = 100;
    int screen_width = 1280;
    int screen_height = 960;
    bool fullscreen = false;

    std::string finalFile = "data/config.txt";
    std::ifstream read(finalFile.c_str());

    std::string config;
    read >> config;
    if( config == "sound:"){
      read >> config;
      if( config == "true"){
        sound = true;
      }
      else{
        sound = false;
      }
    }
    read >> config;
    if( config == "maxFPS:"){
      read >> config;
      maxFPS = convertString(config);
    }
    read >> config;
    if( config == "screen_width:"){
      read >> config;
      screen_width = convertString(config);
    }
    read >> config;
    if( config == "screen_height:"){
      read >> config;
      screen_height = convertString(config);
    }
    read >> config;
    if( config == "fullscreen:"){
      read >> config;
      if( config == "true")
        fullscreen = true;
      else
        fullscreen = false;
    }
    read >> config;
    if( config == "showFPS:"){
      read >> config;
      if( config == "true")
        showFPS = true;
      else
        showFPS = false;
    }
    read.close();

    // Init sound
    if(sound){
      // Fmod
      FSOUND_Init ( 44100, 32, 0);
      // Allegro
      install_sound( DIGI_AUTODETECT,MIDI_AUTODETECT,".");
    }

    // Set max FPS
    updates_per_second = maxFPS;

    // Set screenmode
    if(fullscreen){
      if( set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, screen_width, screen_height, 0, 0) !=0){
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0);
        allegro_message( "Unable to go into fullscreen graphic mode\n%s\n", allegro_error);
        exit(1);
      }
    }
    else{
      if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, screen_width, screen_height, 0, 0) !=0){
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0);
        allegro_message( "Unable to set any windowed graphic mode\n%s\n", allegro_error);
        exit(1);
      }
    }

    // Seeds random generator with time
    srand(time(NULL));

    // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

    // Sets button images
    start_game.SetImages("images/buttons/start.png","images/buttons/start_hover.png");
    start_easy.SetImages("images/buttons/start_easy.png","images/buttons/start_easy_hover.png");
    start_medium.SetImages("images/buttons/start_medium.png","images/buttons/start_medium_hover.png");
    start_hard.SetImages("images/buttons/start_hard.png","images/buttons/start_hard_hover.png");
    back.SetImages("images/buttons/back.png","images/buttons/back_hover.png");
    help.SetImages("images/buttons/help.png","images/buttons/help_hover.png");
    quit.SetImages("images/buttons/quit.png","images/buttons/quit_hover.png");
    viewHighScores.SetImages("images/buttons/highscores.png","images/buttons/highscores_hover.png");

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

    // Sets Starting Images
    title = load_bitmap( "images/title.png", NULL);
    intro = load_bitmap( "images/intro.png", NULL);

    // Sets background
    background = load_bitmap( "images/background.png", NULL);

    // Sets Foreground
    foreground = load_bitmap( "images/foreground.png", NULL);

    // Sets Sounds
    block_break = load_sample( "sounds/break.wav" );
    click = load_sample( "sounds/click.wav" );
    music = FSOUND_Stream_Open( "sounds/music.ogg", 2, 0, 0);

    // Sets menu
    menu = load_bitmap( "images/menu.png", NULL);

    // Sets help
    menuHelp = load_bitmap( "images/help.png", NULL);

    // Sets main menu
    mainmenu = load_bitmap( "images/mainmenu.png", NULL);

    // Sets the high score table image
    highScoresTable = load_bitmap( "images/highScoresTable.png", NULL);

    // Sets Font
    f1 = load_font( "fonts/arial_black.pcx", NULL, NULL);
    f2 = extract_font_range( f1, ' ', 'A'-1);
    f3 = extract_font_range( f1, 'A', 'Z');
    f4 = extract_font_range( f1, 'Z'+1, 'z');

    // Merge fonts
    font = merge_fonts( f4, f5 = merge_fonts(f2, f3));

    // Destroy temporary fonts
    destroy_font(f1);
    destroy_font(f2);
    destroy_font(f3);
    destroy_font(f4);
    destroy_font(f5);

    // Background Music
    FSOUND_Stream_Play( 0, music);

    // Load scores
    updateScores();
  }


  // Sets Variables
  gameScreen = 0;
  mouse_b = false;
  mousedown = false;
  helpOn = false;
  viewScores = false;
  score = 0;
  startAnimate = 1200;

  // Resets Timers
  startTime = clock();
  currentTime = clock();

  // Sets block info
  for(int i = 0; i < 14; i++){
    for(int t = 0; t < 9; t++){
      MyBlocks[i][t].SetType( random(0, difficulty));
      MyBlocks[i][t].Change();
      MyBlocks[i][t].SetSelected(false);
      MyBlocks[i][t].SetX( i * 80 + 80);
      MyBlocks[i][t].SetY( t * 80 + 80);
    }
  }

}

// Does all game loops
void game(){
  // Title
  if( gameScreen == 0){
    // Shows logo
    /*highcolor_fade_in( intro, 16);
    rest(2000);
    highcolor_fade_out(16);

    // Shows title
    highcolor_fade_in( title, 16);
    readkey();
    highcolor_fade_out(16);*/
    gameScreen = 1;
    draw(false);
    // highcolor_fade_in( buffer, 16);
  }

  // Menu
  else if( gameScreen == 1){
    // Checks for mouse press
    if( mouse_b & 1){
      if( start_game.CheckHover()){
        highcolor_fade_out(16);
        gameScreen = 2;
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
      else if( viewHighScores.CheckHover()){
        if( !viewScores && !helpOn){
          highcolor_fade_out(16);
          clear_keybuf();
          viewScores = true;
          draw(false);
          highcolor_fade_in( buffer, 16);
        }
      }
      else if( help.CheckHover()){
        if( !helpOn && !viewScores){
          highcolor_fade_out(16);
          clear_keybuf();
          helpOn = true;
          draw(false);
          highcolor_fade_in( buffer, 16);
        }
      }
      else if(quit.CheckHover()){
        closeGame = true;
      }
    }

    // Help if necessary
    if(helpOn){
      if(mouse_b || keypressed()){
        highcolor_fade_out(16);
        helpOn = false;
        draw(false);
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
    }

    // Scores if necessary
    if(viewScores){
      updateScores();
      if( mouse_b || keypressed()){
        highcolor_fade_out(16);
        viewScores = false;
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
    }
  }

  // Difficulty Select
  else if( gameScreen == 2){
    // Press buttons
    if( mouse_b & 1){
      if( start_easy.CheckHover()){
        highcolor_fade_out(16);
        difficulty = 3;
        setup(false);
        gameScreen = 3;
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
      else if( start_medium.CheckHover()){
        highcolor_fade_out(16);
        difficulty = 4;
        setup(false);
        gameScreen = 3;
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
      else if( start_hard.CheckHover()){
        highcolor_fade_out(16);
        difficulty = 5;
        setup(false);
        gameScreen = 3;
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
      else if( back.CheckHover()){
        highcolor_fade_out(16);
        gameScreen = 1;
        draw(false);
        highcolor_fade_in( buffer, 16);
      }
    }
  }

  // Game
  else if( gameScreen == 3){
    // Animation for start of game
    if( startAnimate > 2)
      startAnimate -= 3;
    else
      startAnimate = 0;

    // Resets block count
    blocks_left = 0;

    // Updates Elasped Time
    currentTime = clock();
    elaspedTime = int( currentTime - startTime) / CLOCKS_PER_SEC;

    // Performs many functions
    for( int i = 0; i < 14; i++){
      for( int t = 0; t < 9; t++){
        // Blocks Remaining
        if( MyBlocks[i][t].GetType() != 6)
          blocks_left ++;

        // Logic
        MyBlocks[i][t].logic();

        // Groups Same Blocks
        if( MyBlocks[i][t].GetSelected()){
          int offset_x, offset_y;
          for( int k = 0; k < 4; k ++){
            //1, 0, -1, 0
            offset_x = sin(k * M_PI_2);
            //0, +1, 0, -1
            offset_y = sin((k * M_PI_2) + M_PI_2);

            if( i + offset_x < 14 && i + offset_x >= 0 && t + offset_y < 9 && t + offset_y >= 0 && MyBlocks[i + offset_x][t + offset_y].GetType() == MyBlocks[i][t].GetType()){
              MyBlocks[i + offset_x][t + offset_y].SetSelected(true);
            }
          }
        }

        // Blocks fall
        if( MyBlocks[i][t].GetType() != 6 && MyBlocks[i][t+1].GetType() == 6 && t < 8){
          MyBlocks[i][t+1].SetType( MyBlocks[i][t].GetType());
          MyBlocks[i][t].SetType(6);
          MyBlocks[i][t].Change();
          MyBlocks[i][t+1].Change();
          MyBlocks[i][t].SetSelected(false);
          MyBlocks[i][t+1].SetSelected(false);
        }
      }
    }

    // Finish Game
    if( mouse_b & 1){
      // Back button
      if( mouse_y < 60 && mouse_y > 10 && mouse_x < 780 && mouse_x > 500){
        gameScreen = 5;

        // Assign score
        score = (((126 - blocks_left + 1) * 10) * (difficulty)) - ((elaspedTime + 2) * 10);

        // No negative scores
        if(score < 0)
          score = 0;
      }

      // Update selections
      for(int i = 0; i < 14; i++){
        for(int t = 0; t < 9; t++){
          if( MyBlocks[i][t].GetX() == (mouse_x/80)*80 && MyBlocks[i][t].GetY() == (mouse_y/80)*80){
            if( !mousedown && !MyBlocks[i][t].GetSelected() && MyBlocks[i][t].GetType() != 6){
              for(int j = 0; j < 14; j++){
                for(int k = 0; k < 9; k++){
                  MyBlocks[j][k].SetSelected(false);
                }
              }
              MyBlocks[i][t].SetSelected(true);
              mousedown = true;
            }
            if( !mousedown && MyBlocks[i][t].GetSelected()){
              if( MyBlocks[i+1][t].GetSelected() || MyBlocks[i-1][t].GetSelected() || MyBlocks[i][t+1].GetSelected() || MyBlocks[i][t-1].GetSelected()){
                for(int i = 0; i < 14; i++){
                  for(int t = 0; t < 9; t++){
                    if(MyBlocks[i][t].GetSelected()){
                      MyBlocks[i][t].SetSelected(false);
                      MyBlocks[i][t].explode();
                      MyBlocks[i][t].SetType(6);
                      MyBlocks[i][t].Change();
                      play_sample(block_break,255,100,random(400, 2000),0);
                    }
                  }
                }
                mousedown = true;
              }
            }
          }
        }
      }
    }

    // Checks for spaces and removes them
    for(int i = 0; i < 13; i++){
      if( MyBlocks[i][8].GetType() == 6){
        int spaces;
        for( int b = 8; b > 0; b--){
          if(MyBlocks[i][b].GetType() == 6 && MyBlocks[i+1][8].GetType() != 6){
            spaces++;
          }
          else{
            spaces = 1;
          }
        }
        if(spaces > 8){
          for(int a = 0; a < 9; a++){
            MyBlocks[i][a].SetType(MyBlocks[i+1][a].GetType());
            MyBlocks[i+1][a].SetType(6);
            MyBlocks[i][a].Change();
            MyBlocks[i+1][a].Change();
            play_sample(click,255,155,random(400, 1000),0);
          }
        }
      }
    }

    // Checks for possible moves
    int matchesLeft = 0;
    for(int i = 0; i < 14; i++){
      for(int t = 0; t < 9; t++){
        if(MyBlocks[i][t].GetType() != 6){
          if((MyBlocks[i][t].GetType() == MyBlocks[i+1][t].GetType() && i < 13) ||
             (MyBlocks[i][t].GetType() == MyBlocks[i-1][t].GetType() && i > 0 ) ||
             (MyBlocks[i][t].GetType() == MyBlocks[i][t+1].GetType() && t < 8 ) ||
             (MyBlocks[i][t].GetType() == MyBlocks[i][t-1].GetType() && t > 0 )){
            matchesLeft++;
          }
        }
      }
    }
    // Win!
    if(matchesLeft == 0){
      // Makes sure that all blocks are fallen
      bool canWin = true;
      for(int i = 0; i < 14; i++){
        for(int t = 0; t < 9; t++){
          if( t < 8 && MyBlocks[i][t].GetType() != 6 && MyBlocks[i][t+1].GetType() == 6){
            canWin = false;
          }
        }
      }
      // Makes sure all rows have been moved over
      for(int i = 0; i < 14; i++){
        if( i < 12 && MyBlocks[i][8].GetType() != 6 && MyBlocks[i+1][8].GetType() == 6 && MyBlocks[i+2][8].GetType() != 6){
          canWin = false;
        }
      }
      if(canWin){
        // Assign score
        score = (((126 - blocks_left + 1) * 10) * (difficulty)) - ((elaspedTime + 2) * 10);
        if(score < 0){
          score = 0;
        }
        if(blocks_left == 0)
          gameScreen = 4;
        else
          gameScreen = 5;
      }
    }

    // Resets Mousdown
    if( !mouse_b && !key[KEY_SPACE]){
      mousedown = false;
    }
  }

  // Win screen
  else if( gameScreen == 4){
    // Checks for button press
    if( mouse_b & 1){
      if( mouse_x < 520 && mouse_x > 340 && mouse_y < 580 && mouse_y > 510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 3;
        draw( false);
        highcolor_fade_in(buffer,16);
      }
      else if(mouse_x < 940 && mouse_x > 760 && mouse_y < 580 && mouse_y > 510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 1;
        draw( false);
        highcolor_fade_in(buffer,16);
      }
    }

    // Name input
    if(keypressed()){
      int  newkey   = readkey();
      char ASCII    = newkey & 0xff;
      char scancode = newkey >> 8;

      // a character key was pressed; add it to the string
      if(ASCII >= 32 && ASCII <= 126 && edittext.length() < 14 && scancode != KEY_SPACE){
        // add the new char
        iter = edittext.insert(iter, ASCII);
        // increment both the caret and the iterator
        iter++;
      }
      // some other, "special" key was pressed; handle it here
      else{
        if(scancode == KEY_DEL){
          if(iter != edittext.end()){
            iter = edittext.erase(iter);
          }
        }
        if(scancode == KEY_BACKSPACE){
          if(iter != edittext.begin()){
             iter--;
             iter = edittext.erase(iter);
          }
        }
        if(scancode == KEY_RIGHT){
          if(iter != edittext.end()){
            iter++;
          }
        }
        if(scancode == KEY_LEFT){
          if(iter != edittext.begin()){
            iter--;
          }
        }
        if(scancode == KEY_ENTER){
          addScore(edittext);
          setup(false);
          gameScreen = 3;
        }
      }
    }
  }

  // Lose Screen
  if( gameScreen == 5){
    // Checks for button press
    if( mouse_b & 1){
      if( mouse_x < 520 && mouse_x > 340 && mouse_y < 580 && mouse_y > 510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 3;
        draw( false);
        highcolor_fade_in(buffer,16);
      }
      else if( mouse_x < 940 && mouse_x > 760 && mouse_y < 580 && mouse_y > 510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 1;
        draw( false);
        highcolor_fade_in(buffer,16);
      }
    }

    // Name input
    if(keypressed()){
      int  newkey   = readkey();
      char ASCII    = newkey & 0xff;
      char scancode = newkey >> 8;

      // a character key was pressed; add it to the string
      if( ASCII >= 32 && ASCII <= 126 && edittext.length() < 14 && scancode != KEY_SPACE){
        // add the new char
        iter = edittext.insert(iter, ASCII);
        // increment both the caret and the iterator
        iter++;
      }
      // some other, "special" key was pressed; handle it here
      else{
        if( scancode == KEY_DEL){
          if( iter != edittext.end()){
            iter = edittext.erase(iter);
          }
        }
        if( scancode == KEY_BACKSPACE){
          if( iter != edittext.begin()){
             iter--;
             iter = edittext.erase(iter);
          }
        }
        if( scancode == KEY_RIGHT){
          if(iter != edittext.end()){
            iter++;
          }
        }
        if( scancode == KEY_LEFT){
          if( iter != edittext.begin()){
            iter--;
          }
        }
        if( scancode == KEY_ENTER){
          addScore(edittext);
          setup(false);
          gameScreen = 3;
        }
      }
    }
  }

  // Exit game
  if( key[KEY_ESC]){
    closeGame = true;
  }

  // Counter for FPS
  frames_done++;
}

void draw( bool toScreen){
  // Menu
  if( gameScreen == 1){
    // Draws menu
    draw_sprite( buffer, mainmenu, 0, 0);

    // Draws Buttons
    start_game.draw(buffer);
    help.draw(buffer);
    quit.draw(buffer);
    viewHighScores.draw(buffer);

    // Draw help if nessisary
    if( helpOn){
      draw_sprite(buffer, menuHelp, 0, 0);
    }

    // Draw scores if nessisary
    if( viewScores){
      draw_sprite(buffer, highScoresTable, 0, 0);
      for(int i = 0; i < 10; i++){
        std::string name = scores[i][0];
        textout_ex(buffer, font, name.c_str(), 400, (i * 50) + 200, makecol(0,0,0), -1);

        name = scores[i][1];
        textout_right_ex(buffer, font, name.c_str(), 860, (i * 50) + 200, makecol(0,0,0), -1);
      }
    }

    // Draws Cursor
    draw_sprite( buffer, cursor[(mouse_b & 1)], mouse_x - 20, mouse_y - 20);
  }

  // Difficulty Select
  else if( gameScreen == 2){
    // Draws menu
    draw_sprite( buffer, mainmenu, 0, 0);

    // Draws Buttons
    start_easy.draw(buffer);
    start_medium.draw(buffer);
    start_hard.draw(buffer);
    back.draw(buffer);

    // Draws Cursor
    draw_sprite( buffer, cursor[(mouse_b & 1)], mouse_x - 20, mouse_y - 20);
  }

  // Game
  else if( gameScreen == 3){
    // Draws background
    draw_sprite( buffer, background, 0, 0);

    // Draws Blocks
    for(int i = 0; i < 14; i++){
      for(int t = 0; t < 9; t++){
        MyBlocks[i][t].draw( buffer, startAnimate);
      }
    }

    // Draws post drawing effects for blocks
    for(int i = 0; i < 14; i++){
      for(int t = 0; t < 9; t++){
        MyBlocks[i][t].postDraw( buffer, startAnimate);
      }
    }

    // Draws foreground
    draw_sprite( buffer, foreground, 0, 0);

    // Draws text
    textprintf_right_ex( buffer, font, 1240, 0, makecol(0,0,0), -1, "Blocks Left: %i", blocks_left);
    textprintf_ex( buffer, font, 40, 0, makecol(0,0,0), -1, "Time: %i", elaspedTime);

    // Draws Cursor
    draw_sprite( buffer, cursor[(mouse_b & 1)], mouse_x - 20, mouse_y - 20);
  }

  // Win screen
  else if( gameScreen == 4){
    // Draw background
    draw_sprite( buffer, background, 0, 0);

    // Draw blocks
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        MyBlocks[i][t].draw(buffer, 0);
      }
    }

    // Create gui
    draw_sprite(buffer,menu,300,300);
    draw_sprite( buffer, foreground, 0, 0);

    textprintf_centre_ex(buffer,font,640,310, makecol(0,0,0),-1,"You Win!");
    textprintf_centre_ex(buffer,font,640,360, makecol(0,0,0),-1,"Score: %i", score);

    // Input rectangle
    rectfill(buffer, 488, 408, 892, 452, makecol(0,0,0));
    rectfill(buffer, 490, 410, 890, 450, makecol(255,255,255));

    // output the string to the screen
    textout_ex(buffer, font, edittext.c_str(), 494, 410, makecol(0,0,0), -1);

    // draw the caret
    vline(buffer, text_length(font, edittext.c_str()) + 494, 412, 448, makecol(0,0,0));

    // Draws Cursor
    draw_sprite( buffer, cursor[(mouse_b & 1)], mouse_x - 20, mouse_y - 20);
  }

  // Lose Screen
  if(gameScreen == 5){
    // Draw background
    draw_sprite( buffer, background, 0, 0);

    // Draw blocks
    for(int i = 0; i < 14; i++){
      for(int t = 0; t < 9; t++){
        MyBlocks[i][t].draw( buffer, 0);
      }
    }

    // Create gui
    draw_sprite( buffer, menu, 300, 300);
    draw_sprite( buffer, foreground, 0, 0);

    textprintf_centre_ex( buffer, font, 640, 310, makecol(0,0,0),-1, "No more moves");
    textprintf_centre_ex( buffer, font, 640, 360, makecol(0,0,0),-1, "Score: %i", score);

    // Input rectangle
    rectfill( buffer, 488, 408, 892, 452, makecol(0,0,0));
    rectfill( buffer, 490, 410, 890, 450, makecol(255,255,255));

    // output the string to the screen
    textout_ex( buffer, font, edittext.c_str(), 494, 410, makecol(0,0,0), -1);

    // draw the caret
    vline( buffer, text_length( font, edittext.c_str()) + 494, 412, 448, makecol(0,0,0));

    // Draws Cursor
    draw_sprite( buffer, cursor[(mouse_b & 1)], mouse_x - 20, mouse_y - 20);
  }
  // FPS counter
  if(showFPS){
    textprintf_ex( buffer, font, 0, 80, makecol(0,0,0), -1, "FPS-%i", fps);
  }

  if( toScreen){
    // Draws buffer
    stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
  }
}

// main function of program
int main(){
  // Initializing
  allegro_init();
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();

  set_color_depth(32);

  set_window_title("Breaker");

  // Setup game
  setup(true);

  // Starts Game
  while(!closeGame && !close_button_pressed){
    // Runs FPS system
    while(ticks == 0){
      rest(1);
    }
    while(ticks > 0){
      int old_ticks = ticks;
      // Update always
      game();
      ticks--;
      if( old_ticks <= ticks){
        break;
      }
    }
    if( game_time - old_time >= 10){
      fps = frames_done;
      frames_done = 0;
      old_time = game_time;
    }
    // Update every set amount of frames
    draw( true);
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN()
