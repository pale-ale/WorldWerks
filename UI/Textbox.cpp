#include "Textbox.hpp"

Textbox::Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent, std::string *text,
                 sf::Vector2i size, sf::Vector2i pos)
    : UIElement(uiSystem, parent, size, pos), text{text} {
  font.loadFromFile("/home/alba/projects/WorldWerks/HighOne.ttf");
  rtex = std::make_shared<sf::RenderTexture>();
  rtex->create(size.x, size.y);
  redraw();
}

void Textbox::set_text_binding(std::string *newText) {
  text = newText;
  redraw();
}

void Textbox::redraw() {
  textImage = sf::Text(text ? text->c_str() : "", font, 16);
  textImage.setFillColor(sf::Color::White);
  rtex->clear(sf::Color(50, 60, 70));
  rtex->draw(textImage);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
}
