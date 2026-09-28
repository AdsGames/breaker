/**
 * Block
 * A block object to break
 * Allan Legemaate
 **/

#ifndef BLOCK_H
#define BLOCK_H

#include <asw/asw.h>
#include <array>

class Block {
 public:
  // Special block types. Types 0..TYPE_EMPTY-1 are the playable colors.
  static constexpr int TYPE_EMPTY = 6;  // Empty / cleared cell
  static constexpr int TYPE_FLASH = 7;  // Selection flash overlay
  static constexpr int NUM_TYPES = 8;   // Total entries in the image table

  // On-screen size of a block, in pixels.
  static constexpr float SIZE = 80.0F;

  Block() = default;

  Block(const asw::Vec2<float>& position, int type);

  // Update
  void update(float dt);

  // Draw
  void draw(float offset) const;

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

  // Displace the block from its resting position; update() eases it back to
  // zero, producing the fall (vertical) / slide (horizontal) animation.
  void setVisualOffset(const asw::Vec2<float>& offset);
  const asw::Vec2<float>& getVisualOffset() const;

 private:
  // Load images
  static void loadImages();
  static std::array<asw::Texture, NUM_TYPES> images;

  // Coordinates for screen
  asw::Quad<float> transform{};

  // Type of block
  int type{0};

  // Frame on for flashing
  int frame{0};
  float acc{0.0F};

  // Selected by flash
  bool selected{false};

  // Animated offset from the resting position, and vertical fall velocity.
  asw::Vec2<float> visual_offset{0.0F, 0.0F};
  float fall_velocity{0.0F};
};

#endif
