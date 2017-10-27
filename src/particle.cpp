#include "particle.h"

// Constructor
particle::particle( int x = 0, int y = 0, vec2 velocity = vec2( 0), vec2 acceleration = vec2( 0), vec2 size = vec2( 1), int colorStart = 0xFFFFFF, int colorEnd = 0xFFFFFF, int life = 100, char type = PIXEL, bool trans_life = false){
  this -> x = x;
  this -> y = y;

  this -> velocity = velocity;
  this -> acceleration = acceleration;

  this -> colorStart = colorStart;
  this -> colorEnd = colorEnd;
  this -> color = colorStart;

  this -> size = size;

  this -> life = life;
  this -> life_start = life;

  this -> image = NULL;

  this -> type = type;
  this -> trans_life = trans_life;

  this -> onDeath = NULL;

  mix_colors();
}

// Make new color
void particle::mix_colors(){
  // For readability
  int c_red = (getr(colorStart) * life + getr(colorEnd) * (life_start - life)) / life_start;
  int c_green = (getg(colorStart) * life + getg(colorEnd) * (life_start - life)) / life_start;
  int c_blue = (getb(colorStart) * life + getb(colorEnd) * (life_start - life)) / life_start;

  this -> color = makecol( c_red, c_green, c_blue);
}

// Is dead
bool particle::dead(){
  if( life <= 0){
    if( onDeath == NULL){
      return true;
    }
    else{
      velocity = onDeath -> velocity;
      acceleration = onDeath -> acceleration;
      size = onDeath -> size;
      colorStart = onDeath -> colorStart;
      colorEnd = onDeath -> colorEnd;
      life = onDeath -> life;
      type = onDeath -> type;
      trans_life = onDeath -> trans_life;
      image = onDeath -> image;
      onDeath = onDeath -> onDeath;
    }
  }
  return false;
}

// Set image
void particle::set_image( BITMAP *image){
  this -> image = image;
  this -> type = IMAGE;
}

// Set particle to spawn on death
void particle::set_particle_ondeath( particle *onDeath){
  this -> onDeath = onDeath;
}

// Logic
void particle::update( int dt){
  if( !dead()){
    life -= dt;
    x += velocity.x * dt;
    y += velocity.y * dt;
    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;
    mix_colors();
  }
}

// Draw
void particle::draw( BITMAP* tempBitmap){
  if( !dead()){
    if( trans_life){
      set_trans_blender( 255, 255, 255, int(float(life)/life_start * 255));
    }

    if( type == IMAGE && image != NULL){
      set_alpha_blender();
      draw_trans_sprite( tempBitmap, image, x - image -> w / 2, y - image -> w / 2);
    }
    else if( type == PIXEL){
      putpixel( tempBitmap, x, y, color);
    }
    else if( type == CIRCLE){
      circlefill( tempBitmap, x, y, size.x/2, color);
    }
    else if( type == RECTANGLE){
      rectfill( tempBitmap, x, y, x + size.x, y + size.y, color);
    }
  }
}
