/**
 * Block
 * A block object to break
 * Allan Legemaate
**/

#ifndef BLOCK_H
#define BLOCK_H

#include <allegro.h>

#include "particle_emitter.h"

class Block {

  public:
    // Ctor / Dtor
    Block();
    Block(int x, int y, int type);
    ~Block();

    // Draw
    void draw (BITMAP *buffer, int offset);

    // Explode that block
    void explode (particle_emitter &emitter);

    // Change images depending on block
    void change();

    // Get position on screen
    int getX() const;
    int getY() const;

    // Get width
    static int getWidth();
    static int getHeight();

    // Get type
    int getType() const;

    // Set type
    void setType(int type);

    // Check if its selected
    bool getSelected() const;

    // Set wheather block is selected or not
    void setSelected (bool selected);

  private:
    // Load images
    static void loadImages();
    static BITMAP *images[8];
    static int block_count;

    // Coordinates for screen
    int x, y;

    // Type of block
    int type;

    // Frame on for flashing
    int frame;

    // Selected by flash
    bool selected;
};

#endif
