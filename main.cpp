#include "globals.h"

#include"block.h"
#include"button.h"
#include"particle.h"

using namespace std;

// Constants
const int numberParticles = 1000;

// Close button
volatile int close_button_pressed = FALSE;

//Creates the blocks on screen
Block MyBlocks[14][9];

//Creates Butons
Button start_game;
Button start_easy;
Button start_medium;
Button start_hard;
Button back;
Button help;
Button quit;
Button viewHighScores;

//Images
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

//Title images
BITMAP* title;

//Sounds
SAMPLE* block_break;
SAMPLE* click;
FSOUND_STREAM* music;

//Fonts
FONT *f1, *f2, *f3, *f4, *f5;

//Variables
int done;
int blocks_left;
int elaspedTime;
int difficulty;
int gameScreen;
int score;
int startAnimate;

int red1;
int green1;
int blue1;

string scores[10][2];

bool mousedown;
bool helpOn;
bool viewScores;
bool closeGame;

//Config file
bool sound;
int maxFPS;
int mouseSpeedX;
int mouseSpeedY;
bool fullscreen;
bool showFPS;

//Resolution X
int resDiv;

//Timers
clock_t startTime;
clock_t currentTime;

//Text input
string  edittext = "Player";
string::iterator iter = edittext.end();

//FPS System
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

//Convert int to string
string convertInt(int number){
   stringstream ss;
   ss << number;
   return ss.str();
}

//Convert bool to string
string convertBool(bool boolean){
    stringstream ss;
    ss << boolean;
    return ss.str();
}

//Convert string to int
int convertString(string newString){
  int result;
  stringstream(newString) >> result;
  return result;
}

