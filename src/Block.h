/**
 * Block
 * A block object to break
 * Allan Legemaate
 **/

#ifndef BLOCK_H
#define BLOCK_H

#include <asw/asw.h>
#include <array>

#include "ParticleEmitter.h"

class Block {
 public:
  Block() = default;

  Block(const asw::Vec2<float>& position, int type);

  // Update
  void update(float deltaTime);

  // Draw
  void draw(float offset) const;

  // Explode that block
  void explode(ParticleEmitter& emitter) const;

  // Get position on screen
  const asw::Quad<float>& getTransform() const;

  // Get type
  int getType() const;

  // Set type
  void setType(int type);

  // Check if its selected
  bool getSelected() const;

  // Set wheather block is selected or not
  void setSelected(bool selected);

 private:
  // Load images
  static void loadImages();
  static std::array<asw::Texture, 8> images;

  // Coordinates for screen
  asw::Quad<float> transform{};

  // Type of block
  int type{0};

  // Frame on for flashing
  int frame{0};
  float acc{0.0F};

  // Selected by flash
  bool selected{false};
};

#endif
