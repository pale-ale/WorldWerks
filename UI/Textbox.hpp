#pragma once

#include <SFML/Graphics.hpp>

#include "UIElement.hpp"

/* Displays some text in a rectangle with a background. */
class Textbox : public UIElement {
 public:
  Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
          std::string text, sf::Vector2f size = {100, 50},
          sf::Vector2f pos = {0, 0});

  void set_text(std::string newText);
  void redraw();

 private:
  std::string text;
  sf::Font font;
  sf::Text textImage;
  std::shared_ptr<sf::RenderTexture> rtex;
};