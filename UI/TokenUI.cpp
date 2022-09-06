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
  if (displayedToken == token){
    printf("[TokenUI]: Token already displayed. Nothing to do.\n");
    return;
  }
  printf("[TokenUI]: Set token.\n");
  displayedToken = token;
  if (token) {
    tokenTitle->set_text_binding({[token] { return token->get_title(); },
                                  [token](auto str) { token->set_title(str); }});
    tokenHitpoints->set_current([token] { return 5; });
    tokenHitpoints->set_max([token] { return 15; });
  }
}

void TokenUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (displayedToken) {
    UIElement::draw(target, states);
  }
}
