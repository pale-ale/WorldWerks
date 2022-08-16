#pragma once

#include <SFML/Graphics.hpp>

#include "../Tabletop/Token.hpp"
#include "Textbox.hpp"
#include "UISystem.hpp"

/* 
 Class used to display information about a token like it's name, properties,
 inventories etc. 
*/
class TokenUI : public UIElement {
 public:
  TokenUI(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
          const sf::Vector2f &size);

  /* Set the token to display information of. Set to nullptr to leave fields empty. */
  void set_token(const Token *token);

  virtual void event_position_updated() override {
    bgSprite.setPosition(get_parent_position() + relativePosition);
  }

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  virtual void post_init() override {
    tokenTitle = uiSystem->create_widget<Textbox>(
        shared_from_this(), "<no token selected>", sf::Vector2f(size.x, 25));
  }

  const Token *displayedToken = nullptr;
  std::shared_ptr<Textbox> tokenTitle;
  sf::RenderTexture *background;
  sf::Sprite bgSprite;
  sf::Color bgColor = {20, 25, 30};
};
