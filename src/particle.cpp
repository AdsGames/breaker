#include "particle.h"

// Constructor
particle::particle(int newX, int newY, int newColor, int newXMin, int newXMax, int newYMin, int newYMax, char newType, int newSize){
  // Position
  x = newX;
  y = newY;

  // Colors
  particleColor = newColor;
  particleRed = getr(particleColor);
  particleGreen = getg(particleColor);
  particleBlue = getb(particleColor);

  // Type
  type = newType;

  // Select one
  if( type == RANDOM){
    type = random( 0, 2);
  }

  // Size
  size = newSize;

  // Max movement in each direction
  xMin = newXMin;
  xMax = newXMax;
  yMin = newYMin;
  yMax = newYMax;
}

// Destructor
particle::~particle(){

}

// Logic
void particle::logic(){
  x += random( xMin, xMax);
  y += random( yMin, yMax);
}

// Draw
void particle::draw( BITMAP* tempBitmap){
  // Single pixel
  if(type == PIXEL){
    putpixel( tempBitmap, x, y, makecol( particleRed, particleGreen, particleBlue));
  }
  // Square
  else if(type == SQUARE){
    rectfill( tempBitmap, x, y, x + size, y + size, makecol( particleRed, particleGreen, particleBlue));
  }
  // Ellipse
  else if(type == CIRCLE){
    circlefill( tempBitmap, x, y, size, makecol( particleRed, particleGreen, particleBlue));
  }
  // Randomly change
  else if(type == RANDOM){
    switch(random(0,3)){
      case 0:
        putpixel( tempBitmap, x, y, makecol( particleRed, particleGreen, particleBlue));
        break;
      case 1:
        circlefill( tempBitmap, x, y, size, makecol( particleRed, particleGreen, particleBlue));
        break;
      case 2:
        rectfill( tempBitmap, x, y, x + size, y + size, makecol( particleRed, particleGreen, particleBlue));
        break;
    }
  }
}
