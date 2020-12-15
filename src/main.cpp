#include <allegro.h>
#include <chrono>

#include "utility/MouseListener.h"

#include "game.h"
#include "globals.h"
#include "init.h"
#include "menu.h"
#include "splash.h"
#include "state.h"

// Fixed update system
using namespace std::chrono_literals;
using namespace std::chrono;
constexpr nanoseconds timestep(16ms);

// Mouse listener
MouseListener m_listener;

// Current state object
state* currentState = nullptr;

// Are we closing?
bool closing = false;

// Function declarations
void draw();
void update();
void setup();
void change_state();
void clean_up();
void close_button_handler();

// Close button handler
void close_button_handler(void) {
  closing = true;
}
END_OF_FUNCTION(close_button_handler)

// Delete game state and free state resources
void clean_up() {
  delete currentState;
}

// Change game screen
void change_state() {
  // If the state needs to be changed
  if (nextState != STATE_NULL) {
    // Delete the current state
    if (nextState != STATE_EXIT) {
      delete currentState;
    }

    // Change the state
    switch (nextState) {
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

    // Change the current state ID
    stateID = nextState;

    // nullptr the next state ID
    nextState = STATE_NULL;
  }
}

// Sets up game
void setup() {
  // Set the current state to INIT
  stateID = STATE_INIT;
  currentState = new init();

  // Close button
  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);
}

// Does all game loops
void update() {
  // Change state (if needed)
  change_state();

  // Update listeners
  m_listener.update();

  // Update state
  currentState->update();
}

// Draw
void draw() {
  // Update state
  currentState->draw();

  // Draw fps if requested
  if (config_show_fps) {
    // textprintf_ex (screen, font, 0, 0, 0xFFFFFF, 0x000000, "%i", fps);
  }
}

// main function of program
int main() {
  // Setup
  setup();

  using clock = high_resolution_clock;
  nanoseconds lag(0ns);
  auto time_start = clock::now();

  while (!key[KEY_ESC] && !closing) {
    auto delta_time = clock::now() - time_start;
    time_start = clock::now();
    lag += duration_cast<nanoseconds>(delta_time);

    while (lag >= timestep) {
      lag -= timestep;
      update();
    }

    draw();
  }

  clean_up();

  return 0;
}
END_OF_MAIN()
