#include "state.h"

//State variables
int stateID = STATE_nullptr;
int nextState = STATE_nullptr;

// Set next state
void set_next_state (int newState) {
  //If the user doesn't want to exit
  if (nextState != STATE_EXIT) {
    //Set the next state
    nextState = newState;
  }
}
