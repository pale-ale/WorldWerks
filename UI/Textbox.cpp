#include "Textbox.hpp"

Textbox::Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
                 StrBinding binding, sf::Vector2i size, sf::Vector2i pos)
    : UIElement(uiSystem, parent, size, pos), binding{binding} {
  font.loadFromFile("/home/alba/projects/WorldWerks/HighOne.ttf");
  rtex = std::make_shared<sf::RenderTexture>();
  rtex->create(size.x, size.y);
  redraw();
}

/**
 * @brief Set a new getter/setter to read/write the contents of the textbox.
 *
 * @param newBinding The new binding to use.
 */
void Textbox::set_text_binding(StrBinding newBinding) {
  binding = newBinding;
  redraw();
}

/**
 * @brief Update the sprite's texture with the new changes.
 */
void Textbox::redraw() {
  textImage = sf::Text(binding.get ? binding.get().c_str() : "", font, 16);
  textImage.setFillColor(sf::Color::White);
  rtex->clear(sf::Color(50, 60, 70));
  rtex->draw(textImage);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
}

void Textbox::event_text_input(const char &input) {
  auto str = binding.get();
  if (input == '\b') {
    if (str.length() > 0) str.pop_back();
  } else {
    str += input;
  }
  binding.set(str);
  redraw();
}
