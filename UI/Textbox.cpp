#include "Textbox.hpp"

Textbox::Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
                 std::string text, sf::Vector2f size, sf::Vector2f pos)
    : UIElement(uiSystem, parent, size, pos), text{text} {
  font.loadFromFile("/home/alba/projects/WorldWerks/HighOne.ttf");
  rtex = std::make_shared<sf::RenderTexture>();
  rtex->create(size.x, size.y);
  set_text(text);
}

void Textbox::set_text(std::string newText) {
  this->text = newText;
  redraw();
}

void Textbox::redraw() {
  textImage = sf::Text(text, font, 16);
  textImage.setFillColor(sf::Color::White);
  rtex->clear(sf::Color(50, 60, 70));
  rtex->draw(textImage);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
}