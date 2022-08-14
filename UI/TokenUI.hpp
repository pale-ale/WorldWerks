#pragma once

#include <SFML/Graphics.hpp>

#include "../Tabletop/Token.hpp"

class TokenUI : public sf::Drawable {
 public:
  void set_token(Token *token);
  TokenUI();

  

 private:
  void on_start_mouseover();
  void on_end_mouseover();
  bool mousedOver;
  Token *displayedToken = nullptr;
  sf::RenderTexture *background;
  sf::Sprite bgSprite;
  void update_background();
  sf::Color bgColor = {20,25,30};
  sf::Vector2f bgSize = {50,150};
  //sf::Vector2f bgPos = {50,150};

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
