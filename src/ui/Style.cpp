#include "Style.h"

namespace {
const asw::Color GREEN{0, 128, 0};
const asw::Color GREEN_DARK{0, 88, 0};
const asw::Color GREEN_LIGHT{0, 186, 0};
const asw::Color GREEN_PRESSED{0, 150, 0};
const asw::Color BLACK{0, 0, 0};

// Width of the dark border around panels
constexpr float PANEL_BORDER = 8.0F;
}  // namespace

void applyTheme(asw::ui::Theme& theme, const asw::Font& font) {
  theme.font = font;
  theme.text = BLACK;

  theme.button.bg = GREEN;
  theme.button.bg_hover = GREEN_LIGHT;
  theme.button.bg_pressed = GREEN_PRESSED;
  theme.button.text = BLACK;
  theme.button.text_hover = BLACK;
  theme.button.border = GREEN_DARK;
  theme.button.border_width = 3.0F;
}

asw::ui::Panel& addPanel(asw::ui::Widget& parent, const asw::Quadf& area) {
  auto& border = parent.add_child<asw::ui::Panel>();
  border.transform = area;
  border.bg = GREEN_DARK;

  auto& fill = border.add_child<asw::ui::Panel>();
  fill.transform =
      asw::Quadf(area.position.x + PANEL_BORDER, area.position.y + PANEL_BORDER,
                 area.size.x - (PANEL_BORDER * 2), area.size.y - (PANEL_BORDER * 2));
  fill.bg = GREEN;

  return border;
}

asw::ui::Label& addLabel(asw::ui::Widget& parent,
                         const asw::Vec2f& position,
                         const std::string& text,
                         const asw::Font& font,
                         asw::TextJustify justify) {
  auto& label = parent.add_child<asw::ui::Label>();
  label.transform.position = position;
  label.text = text;
  label.font = font;
  label.justify = justify;
  return label;
}
