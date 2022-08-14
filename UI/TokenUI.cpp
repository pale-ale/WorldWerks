#include "TokenUI.hpp"

TokenUI::TokenUI(){
  background = new sf::RenderTexture();
  background->create(bgSize.x, bgSize.y);
  background->clear(bgColor);
  background->display();
  bgSprite.setTexture(background->getTexture());
}

void TokenUI::draw(sf::RenderTarget &target, sf::RenderStates states) const{
  if (!displayedToken){
    return;
  }
  target.draw(bgSprite);
}

void TokenUI::update_background(){
  background->clear(bgColor);
}

void TokenUI::set_token(Token *token){
  displayedToken = token;
}
