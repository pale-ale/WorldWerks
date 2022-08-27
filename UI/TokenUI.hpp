#pragma once

#include <SFML/Graphics.hpp>

#include "../Tabletop/Token.hpp"
#include "Textbox.hpp"
#include "UISystem.hpp"
#include "WBar.hpp"

/**
 @brief Displays information about a token like it's name, properties, inventory, etc..
*/
class TokenUI : public UIElement {
 public:
  TokenUI(UISystem *uiSystem, std::weak_ptr<UIElement> parent, const sf::Vector2i &size);

  /**
   * @brief Set the token to display information of.
   * Set to nullptr to leave the display empty.
   */
  void set_token(Token *token);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  virtual void post_init() override {
    int loffset = 1;
    int toffset = 1;
    int width = size.x - 2 * loffset;
    sf::Vector2i titlePos(loffset, toffset);
    sf::Vector2i titleSize(width, 25);
    sf::Vector2i hpPos(loffset, titlePos.y + titleSize.y);
    sf::Vector2i hpSize(width, 10);
    tokenTitle = uiSystem->create_widget<Textbox>(
        shared_from_this(), Binding<std::string>(), titleSize, titlePos);
    tokenHitpoints = uiSystem->create_widget<WBar>(
        shared_from_this(), GetterFn<int>(), GetterFn<int>(), sf::Color{255,0,0},
        sf::Color{75, 0, 0}, hpSize, hpPos);
  }

  /** @brief The Token we are currently displaying */
  const Token *displayedToken = nullptr;

  /** @brief The Textbox that is used to display the title */
  std::shared_ptr<Textbox> tokenTitle;

  /** @brief The hit points this token has left */
  std::shared_ptr<WBar> tokenHitpoints;

  /** @brief The Texture containing the background, kept in memory to draw */
  sf::RenderTexture *background;

  /** @brief The color we want our background to be */
  sf::Color bgColor = {20, 25, 30};
};
