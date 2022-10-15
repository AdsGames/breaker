// Includes
#include <asw/asw.h>

#include <chrono>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// For state engine
#include "State.h"

using namespace std::chrono_literals;
using namespace std::chrono;
constexpr nanoseconds timestep(16ms);

// State engine
std::unique_ptr<StateEngine> state;

// Functions
void setup();
void draw();
void update();

// FPS system
int fps = 0;
int frames_done = 0;

// Setup game
void setup() {
  // Load allegro library
  asw::core::init(1280, 960);

  state = std::make_unique<StateEngine>();
}

// Update
void update() {
  // Update core
  asw::core::update();

  // Do state logic
  state->update();

  // Handle exit
  if (state->getStateId() == ProgramState::STATE_EXIT) {
    asw::core::exit = true;
  }
}

// Do state rendering
void draw() {
  state->draw();
}

// Loop (emscripten compatibility)
#ifdef __EMSCRIPTEN__
void loop() {
  update();
  draw();
}
#endif

// Main function*/
int main(int argc, char* argv[]) {
  // Setup basic functionality
  setup();

  // Set the current state ID
  state->setNextState(ProgramState::STATE_INIT);

  // Background Music
  asw::Sample music = asw::assets::loadSample("assets/sounds/music.ogg");
  asw::sound::play(music, 255, 128, -1);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
#else

  using clock = high_resolution_clock;
  nanoseconds lag(0ns);
  auto time_start = clock::now();

  while (!asw::input::keyboard.down[SDL_SCANCODE_ESCAPE] && !asw::core::exit) {
    auto delta_time = clock::now() - time_start;
    time_start = clock::now();
    lag += duration_cast<nanoseconds>(delta_time);

    while (lag >= timestep) {
      lag -= timestep;
      update();
    }

    draw();
    frames_done++;
  }
#endif

  return 0;
}
