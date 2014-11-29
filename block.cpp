#include "block.h"

using namespace std;

Block::Block(){
    
}

Block::~Block(){
  delete [] images[0][0], images[0][1];
}

void Block::SetImages( char image1[], char image2[]){  
  images[0][0]= load_bitmap(image1, NULL);
  images[0][1]= load_bitmap(image2, NULL); 
}

int Block::GetType(){
   return type;
}
     
int Block::GetX(){
   return x;
}
     
int Block::GetY(){
   return y;
}

bool Block::GetSelected(){
   return selected;
}

BITMAP* Block::GetImage(){
   return images[0][0];
}

void Block::SetType(int newType){
	type = newType;
}

void Block::SetSelected(bool newSelected){
   selected = newSelected;
}

void Block::SetX( int newValue){
   x = newValue;
}

void Block::SetY( int newValue){
   y = newValue;
}

void Block::logic(){
	for( int i = 0; i < debris.size(); i++){
		debris.at(i).logic();
		if(random(0,20) == 0){
			debris.erase( debris.begin() + i);
		}
  }
}

void Block::draw(BITMAP* tempBitmap, int offset){
  if(selected){
    if(frame==0||frame==1||frame==2||frame==3||frame==4||frame==5||frame==6||frame==7){
      frame+=1;
      DrawNewSprite( tempBitmap, images[0][0], offset);
    }
    else if(frame==8||frame==9||frame==10||frame==11||frame==12||frame==13||frame==14||frame==15){
      frame+=1;
      DrawNewSprite( tempBitmap, images[0][1], offset);
      if(frame == 16){
        frame=0;
      }
    }
  }
  else{
    frame = 5;
    DrawNewSprite( tempBitmap, images[0][0], offset);
  }
}

void Block::postDraw(BITMAP* tempBitmap, int offset){
	for( int i = 0; i < debris.size(); i++){
    debris.at(i).draw( tempBitmap);
  }
}

void Block::explode(){
	int numberDivision = 16;
	int iteratorX = 0;
	int iteratorY = 0;
	for(int i = 0; i < 80/numberDivision * 80/numberDivision; i += 1){ 
	  if(iteratorX < 80 - numberDivision){
	    iteratorX += numberDivision;
	  }
	  else{
	    iteratorX = 0;
	    iteratorY += numberDivision;
	  }
	  particle newParticle( iteratorX + x, iteratorY + y, getpixel(images[0][0], iteratorX, iteratorY), random(-8,-1), random(1,8), random(-8,-1), random(1,8), SQUARE, numberDivision);
		debris.push_back( newParticle);
	}
}

void Block::Change(){
  if(GetType() == 0){
		SetImages("images/blocks/red.png","images/blocks/flash.png");
	}
  else if(GetType() == 1){
		SetImages("images/blocks/orange.png","images/blocks/flash.png");
	}
  else if(GetType() == 2){
		SetImages("images/blocks/yellow.png","images/blocks/flash.png");
	}
  else if(GetType() == 3){
		SetImages("images/blocks/green.png","images/blocks/flash.png");
	}
  else if(GetType() == 4){
		SetImages("images/blocks/blue.png","images/blocks/flash.png");
	}
  else if(GetType() == 5){
		SetImages("images/blocks/purple.png","images/blocks/flash.png");
	}
  else if(GetType() == 6){
		SetImages("images/blocks/none.png","images/blocks/none.png");
	}     
}

void Block::DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw, int offset){
  draw_sprite(tempBitmap, spriteToDraw, GetX(), GetY() - offset);  
}
