#include "particle_emitter.h"

// Constructor basic
particle_emitter::particle_emitter(){
  // Position
  this -> position = vec2( 0, 0);
  this -> size = vec2( 0, 0);
  type = 0;
}

// Constructor with parameters
particle_emitter::particle_emitter( vec2 position, vec2 size){
  // Position
  this -> position = position;
  this -> size = size;

  launch_velocity = vec2( -10, 10);

  // Load image
  image = load_bitmap( "images/fuzzball.png", NULL);
  image2 = load_bitmap( "images/fuzzball2.png", NULL);
  image3 = load_bitmap( "images/fuzzball_blue.png", NULL);
}

// Destructor
particle_emitter::~particle_emitter(){

}

// Create particles
void particle_emitter::create_particle( int type){
  // Smoke
  if( type == 0){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -0.1, 0.2), randomf( -0.1, -0.5)), vec2( 0.003, -0.003),
                      vec2( random( 5, 8)), 0xFFFFFF, 0x000000, random( 500, 1000), IMAGE, true);
    newPart.set_image( image);
    particles.push_back( newPart);
  }
  // Fire
  if( type == 1){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -0.5, 0.5), randomf( -1, -2)), vec2( 0, randomf( 0.01, 0.03)),
                      vec2( random( 4, 8)), 0xFFAA00, 0xFF0000, random( 20, 80), RECTANGLE, false);
    particles.push_back( newPart);
  }
  // Water
  if( type == 2){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -1, 1), randomf( -10, -5)), vec2( 0, 0.1),
                      vec2( random( 2, 6)), 0x0000FF, 0x000000, random( 200, 400), CIRCLE, true);
    particles.push_back( newPart);
  }
  // Spark
  if( type == 3){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -2, 2), randomf( -3, -5)), vec2( 0, randomf( 0.1, 0.2)),
                      vec2( 0), 0xFFAA00, 0xFF0000, random( 10, 50), PIXEL, false);
    particles.push_back( newPart);
  }
  // Cool bubbles
  if( type == 4){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -0.01, 0.01), randomf( -0.1, -0.1)), vec2( 0, randomf( -0.01, -0.02)),
                      vec2( random( 10, 30)), 0x0000FF, 0xFF0000, random( 100, 500), CIRCLE, false);
    particles.push_back( newPart);
  }
  // Rain
  if( type == 5){
    particle *newPart3 = new particle( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( 0, -0.5), vec2( 0, 0.01),
                      vec2( 0), 0x0000AA, 0xFFFFFF, 100, PIXEL, false);

    particle *newPart2 = new particle( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( 0), vec2( 0.005, 0.05),
                      vec2( 1, random( 3, 20)), 0x0000FF, 0x0000AA, 120, RECTANGLE, false);

    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( 0.05, 0.1), 0), vec2( 0),
                      vec2( 0), 0xFFFFFF, 0x000000, random( 100, 300), IMAGE, true);
    newPart.set_image( image2);

    newPart2 -> onDeath = newPart3;
    newPart.onDeath = newPart2;
    particles.push_back( newPart);
  }
  // Explosion (this is just wrong!) FRIGGED IN THE HEAD IF U WILL
  if( type == 6){
    // On death creation
    particle *newPart2 = new particle( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -0.1, 0.1), randomf( 0.1, -0.1)), vec2( 0),
                      vec2( random( 5, 8)), 0xFFFFFF, 0x000000, random( 100, 400), IMAGE, true);
    newPart2 -> set_image( image);

    // Our primary particle
    particle *newPart = new particle( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( 0), vec2( randomf( -0.2, 0.2), randomf( -0.2, 0.2)),
                      vec2( random( 3, 8)), 0xFFFF00, 0xFF0000, 50, RECTANGLE, false);

    // Point to each other
    newPart -> onDeath = newPart2;
    newPart2 -> onDeath = newPart;

    particles.push_back( *newPart);
  }
  // Clouds
  if( type == 7){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( 0.05, 0.1), 0), vec2( 0),
                      vec2( 0), 0xFFFFFF, 0x000000, random( 1800, 2000), IMAGE, true);
    newPart.set_image( image2);
    particles.push_back( newPart);
  }
  // Comet
  if( type == 8){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -0.4, 0.4), randomf( -0.2, 0.2)), vec2( 0.03, -0.03),
                      vec2( random( 5, 8)), 0xFFFFFF, 0x000000, random( 300, 500), IMAGE, true);
    newPart.set_image( image3);
    particles.push_back( newPart);
  }
  // Explosion real
  if( type == 9){
    particle newPart( random( position.x, position.x + size.x), random( position.y, position.y + size.y),
                      vec2( randomf( -0.2, 0.2), randomf( -0.2, 0.2)), vec2( randomf( -0.2, 0.2), randomf( -0.2, 0.2)),
                      vec2( random( 3, 8)), 0xFFAA00, 0xFF0000, random( 5, 30), RECTANGLE, false);
    particles.push_back( newPart);
  }
}

// Make a user defined particle
void particle_emitter::create_particle( particle newPart){
  particles.push_back( newPart);
}

// Move
void particle_emitter::move_to( vec2 position){
  this -> position.x += position.x - size.x;
  this -> position.y += position.y - size.y;
  this -> position = position;
}

// Update
void particle_emitter::update( int dt){
  // Update each particle
  for( unsigned int i = 0; i < particles.size(); i++){
    if( particles.at(i).dead() || key[KEY_BACKSPACE]){
      particles.erase( particles.begin() + i);
    }
    else{
      particles.at(i).update( dt/4);
    }
  }
}

// Draw
void particle_emitter::draw( BITMAP *tempBitmap){
  // Show how many particles are on screen
  //textprintf_ex( tempBitmap, font, 0, 30 + (20 * type), 0xFFFFFF, -1, "Particles:%i", particles.size());

  // Draw emitter
  circle( tempBitmap, position.x + size.x/2, position.y + size.y/2, 2, 0xFF0000);

  // Update each particle
  for( unsigned int i = 0; i < particles.size(); i++){
    particles.at(i).draw( tempBitmap);
  }
}

