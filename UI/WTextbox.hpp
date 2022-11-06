#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "LayoutParams.hpp"
#include "UIBinding.hpp"
#include "UIElement.hpp"

using std::string;

/**
 * @brief Displays some text in a rectangle with a background.
 */
class WTextbox : public UIElement {
 public:
  WTextbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent, Binding<string> binding,
           sf::Vector2i size = {100, 50}, sf::Vector2i pos = {0, 0},
           std::string name = "WTextbox");
  void redraw();
  void set_text_binding(Binding<string> newBinding);
  virtual bool event_text_input(const char &input) override;
  virtual bool event_clicked() override;
  virtual bool event_mouse_down(const sf::Vector2i &mousePos) override {
    return binding.set && is_mouse_inside(mousePos);
  }
  virtual bool event_mouse_up(const sf::Vector2i &mousePos) override {
    return binding.set && is_mouse_inside(mousePos);
  }
  sf::Color textColor = {255, 255, 255};
  sf::Color bgColor = {50, 60, 70};
  ELayoutParams textPosition = ELayoutParams::POSITION_CENTER;

 private:
  /** @brief The text source */
  Binding<string> binding;
  /** @brief Font needed to draw the text */
  sf::Font font;
  /** @brief Essentially a sf::Texture, needs to be kept in memory */
  sf::Text textImage;
  /** @brief Also needs to be kept in memory to be drawn */
  std::shared_ptr<sf::RenderTexture> rtex;
};
