#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.hpp"

class Textbox : public UIElement{
  public:
  Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent, std::string text, sf::Vector2f pos = {0,0}):UIElement(uiSystem, parent), text{text}{
    font.loadFromFile("/home/alba/projects/WorldWerks/HighOneMono.ttf");
    set_text(text);
    set_position(pos);
  }

  void set_text(std::string text){
    textImage = sf::Text(text, font, 16);
    textImage.setFillColor(sf::Color::Black);
  }

  void set_position(const sf::Vector2f &pos){
    position = pos;
    textImage.setPosition(pos);
  }

  private:
  std::string text;
  sf::Font font;
  sf::Text textImage;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(textImage);
  };
};