// Fade in
void highcolor_fade_in(BITMAP* bmp_orig, int speed){
  draw( false);
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP* str_orig = create_bitmap( SCREEN_W, SCREEN_H);
  stretch_sprite( str_orig, bmp_orig, 0, 0, SCREEN_W, SCREEN_H);

  if ( speed<=0)
    speed=16;

  for(int a=0; a<256; a+=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, str_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  stretch_sprite( screen, str_orig, 0, 0,  SCREEN_W, SCREEN_H);
}

// Fade out
void highcolor_fade_out(int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP *bmp_orig = create_bitmap( SCREEN_W, SCREEN_H);
  blit(screen,bmp_orig,0,0,0,0,SCREEN_W,SCREEN_H);

  if ( speed<=0)
    speed=16;

  for( int a=255 - speed; a>0; a-=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, bmp_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  destroy_bitmap( bmp_orig);
  rectfill( screen, 0, 0,  SCREEN_W, SCREEN_H, makecol(0,0,0));
}

//Read High Scores
void updateScores(){
  ifstream read("data/scores.dat");

  for (int i = 0; i < 10; i++){
    for( int t = 0; t < 2; t++){
      read>>scores[i][t];
    }
  }
  read.close();
}

//Add score
void addScore(string name){
  //Update table
  updateScores();

  //Prevent crashing
  if(name == ""){
    name = "player";
  }

  //Update List
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

  //Save Scores
  ofstream saveFile;
  saveFile.open("data/scores.dat");

  for (int i = 0; i < 10; i++){
    for( int t = 0; t < 2; t++){
      saveFile<<scores[i][t]<<" ";
    }
  }
  saveFile.close();
}

//Sets up game
void setup(bool first){
  //Sets Variables
  gameScreen = 0;
  mouse_b = false;
  mousedown = false;
  helpOn = false;
  viewScores = false;
  score = 0;
  startAnimate = 1200;

  //Resets Timers
  startTime = clock();
  currentTime = clock();

  //Resets blocks for new game
  for(int i=0; i<14; i++){
    for(int t=0; t<9; t++){
      MyBlocks[i][t].SetType(0);
    }
  }

  //Sets block info
  for(int i=0; i<14; i++){
    for(int t=0; t<9; t++){
      MyBlocks[i][t].SetType(random(0, difficulty));
      MyBlocks[i][t].Change();
      MyBlocks[i][t].SetSelected(false);
    }
  }

  //Sets block positions
  for(int i=0; i<14; i++){
    for(int t=0; t<9; t++){
      MyBlocks[i][t].SetX(i*80+80);
      MyBlocks[i][t].SetY(t*80+80);
    }
  }

  //Runs only for full setup
  if(first){
    //Read config file
    string finalFile = "data/config.txt";
    ifstream read(finalFile.c_str());

    string config;
    read>>config;
    if(config == "sound:"){
      read>>config;
      if(config == "true"){
        sound = true;
      }
      else{
        sound = false;
      }
    }
    read>>config;
    if(config == "maxFPS:"){
      read>>config;
      maxFPS = convertString(config);
    }
    read>>config;
    if(config == "mouseSpeedX:"){
      read>>config;
      mouseSpeedX = convertString(config);
    }
    read>>config;
    if(config == "mouseSpeedY:"){
      read>>config;
      mouseSpeedY = convertString(config);
    }
    read>>config;
    if(config == "fullscreen:"){
      read>>config;
      if(config == "true"){
        fullscreen = true;
      }
      else{
        fullscreen = false;
      }
    }
    read>>config;
    if(config == "showFPS:"){
      read>>config;
      if(config == "true"){
        showFPS = true;
      }
      else{
        showFPS = false;
      }
    }
    read.close();

    //Init sound
    if(sound){
      //Fmod
      FSOUND_Init (44100, 32, 0);
      //Allegro
      install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");
    }

    //Set max FPS
    updates_per_second = maxFPS;

    //Mouse acceleration
    set_mouse_speed(mouseSpeedX, mouseSpeedY);

    //Set screenmode
    if(fullscreen){
      resDiv = 1;
      if(set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	allegro_message("Unable to go into fullscreen graphic mode\n%s\n", allegro_error);
            exit(1);
          }
        }
      }
    }
    else{
      resDiv = 1;
      if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	allegro_message("Unable to set any windowed graphic mode\n%s\n", allegro_error);
            exit(1);
          }
        }
      }
    }



    //Creates a random number generator (based on time)
    srand (time(NULL));

    //Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

    //Sets button images
    start_game.SetImages("images/buttons/start.png","images/buttons/start_hover.png");
    start_easy.SetImages("images/buttons/start_easy.png","images/buttons/start_easy_hover.png");
    start_medium.SetImages("images/buttons/start_medium.png","images/buttons/start_medium_hover.png");
    start_hard.SetImages("images/buttons/start_hard.png","images/buttons/start_hard_hover.png");
    back.SetImages("images/buttons/back.png","images/buttons/back_hover.png");
    help.SetImages("images/buttons/help.png","images/buttons/help_hover.png");
    quit.SetImages("images/buttons/quit.png","images/buttons/quit_hover.png");
    viewHighScores.SetImages("images/buttons/highscores.png","images/buttons/highscores_hover.png");

    //Sets button positions
    start_game.SetPosition(380, 240);
    viewHighScores.SetPosition(380, 380);
    help.SetPosition(380, 520);
    quit.SetPosition(380, 660);

    start_easy.SetPosition(380, 240);
    start_medium.SetPosition(380, 380);
    start_hard.SetPosition(380, 520);
    back.SetPosition(380, 800);

    start_game.setResDiv(resDiv);
    viewHighScores.setResDiv(resDiv);
    help.setResDiv(resDiv);
    quit.setResDiv(resDiv);
    start_easy.setResDiv(resDiv);
    start_medium.setResDiv(resDiv);
    start_hard.setResDiv(resDiv);
    back.setResDiv(resDiv);

    //Sets Cursors
    cursor[0]= load_bitmap( "images/cursor1.png", NULL);
    cursor[1]= load_bitmap( "images/cursor2.png", NULL);

    //Creates a buffer
    buffer = create_bitmap( 1280, 960);

    //Sets Starting Images
    title = load_bitmap( "images/title.png", NULL);
    intro = load_bitmap( "images/intro.png", NULL);

    //Sets background
    background = load_bitmap( "images/background.png", NULL);

    //Sets Foreground
    foreground = load_bitmap( "images/foreground.png", NULL);

    //Sets Sounds
    block_break = load_sample( "sounds/break.wav" );
    click = load_sample( "sounds/click.wav" );
    music = FSOUND_Stream_Open("sounds/music.ogg",2, 0, 0);

    //Sets menu
    menu = load_bitmap( "images/menu.png", NULL);

    //Sets help
    menuHelp = load_bitmap( "images/help.png", NULL);

    //Sets main menu
    mainmenu = load_bitmap( "images/mainmenu.png", NULL);

    //Sets the high score table image
    highScoresTable = load_bitmap( "images/highScoresTable.png", NULL);

    //Sets Font
    f1 = load_font("fonts/arial_black.pcx", NULL, NULL);
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');

    //Merge fonts
    font = merge_fonts(f4, f5 = merge_fonts(f2, f3));

    //Destroy temporary fonts
    destroy_font(f1);
    destroy_font(f2);
    destroy_font(f3);
    destroy_font(f4);
    destroy_font(f5);

    //Background Music
    FSOUND_Stream_Play(0,music);
  }
}

