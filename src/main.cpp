#include <allegro.h>
#include <alpng.h>

#include "mouseListener.h"

#include "state.h"
#include "init.h"
#include "splash.h"
#include "menu.h"
#include "game.h"
#include "globals.h"

// Mouse listener
mouseListener m_listener;

// Current state object
state *currentState = NULL;

// Are we closing?
bool closing = false;

// FPS Tickers
volatile int ticks = 0;
int fps;
int frames_done;
int old_time;
int updates_per_second = 120;
int frames_array[10];
int frame_index = 0;

void ticker(){
	ticks++;
}
END_OF_FUNCTION(ticker)

volatile int game_time = 0;
void game_time_ticker(){
	game_time++;
}
END_OF_FUNCTION(game_time_ticker)

// Close button handler
void close_button_handler( void){
  closing = true;
}
END_OF_FUNCTION( close_button_handler)

// Delete game state and free state resources
void clean_up(){
  delete currentState;
}

// Change game screen
void change_state(){
  // If the state needs to be changed
  if( nextState != STATE_NULL ){
    // Delete the current state
    if( nextState != STATE_EXIT ){
      delete currentState;
    }

    // Change the state
    switch( nextState ){
      case STATE_INIT:
        currentState = new init();
        break;
      case STATE_GAME:
        currentState = new game();
        break;
      case STATE_MENU:
        currentState = new menu();
        break;
      case STATE_SPLASH:
        currentState = new splash();
        break;
      case STATE_EXIT:
        closing = true;
        break;
      default:
        currentState = new game();
    }

    //Change the current state ID
    stateID = nextState;

    //NULL the next state ID
    nextState = STATE_NULL;
  }
}

// Sets up game
void setup(){
  //Set the current state to INIT
  stateID = STATE_INIT;
  currentState = new init();

  // Init vars will be set, use them here
  updates_per_second = config_max_FPS;

  // Setup for FPS system
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

  LOCK_VARIABLE(game_time);
  LOCK_FUNCTION(game_time_ticker);
  install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

  // FPS STUFF
  for(int i = 0; i < 10; i++)
    frames_array[i] = 0;

  // Close button
  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);
}

// Does all game loops
void update(){
  // Change state (if needed)
  change_state();

  // Update listeners
  m_listener.update();

  // Update state
  currentState -> update();
}

// Draw
void draw(){
  // Update state
  currentState -> draw();

  // Draw fps if requested
  if( config_show_fps)
    textprintf_ex( screen, font, 0, 0, 0xFFFFFF, 0x000000, "%i", fps);
}

// main function of program
int main(){
  // Setup
  setup();

  // FPS Counter
  while( !key[KEY_ESC] && !closing){
    while( ticks == 0)
      rest( 1);
    while( ticks > 0){
      int old_ticks = ticks;
      update();
      ticks--;
      if( old_ticks <= ticks){
        break;
      }
    }
    if(game_time >= old_time + 1){// i.e. a 0.1 second has passed since we last counted the frames{
			fps -= frames_array[frame_index];// decrement the fps by the frames done a second ago
			frames_array[frame_index] = frames_done;// store the number of frames done this 0.1 second
			fps += frames_done;// increment the fps by the newly done frames
			frame_index = (frame_index + 1) % 10;// increment the frame index and snap it to 10
			frames_done = 0;
			old_time += 1;
		}
    draw();
    frames_done++;
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN()
