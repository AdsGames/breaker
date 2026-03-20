// Includes
#include <asw/asw.h>

#include "./state/Game.h"
#include "./state/Init.h"
#include "./state/Intro.h"
#include "./state/Menu.h"
#include "./state/States.h"

// Main function
int main() {
  // Game state object
  asw::scene::SceneManager<States> app;

  // Load asw library
  asw::core::init(1280, 960);

  // Init states
  app.register_scene<Init>(States::Init, app);
  app.register_scene<Intro>(States::Intro, app);
  app.register_scene<Menu>(States::Menu, app);
  app.register_scene<Game>(States::Game, app);
  app.set_next_scene(States::Init);

  // Background Music
  const auto music = asw::assets::load_music("assets/sounds/music.ogg");
  asw::sound::play_music(music);

  // Start app
  app.start();

  return 0;
}
