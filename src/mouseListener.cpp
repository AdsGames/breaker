#include "mouseListener.h"

bool mouseListener::buttonPressed[MAX_MOUSE_BUTTONS] = { false};
bool mouseListener::buttonReleased[MAX_MOUSE_BUTTONS] = { false};
bool mouseListener::lastTicksButton[MAX_MOUSE_BUTTONS] = { false};

mouseListener::mouseListener(){
}

mouseListener::~mouseListener(){
}


// Check those buttons!
void mouseListener::update(){
  // Check button just pressed
  for( int i = 1; i < MAX_MOUSE_BUTTONS; i++){
    // Clear old values
    buttonPressed[i] = false;
    buttonReleased[i] = false;

    // Pressed since last tick?
    if( (mouse_b & i) != false && lastTicksButton[i] == false){
      buttonPressed[i] = true;
      //std::cout << "Button: " << i << " pressed. \n";
    }

    // Released since last tick?
    if( (mouse_b & i) == false && lastTicksButton[i] != false){
      buttonReleased[i] = true;
      //std::cout << "Button: " << i << " released. \n";
    }

    // Button changed
    if( lastTicksButton[i] != (mouse_b & i)){
        lastTicksButton[i] = (mouse_b & i);
        //std::cout << "Button: " << i << " was " << lastTicksButton[i] << " and became " << (mouse_b & i) << "\n";
    }
  }
}
