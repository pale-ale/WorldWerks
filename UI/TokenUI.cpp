#include "TokenUI.hpp"

TokenUI::TokenUI(UISystem *uiSystem, std::weak_ptr<UIElement> parent, const sf::Vector2f &size)
    : UIElement(uiSystem, parent, size) {
  background = new sf::RenderTexture();
  background->create(size.x, size.y);
  background->clear(bgColor);
  background->display();
  bgSprite.setTexture(background->getTexture());
}

void TokenUI::set_token(const Token *token) { 
  displayedToken = token; 
  tokenTitle->set_text(token ? token->title : "");
}

void TokenUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (!displayedToken) {
    return;
  }
  target.draw(bgSprite);
  UIElement::draw(target, states);
}
