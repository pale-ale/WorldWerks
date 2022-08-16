#pragma once

#include "../Tabletop/Board.hpp"
#include "TokenUI.hpp"
#include "UIElement.hpp"
#include "UISystem.hpp"
#include "WToken.hpp"

class WBoard : public UIElement {
  friend UISystem;

 protected:
  WBoard(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
         const Board *board, SpriteLoader *spriteloader,
         const sf::Vector2f &size);

  void post_init() override;

  void event_key_down(const sf::Event &keyEvent) override;

  /* Re-create the WTokens from the data in board->tokens. */
  void update_tokens();

  /* Redraw the board with a new texture rect with pan and zoom applied. */
  void update_board_view();

  /* Set the zoom of the map. 1 = Zoomed out. Use powers of 2, e.g. 0.25.*/
  void set_scale(float newScale);

  /* Use this to pan the map up/down/left/right. */
  void change_pan(int newX, int newY);

  virtual bool event_clicked() override {
    display_token(nullptr);
    return true;
  }

 private:
  /* Set the displayed token. Set to nullptr to not display a token. */
  void display_token(const Token *token);

  std::shared_ptr<TokenUI> tokenUI;
  UISystem *uiSystem;
  sf::Texture texture;
  const Board *board;
  sf::IntRect mapTextureRect;
  float minScale = 0.25f;
  float maxScale = 1.0f;
  float viewScale = 1.0f;
  sf::Vector2i desiredPan = {0, 0};
};