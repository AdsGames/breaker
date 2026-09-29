/**
 * UI style
 * Green panels and buttons that match the game art
 **/
#ifndef UI_STYLE_H
#define UI_STYLE_H

#include <asw/asw.h>
#include <string>

// Black text in font, and green text buttons with a dark border
void applyTheme(asw::ui::Theme& theme, const asw::Font& font);

// Add a green panel with a dark green border. Add the panel contents as its
// children, at screen positions.
asw::ui::Panel& addPanel(asw::ui::Widget& parent, const asw::Quadf& area);

// Add a label at a screen position
asw::ui::Label& addLabel(
    asw::ui::Widget& parent,
    const asw::Vec2f& position,
    const std::string& text,
    const asw::Font& font = nullptr,
    asw::TextJustify justify = asw::TextJustify::Left);

#endif  // UI_STYLE_H
