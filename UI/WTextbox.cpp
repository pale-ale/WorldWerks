#include "WTextbox.hpp"

#include "UISystem.hpp"

WTextbox::WTextbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
                   Binding<string> binding, sf::Vector2i size, sf::Vector2i pos,
                   std::string name)
    : UIElement(uiSystem, parent, name, size, pos), binding{binding} {
  font.loadFromFile("/home/alba/projects/WorldWerks/HighOne.ttf");
  rtex = std::make_shared<sf::RenderTexture>();
  rtex->create(size.x, size.y);
  redraw();
}

/**
 * @brief Set a new getter/setter to read/write the contents of the textbox and redraw it.
 *
 * @param newBinding The new binding to use.
 */
void WTextbox::set_text_binding(Binding<string> newBinding) {
  binding = newBinding;
  redraw();
}

/**
 * @brief Update the sprite's texture with the new changes.
 */
void WTextbox::redraw() {
  textImage = sf::Text(binding.get ? binding.get().c_str() : "", font, 16);
  textImage.setFillColor(textColor);
  rtex->clear(bgColor);
  sf::RenderStates states;
  /// @brief SFML seems to calculate sf::texts' height as twice it's actual height
  int y = (size.y - textImage.getLocalBounds().height + 1) / 2 -
          textImage.getLocalBounds().height;
  int x = 0;
  if (textPosition == POSITION_CENTER) {
    x = (size.x - textImage.getGlobalBounds().width) / 2;
  } else if (textPosition == POSITION_RIGHT) {
    x = size.x - textImage.getGlobalBounds().width;
  }
  states.transform.translate(x, y);
  rtex->draw(textImage, states);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
}

bool WTextbox::event_text_input(const char &input) {
  if (has_focus() && binding.get && binding.set) {
    auto str = binding.get();
    if (input == '\b') {
      if (str.length() > 0) str.pop_back();
    } else {
      str += input;
    }
    binding.set(str);
    redraw();
    return true;
  }
  return false;
}

bool WTextbox::event_clicked() {
  if (!binding.set) {
    return false;
  }
  if (!has_focus()) {
    uiSystem->request_focus(this);
  }
  return true;
}
