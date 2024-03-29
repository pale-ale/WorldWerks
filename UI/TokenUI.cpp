#include "TokenUI.hpp"

#include "../TooDeeEngine/Util/Log.hpp"

TokenUI::TokenUI(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
                 const sf::Vector2i &size)
    : UIElement(uiSystem, parent, "TokenUI", size) {
  background = new sf::RenderTexture();
  background->create(size.x, size.y);
  background->clear(bgColor);
  background->display();
  sprite.setTexture(background->getTexture());
}

void TokenUI::set_token(Token *token) {
  if (displayedToken == token) {
    LOGDBG("TokenUI", "Token already displayed. Nothing to do.");
    return;
  }
  LOGDBG("TokenUI", "Set token.");
  displayedToken = token;
  if (token) {
    tokenTitle->set_text_binding({[token] { return token->get_title(); },
                                  [token](auto str) { token->set_title(str);}});
    tokenTitle->endEditCallback = [token](){token->send_data_changed();};
    tokenHitpoints->set_current([token] { return 5; });
    tokenHitpoints->set_max([token] { return 15; });
  }
}

void TokenUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (displayedToken) {
    UIElement::draw(target, states);
  }
}
