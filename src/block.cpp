#include "block.h"

// Constructor
Block::Block(){
	this -> x = 0;
	this -> y = 0;

	this -> frame = 0;
	this -> type = 0;

	this -> selected = false;

	this -> images[0] = NULL;
	this -> images[1] = NULL;
}

// Deconstructor
Block::~Block(){
  //destroy_bitmap( images[0]);
  //destroy_bitmap( images[1]);
}

// Load images from file
void Block::setImages( const char *image1, const char *image2){
  images[0] = load_bitmap( image1, NULL);
  images[1] = load_bitmap( image2, NULL);
}

// Do logic of block
void Block::logic(){
	for( unsigned int i = 0; i < debris.size(); i++){
		debris.at(i).logic();
		if(random(0,20) == 0){
			debris.erase( debris.begin() + i);
		}
  }
}

// Explode that block
void Block::explode(){
  // Number of particles to expode into
	int numberDivision = 10;

	// Select pixels to make into particles
	for(int i = 0; i < images[0] -> w; i += numberDivision){
    for(int t = 0; t < images[0] -> h; t += numberDivision){
      particle newParticle( i + x, t + y, getpixel( images[0], i, t), random(-8,-1), random(1,8), random(-8,-1), random(1,8), SQUARE, numberDivision);
      debris.push_back( newParticle);
    }
	}
}

// Change images depending on block
void Block::change(){
  switch( getType()){
    case 0:
      setImages("images/blocks/red.png","images/blocks/flash.png");
      break;
    case 1:
      setImages("images/blocks/orange.png","images/blocks/flash.png");
      break;
    case 2:
      setImages("images/blocks/yellow.png","images/blocks/flash.png");
      break;
    case 3:
      setImages("images/blocks/green.png","images/blocks/flash.png");
      break;
    case 4:
      setImages("images/blocks/blue.png","images/blocks/flash.png");
      break;
    case 5:
      setImages("images/blocks/purple.png","images/blocks/flash.png");
      break;
    default:
      setImages("images/blocks/none.png","images/blocks/none.png");
      break;
  }
}

// Draw block to screen
void Block::draw( BITMAP* buffer, int offset){
  // Draw overlay if selected
  draw_sprite( buffer, images[bool(selected * floor(frame/8))], x, y - offset);

  // Increase frame counter
  frame = (frame + 1) % 16;
}

// Draw debris overtop all tiles
void Block::postDraw( BITMAP* buffer, int offset){
	for( unsigned int i = 0; i < debris.size(); i++)
    debris.at(i).draw( buffer);
}
