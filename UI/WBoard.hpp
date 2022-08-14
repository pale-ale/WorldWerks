#pragma once

#include "UIElement.hpp"
#include "WToken.hpp"
#include "UISystem.hpp"
#include "../Tabletop/Board.hpp"

class WBoard : public UIElement {
  friend UISystem;
 protected:
  WBoard(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
         const Board* board, SpriteLoader *spriteloader);

  void post_init() override;

  void event_key_down(const sf::Event &keyEvent) override;

  /* Re-create the WTokens from the data in board->tokens. */
  void update_tokens();

  /* Redraw the board with a new texture rect with pan and zoom applied. */
  void update_board_view();

  /* Use this to zoom in / out of the map. */
  void set_scale(float delta);
  
  /* Use this to pan the map up/down/left/right. */
  void change_pan(int newX, int newY);

 private:
  UISystem *uiSystem;
  sf::Texture boardTexture;
  const Board* board;
  sf::IntRect mapTextureRect;
  float minScale = 0.25f;
  float maxScale = 1.0f;
  float viewScale = 1.0f;
  sf::Vector2i desiredPan = {0,0};
};