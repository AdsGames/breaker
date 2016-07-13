/*
 * Block
 * A block object to break
 * Allan Legemaate
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <allegro.h>
#include <string>
#include <time.h>
#include <vector>

#include "particle.h"

class Block{

public:
	Block();
	~Block();

  // Load images from file
	void SetImages( char image1[], char image2[]);

  // Draw
	void draw(BITMAP* tempBitmap, int offset);
	void postDraw(BITMAP* tempBitmap, int offset);
	void DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw, int offset);

  // Do logic of block
	void logic();

  // Explode that block
	void explode();

  // Change images depending on block
	void Change();

	// Get position on screen
	int GetX(){ return x; }
	int GetY(){ return y; }

	// Set position
	void SetX( int newValue){ x = newValue; }
	void SetY( int newValue){ y = newValue; }

  // Get the type (color) of block
	int GetType(){ return type; }

  // Set type of block
	void SetType( int newType){ type = newType; }

  // Check if its selected
	bool GetSelected(){ return selected; }

	// Set wheather block is selected or not
	void SetSelected( bool newSelected){ selected = newSelected; }

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

  // Debris for explosions
	std::vector <particle> debris;

  // Images
	BITMAP *images[2];
};

#endif

