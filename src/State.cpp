#include "State.h"

#include <iostream>

#include "Game.h"
#include "Init.h"
#include "Intro.h"
#include "Menu.h"

/*****************
 * STATE ENGINE
 *****************/

// Draw
void StateEngine::draw() const {
  if (state) {
    // Clear screen
    SDL_RenderClear(asw::display::renderer);

    state->draw();

    // Update screen
    SDL_RenderPresent(asw::display::renderer);
  }
}

// Update
void StateEngine::update() {
  if (state) {
    state->update();
  }

  changeState();
}

// Set next state
void StateEngine::setNextState(const ProgramState newState) {
  nextState = newState;
}

// Get state id
ProgramState StateEngine::getStateId() const {
  return currentState;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == ProgramState::STATE_NULL) {
    return;
  }

  // Delete the current state
  if (state) {
    state->cleanup();
    state = nullptr;
  }

  // Change the state
  switch (nextState) {
    case ProgramState::STATE_GAME:
      state = std::make_unique<Game>(*this);
      std::cout << "Switched state to game." << std::endl;
      break;

    case ProgramState::STATE_MENU:
      state = std::make_unique<Menu>(*this);
      std::cout << "Switched state to main menu." << std::endl;
      break;

    case ProgramState::STATE_INIT:
      state = std::make_unique<Init>(*this);
      std::cout << "Switched state to init." << std::endl;
      break;

    case ProgramState::STATE_INTRO:
      state = std::make_unique<Intro>(*this);
      std::cout << "Switched state to intro." << std::endl;
      break;

    default:
      std::cout << "Exiting program." << std::endl;
      break;
  }

  state->init();

  // Change the current state ID
  currentState = nextState;

  // NULL the next state ID
  nextState = ProgramState::STATE_NULL;
}

/*********
 * STATE
 *********/

// Change state
void State::setNextState(const ProgramState state) {
  this->engine.setNextState(state);
}