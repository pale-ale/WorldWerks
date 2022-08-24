#pragma once

#include <SFML/Graphics.hpp>

#include "../Tabletop/Token.hpp"
#include "Textbox.hpp"
#include "UISystem.hpp"

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
    tokenTitle = uiSystem->create_widget<Textbox>(shared_from_this(), nullptr,
                                                  sf::Vector2i(size.x, 25));
  }

  /** @brief The Token we are currently displaying */
  const Token *displayedToken = nullptr;
  /** @brief The Textbox that is used to display the title */
  std::shared_ptr<Textbox> tokenTitle;
  /** @brief The Texture containing the background, kept in memory to draw */
  sf::RenderTexture *background;
  /** @brief The color we want our background to be */
  sf::Color bgColor = {20, 25, 30};
};
