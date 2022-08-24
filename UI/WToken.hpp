#pragma once

#include <SFML/Graphics.hpp>

#include "../SpriteLoader/SpriteLoader.h"
#include "../Tabletop/Token.hpp"
#include "WButton.hpp"

/**
 * @brief This displays a token on the map.
 * By default the token will be a red circle with a black outline.
 */
class WToken : public WButton {
 public:
  WToken(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
         const Token &token);

  virtual void event_begin_mouse_over() override {
    set_outline_color(sf::Color::White);
  }

  virtual void event_end_mouse_over() override {
    set_outline_color(sf::Color::Black);
  }

  virtual bool is_mouse_inside(const sf::Vector2i &mousePos) override;

  /** @brief The Token displayed by this Widget */
  const Token* token;

 private:
  void setup_sprite();
  void set_outline_color(sf::Color color);

  /**
   * @brief Get the full radius of the token, including the outline.
   * 
   * @return constexpr float --- The full radius
   */
  constexpr float get_full_radius() { return radius + borderThickness; }

  /** @brief The rendertexture used to store the graphics for the token. */
  sf::RenderTexture *rtex;

  /** @brief The used to draw the shape. */
  sf::CircleShape tokenShape;
  
  /** @brief The radius of the inner circle. */
  float radius = 5;
  
  /** @brief The thickness of the border in pixels. */
  float borderThickness = 1;
};