#pragma once

#include <SFML/Graphics.hpp>

#include "../SpriteLoader/SpriteLoader.h"
#include "../Tabletop/Token.hpp"
#include "WButton.hpp"

class WToken : public WButton {
 public:
  /* The Token displayed by this Widget. */
  const Token token;

  WToken(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
         const Token &token);

  virtual void event_begin_mouse_over() override {
    set_outline_color(sf::Color::White);
  }
  virtual void event_end_mouse_over() override {
    set_outline_color(sf::Color::Black);
  }

  virtual bool is_mouse_inside(const sf::Vector2i &mousePos) override;

 private:
  /* Initialize textures and the sprite. */
  void setup_sprite();

  /* Change the color of the Widget's outline. */
  void set_outline_color(sf::Color color);

  sf::RenderTexture *rtex;
  sf::CircleShape tokenShape;
  float radius = 5;
  float borderThickness = 1;
  constexpr float get_full_radius() { return radius + borderThickness; }
};