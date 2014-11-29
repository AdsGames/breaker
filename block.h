#ifndef BLOCK_H
#define BLOCK_H

#include <allegro.h>
#include <string>
#include <time.h>

#include "particle.h"

using namespace std;

class Block{

public:
	Block();
	~Block();
	
	void SetImages( char image1[], char image2[]);
	
	int GetY();     
	int GetX();
	
	int GetType();
	
	bool GetSelected();
	
	bool GetDead();
	
	BITMAP* GetImage();
	
	void draw(BITMAP* tempBitmap, int offset);
	void postDraw(BITMAP* tempBitmap, int offset);
	void DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw, int offset);  
	
	void SetX( int newValue);
	void SetY( int newValue);
	
	void logic();
	
	void SetType(int newType); 
	void SetValue(int newValue);  
	
	void SetSelected(bool newSelected);
	
	void explode();
	
	void Change();
        
private:
	int x;
	int y;
	
	int frame;
	int type;
	int value;
	
	bool selected;
	
	vector<particle> debris;
	
	BITMAP *images[2][2]; 
};

#endif

