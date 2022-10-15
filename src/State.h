/**
 * State for machine and State Engine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program into states
 *   which can handle only their own logic,
 *   drawing and transitions
 */

#ifndef STATE_H
#define STATE_H

#include <asw/asw.h>
#include <memory>

// Class
class State;

// Game states
enum class ProgramState {
  STATE_INIT,
  STATE_GAME,
  STATE_MENU,
  STATE_INTRO,
  STATE_EXIT,
  STATE_NULL
};

/*****************
 * STATE ENGINE
 *****************/
class StateEngine {
 public:
  // Update
  void update();

  // Draw
  void draw() const;

  // Set next state
  void setNextState(const ProgramState state);

  // Get state id
  ProgramState getStateId() const;

 private:
  // Change state
  void changeState();

  // Next state
  ProgramState nextState{ProgramState::STATE_NULL};

  // State id
  ProgramState currentState{ProgramState::STATE_NULL};

  // Stores states
  std::unique_ptr<State> state{nullptr};
};

/*********
 * STATE
 *********/
class State {
 public:
  // Constructor
  explicit State(StateEngine& engine) : engine(engine){};

  virtual ~State() = default;

  // Init the state
  virtual void init() = 0;

  // Draw to screen
  virtual void draw() = 0;

  // Cleanup
  virtual void cleanup() = 0;

  // Update logic
  virtual void update() = 0;

  // Change state
  void setNextState(const ProgramState state);

 private:
  StateEngine& engine;
};

#endif  // STATE_H