//Does all game loops
void game(){
  //Title
  if(gameScreen == 0){
    //Shows logo
    highcolor_fade_in(intro,16);
    rest(2000);
    highcolor_fade_out(16);

    //Shows title
    highcolor_fade_in(title,16);
    readkey();
    highcolor_fade_out(16);
    gameScreen = 1;
    highcolor_fade_in(buffer,16);
  }

  //Menu
  else if(gameScreen == 1){
    //Checks for mouse press
    if(mouse_b&1){
      if(start_game.CheckHover()){
        highcolor_fade_out(16);
        gameScreen = 2;
        highcolor_fade_in(buffer,16);
      }
      else if(viewHighScores.CheckHover()){
        if(!viewScores && !helpOn){
          highcolor_fade_out(16);
          clear_keybuf();
          viewScores = true;
          highcolor_fade_in(buffer,16);
        }
      }
      else if(help.CheckHover()){
        if(!helpOn && !viewScores){
          highcolor_fade_out(16);
          clear_keybuf();
          helpOn = true;
          highcolor_fade_in(buffer,16);
        }
      }
      else if(quit.CheckHover()){
        closeGame = true;
      }
    }

    //Help if necessary
    if(helpOn){
      if(mouse_b || keypressed()){
        highcolor_fade_out(16);
        helpOn = false;
        highcolor_fade_in(buffer,16);
      }
    }

    //Scores if necessary
    if(viewScores){
      updateScores();
      if(mouse_b || keypressed()){
        highcolor_fade_out(16);
        viewScores = false;
        highcolor_fade_in(buffer,16);
      }
    }
  }

  //Difficulty Select
  else if(gameScreen == 2){
    //Press buttons
    if(mouse_b&1){
       if(start_easy.CheckHover()==true){
        highcolor_fade_out(16);
        difficulty = 3;
        setup(false);
        gameScreen = 3;
        highcolor_fade_in(buffer,16);
      }
      if(start_medium.CheckHover()==true){
        highcolor_fade_out(16);
        difficulty = 4;
        setup(false);
        gameScreen = 3;
        highcolor_fade_in(buffer,16);
      }
      if(start_hard.CheckHover()==true){
        highcolor_fade_out(16);
        difficulty = 5;
        setup(false);
        gameScreen = 3;
        highcolor_fade_in(buffer,16);
      }
      if(back.CheckHover()==true){
        highcolor_fade_out(16);
        gameScreen = 1;
        highcolor_fade_in(buffer,16);
      }
    }
  }

  //Game
  else if(gameScreen == 3){
    //Animation for start of game
    if(startAnimate > 2 && startAnimate != 0){
      startAnimate -= 3;
    }
    else{
      startAnimate = 0;
    }

    //resets block count
    blocks_left=0;

    //Update Clock
    currentTime = clock();

    //Updates Elasped Time
    elaspedTime = int(currentTime-startTime)/CLOCKS_PER_SEC;

    //Performs many functions
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        //Blocks Remaining
        if(MyBlocks[i][t].GetType()!=6){
          blocks_left+=1;
        }

        //Logic
        MyBlocks[i][t].logic();

        //Groups Same Blocks
        if(MyBlocks[i][t].GetSelected()==true){
          if(i<13 && MyBlocks[i+1][t].GetType()==MyBlocks[i][t].GetType()){
            MyBlocks[i+1][t].SetSelected(true);
          }
          if(t<8 && MyBlocks[i][t+1].GetType()==MyBlocks[i][t].GetType()){
            MyBlocks[i][t+1].SetSelected(true);
          }
          if(i>0 && MyBlocks[i-1][t].GetType()==MyBlocks[i][t].GetType()){
            MyBlocks[i-1][t].SetSelected(true);
          }
          if(t>0 && MyBlocks[i][t-1].GetType()==MyBlocks[i][t].GetType()){
            MyBlocks[i][t-1].SetSelected(true);
          }
        }
      }
    }

    //Finish Game
    if(mouse_b&1){
      if(mouse_y*resDiv<60 && mouse_y*resDiv>10 && mouse_x*resDiv<780 && mouse_x*resDiv>500){
        gameScreen = 5;
        //Assign score
        score = (((126 - blocks_left + 1) * 10) * (difficulty)) - ((elaspedTime + 2) * 10);
        if(score < 0){
          score = 0;
        }
      }

      //Update selections
      for(int i=0; i<14; i++){
        for(int t=0; t<9; t++){
          if((MyBlocks[i][t].GetX()/80)*80==(mouse_x*resDiv/80)*80 && (MyBlocks[i][t].GetY()/80)*80==(mouse_y*resDiv/80)*80){
            if(mousedown==false && MyBlocks[i][t].GetSelected()==false && MyBlocks[i][t].GetType()!=6){
              for(int j=0; j<14; j++){
                for(int k=0; k<9; k++){
                  MyBlocks[j][k].SetSelected(false);
                }
              }
              MyBlocks[i][t].SetSelected(true);
              mousedown=true;
            }
            if(mousedown==false && MyBlocks[i][t].GetSelected()==true){
              if(MyBlocks[i+1][t].GetSelected()==true||MyBlocks[i-1][t].GetSelected()==true||MyBlocks[i][t+1].GetSelected()==true||MyBlocks[i][t-1].GetSelected()==true){
                for(int i=0; i<14; i++){
                  for(int t=0; t<9; t++){
                    if(MyBlocks[i][t].GetSelected()==true){
                      MyBlocks[i][t].SetSelected(false);
                      MyBlocks[i][t].explode();
                      MyBlocks[i][t].SetType(6);
                      MyBlocks[i][t].Change();
                      play_sample(block_break,255,100,random(400, 2000),0);
                    }
                  }
                }
              mousedown=true;
              }
            }
          }
        }
      }
    }

    //Checks for blocks falling
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        if(MyBlocks[i][t].GetType()!=6 && MyBlocks[i][t+1].GetType()==6 && t<8){
          MyBlocks[i][t+1].SetType(MyBlocks[i][t].GetType());
          MyBlocks[i][t].SetType(6);
          MyBlocks[i][t].Change();
          MyBlocks[i][t+1].Change();
          MyBlocks[i][t].SetSelected(false);
          MyBlocks[i][t+1].SetSelected(false);
        }
      }
    }

    //Checks for spaces and removes them
    for(int i=0; i<13; i++){
      if(MyBlocks[i][8].GetType()==6){
        int spaces;
        for(int b=8; b>0; b--){
          if(MyBlocks[i][b].GetType() == 6 && MyBlocks[i+1][8].GetType() != 6){
            spaces++;
          }
          else{
            spaces = 1;
          }
        }
        if(spaces > 8){
          for(int a=0; a<9; a++){
            MyBlocks[i][a].SetType(MyBlocks[i+1][a].GetType());
            MyBlocks[i+1][a].SetType(6);
            MyBlocks[i][a].Change();
            MyBlocks[i+1][a].Change();
            play_sample(click,255,155,random(400, 1000),0);
          }
        }
      }
    }

    //Checks for possible moves
    int matchesLeft = 0;
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        if(MyBlocks[i][t].GetType() != 6){
          if(MyBlocks[i][t].GetType() == MyBlocks[i+1][t].GetType() && i < 13||
            MyBlocks[i][t].GetType() == MyBlocks[i-1][t].GetType() && i > 0||
            MyBlocks[i][t].GetType() == MyBlocks[i][t+1].GetType() && t < 8||
            MyBlocks[i][t].GetType() == MyBlocks[i][t-1].GetType() && t > 0){
            matchesLeft++;
          }
        }
      }
    }
    //Win!
    if(matchesLeft == 0){
      //Makes sure that all blocks are fallen
      bool canWin = true;
      for(int i=0; i<14; i++){
        for(int t=0; t<9; t++){
          if(t<8 && MyBlocks[i][t].GetType()!=6 && MyBlocks[i][t+1].GetType()==6){
            canWin = false;
          }
        }
      }
      //Makes sure all rows have been moved over
      for(int i=0; i<14; i++){
        if(i<12 && MyBlocks[i][8].GetType()!=6 && MyBlocks[i+1][8].GetType()==6 && MyBlocks[i+2][8].GetType()!=6){
          canWin = false;
        }
      }
      if(canWin){
        //Assign score
        score = (((126 - blocks_left + 1) * 10) * (difficulty)) - ((elaspedTime + 2) * 10);
        if(score < 0){
          score = 0;
        }
        if(blocks_left == 0){
          gameScreen = 4;
        }
        else{
          gameScreen = 5;
        }
      }
    }

    //Resets Mousdown
    if(!mouse_b && !key[KEY_SPACE]){
      mousedown=false;
    }
  }

  //Win screen
  else if(gameScreen == 4){
    //Checks for button press
    if(mouse_b&1){
      if(mouse_x*resDiv<520 && mouse_x*resDiv>340 && mouse_y*resDiv<580 && mouse_y*resDiv>510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 3;
        highcolor_fade_in(buffer,16);
      }
      else if(mouse_x*resDiv<940 && mouse_x*resDiv>760 && mouse_y*resDiv<580 && mouse_y*resDiv>510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 1;
        highcolor_fade_in(buffer,16);
      }
    }

    //Name input
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

  //Lose Screen
  if(gameScreen == 5){
    //Checks for button press
    if(mouse_b&1){
      if(mouse_x*resDiv<520 && mouse_x*resDiv>340 && mouse_y*resDiv<580 && mouse_y*resDiv>510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 3;
        highcolor_fade_in(buffer,16);
      }
      else if(mouse_x*resDiv<940 && mouse_x*resDiv>760 && mouse_y*resDiv<580 && mouse_y*resDiv>510){
        highcolor_fade_out(16);
        addScore(edittext);
        setup(false);
        gameScreen = 1;
        highcolor_fade_in(buffer,16);
      }
    }

    //Name input
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

  //Exit game
  if(key[KEY_ESC]){
    closeGame = true;
  }

  //Counter for FPS
  frames_done++;
}

void draw( bool toScreen){
  //Menu
  if(gameScreen == 1){
    //Draws menu
    draw_sprite( buffer, mainmenu, 0, 0);

    //Draws Buttons
    start_game.draw(buffer);
    help.draw(buffer);
    quit.draw(buffer);
    viewHighScores.draw(buffer);

    //Draw help if neccisary
    if(helpOn){
      draw_sprite(buffer, menuHelp, 0, 0);
    }

    //Draw scores if neccisary
    if(viewScores){
      draw_sprite(buffer, highScoresTable, 0, 0);
      for(int i = 0; i < 10; i++){
        string name = scores[i][0];
        textout_ex(buffer, font, name.c_str(), 400, (i * 50) + 200, makecol(0,0,0), -1);

        name = scores[i][1];
        textout_right_ex(buffer, font, name.c_str(), 860, (i * 50) + 200, makecol(0,0,0), -1);
      }
    }

    //Draws Cursor
    if(mouse_b & 1){
      draw_sprite( buffer, cursor[1], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
    else{
      draw_sprite( buffer, cursor[0], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
  }

  //Difficulty Select
  else if(gameScreen == 2){
    //Draws menu
    draw_sprite( buffer, mainmenu, 0, 0);

    //Draws Buttons
    start_easy.draw(buffer);
    start_medium.draw(buffer);
    start_hard.draw(buffer);
    back.draw(buffer);

    //Draws Cursor
    if(mouse_b & 1){
      draw_sprite( buffer, cursor[1], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
    else{
      draw_sprite( buffer, cursor[0], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
  }

  //Game
  else if(gameScreen == 3){
    //Draws background
    draw_sprite( buffer, background, 0, 0);

    //Draws Blocks
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        MyBlocks[i][t].draw(buffer, startAnimate);
      }
    }

    //Draws post drawing effects for blocks
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        MyBlocks[i][t].postDraw(buffer, startAnimate);
      }
    }

    //Draws foreground
    draw_sprite( buffer, foreground, 0, 0);

    //Draws text
    textprintf_right_ex(buffer,font,1240,0, makecol(0,0,0),-1,"Blocks Left: %i" ,blocks_left);
    textprintf_ex(buffer,font,40,0, makecol(0,0,0),-1,"Time: %i" ,elaspedTime);

    //Draws Cursor
    if(mouse_b & 1){
      draw_sprite( buffer, cursor[1], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
    else{
      draw_sprite( buffer, cursor[0], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
  }

  //Win screen
  else if(gameScreen == 4){
    //Draw background
    draw_sprite( buffer, background, 0, 0);

    //Draw blocks
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        MyBlocks[i][t].draw(buffer, 0);
      }
    }

    //Create gui
    draw_sprite(buffer,menu,300,300);
    draw_sprite( buffer, foreground, 0, 0);

    textprintf_centre_ex(buffer,font,640,310, makecol(0,0,0),-1,"You Win!");
    textprintf_centre_ex(buffer,font,640,360, makecol(0,0,0),-1,"Score: %i", score);

    //Input rectangle
    rectfill(buffer, 488, 408, 892, 452, makecol(0,0,0));
    rectfill(buffer, 490, 410, 890, 450, makecol(255,255,255));

    // output the string to the screen
    textout_ex(buffer, font, edittext.c_str(), 494, 410, makecol(0,0,0), -1);

    // draw the caret
    vline(buffer, text_length(font, edittext.c_str()) + 494, 412, 448, makecol(0,0,0));

    //Draws Cursor
    if(mouse_b & 1){
      draw_sprite( buffer, cursor[1], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
    else{
      draw_sprite( buffer, cursor[0], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
  }

  //Lose Screen
  if(gameScreen == 5){
    //Draw background
    draw_sprite( buffer, background, 0, 0);

    //Draw blocks
    for(int i=0; i<14; i++){
      for(int t=0; t<9; t++){
        MyBlocks[i][t].draw(buffer, 0);
      }
    }

    //Create gui
    draw_sprite(buffer,menu,300,300);
    draw_sprite( buffer, foreground, 0, 0);

    textprintf_centre_ex(buffer,font,640,310, makecol(0,0,0),-1,"No more moves");
    textprintf_centre_ex(buffer,font,640,360, makecol(0,0,0),-1,"Score: %i", score);

    //Input rectangle
    rectfill(buffer, 488, 408, 892, 452, makecol(0,0,0));
    rectfill(buffer, 490, 410, 890, 450, makecol(255,255,255));

    // output the string to the screen
    textout_ex(buffer, font, edittext.c_str(), 494, 410, makecol(0,0,0), -1);

    // draw the caret
    vline(buffer, text_length(font, edittext.c_str()) + 494, 412, 448, makecol(0,0,0));

    //Draws Cursor
    if(mouse_b & 1){
      draw_sprite( buffer, cursor[1], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
    else{
      draw_sprite( buffer, cursor[0], mouse_x*resDiv-20/resDiv, mouse_y*resDiv-20/resDiv);
    }
  }
  //FPS counter
  if(showFPS){
    textprintf_ex(buffer,font,0,80,makecol(0,0,0),-1,"FPS-%i", fps);
  }

  if( toScreen){
    //Draws buffer
    stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
  }
}

//main function of program
int main(){
  //Initializing
  allegro_init();
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();

  set_color_depth(32);

  set_window_title("Breaker");

  //Setup game
  setup(true);

  //Starts Game
  while(!closeGame && !close_button_pressed){
    //Runs FPS system
    while(ticks == 0){
      rest(1);
    }
    while(ticks > 0){
      int old_ticks = ticks;
      //Update always
      game();
      ticks--;
      if(old_ticks <= ticks){
        break;
      }
    }
    if(game_time - old_time >= 10){
      fps = frames_done;
      frames_done = 0;
      old_time = game_time;
    }
    //Update every set amount of frames
    draw( true);
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN()
