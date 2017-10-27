/**
 * Block
 * A block object to break
 * Allan Legemaate
**/

#ifndef BLOCK_H
#define BLOCK_H

#include <allegro.h>
#include <vector>

#include "particle_emitter.h"

class Block{

public:
	Block();
	~Block();

  // Load images from file
	void setImages( const char *image1, const char *image2);

  // Draw
	void draw( BITMAP* buffer, int offset);

  // Explode that block
	void explode( particle_emitter &emitter);

  // Change images depending on block
	void change();

	// Get position on screen
	int getX(){ return x; }
	int getY(){ return y; }

	// Get width
	int getWidth() { return images[0] -> w; }
	int getHeight(){ return images[0] -> h; }

	// Set position
	void setX( int x){ this -> x = x; }
	void setY( int y){ this -> y = y; }

  // Get the type (color) of block
	int getType(){ return type; }

  // Set type of block
	void setType( int type){ this -> type = type; }

  // Check if its selected
	bool getSelected(){ return selected; }

	// Set wheather block is selected or not
	void setSelected( bool selected){ this -> selected = selected; }

	// Images
	static BITMAP *images[8];
private:
  // Coordinates for screen
	int x;
	int y;

  // Frame on for flashing
	int frame;

	// Type of block
	int type;

  // Selected by flash
	bool selected;
};

#endif
