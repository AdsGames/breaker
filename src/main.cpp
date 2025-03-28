// Includes
#include <asw/asw.h>

#include "./state/Game.h"
#include "./state/Init.h"
#include "./state/Intro.h"
#include "./state/Menu.h"
#include "./state/States.h"

// Main function*/
int main() {
  // Game state object
  asw::scene::SceneManager<States> app;

  // Load asw library
  asw::core::init(1280, 960);

  // Init states
  app.registerScene<Init>(States::Init, app);
  app.registerScene<Intro>(States::Intro, app);
  app.registerScene<Menu>(States::Menu, app);
  app.registerScene<Game>(States::Game, app);
  app.setNextScene(States::Init);

  // Background Music
  const auto music = asw::assets::loadSample("assets/sounds/music.ogg");
  asw::sound::play(music, 255, 128, -1);

  // Start app
  app.start();

  return 0;
}
