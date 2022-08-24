#include "TokenUI.hpp"

TokenUI::TokenUI(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
                 const sf::Vector2i &size)
    : UIElement(uiSystem, parent, size) {
  background = new sf::RenderTexture();
  background->create(size.x, size.y);
  background->clear(bgColor);
  background->display();
  sprite.setTexture(background->getTexture());
}

void TokenUI::set_token(Token *token) {
  displayedToken = token;
  tokenTitle->set_text_binding(token ? &token->title : nullptr);
}

void TokenUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (displayedToken) {
    UIElement::draw(target, states);
  }
}
