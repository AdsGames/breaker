#include "block.h"

// Constructor
Block::Block(){

}

// Deconstructor
Block::~Block(){
  delete images;
}

// Load images from file
void Block::SetImages( const char *image1, const char *image2){
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
void Block::Change(){
  switch( GetType()){
    case 0:
      SetImages("images/blocks/red.png","images/blocks/flash.png");
      break;
    case 1:
      SetImages("images/blocks/orange.png","images/blocks/flash.png");
      break;
    case 2:
      SetImages("images/blocks/yellow.png","images/blocks/flash.png");
      break;
    case 3:
      SetImages("images/blocks/green.png","images/blocks/flash.png");
      break;
    case 4:
      SetImages("images/blocks/blue.png","images/blocks/flash.png");
      break;
    case 5:
      SetImages("images/blocks/purple.png","images/blocks/flash.png");
      break;
    default:
      SetImages("images/blocks/none.png","images/blocks/none.png");
      break;
  }
}

// Draw block to screen
void Block::draw(BITMAP* tempBitmap, int offset){
  // Draw overlay if selected
  draw_sprite(tempBitmap, images[bool(selected * floor(frame/8))], x, y - offset);

  // Increase frame counter
  frame ++;
  if(frame == 16)
    frame = 0;
}

// Draw debris overtop all tiles
void Block::postDraw(BITMAP* tempBitmap, int offset){
	for( unsigned int i = 0; i < debris.size(); i++)
    debris.at(i).draw( tempBitmap);
}
