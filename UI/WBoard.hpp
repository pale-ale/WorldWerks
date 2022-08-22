#pragma once

#include "../Tabletop/Board.hpp"
#include "TokenUI.hpp"
#include "UIElement.hpp"
#include "UISystem.hpp"
#include "WToken.hpp"

class WBoard : public UIElement {
  friend UISystem;

 protected:
  WBoard(UISystem *uiSystem, std::shared_ptr<UIElement> parent, Board *board,
         SpriteLoader *spriteloader, const sf::Vector2f &size);

  void post_init() override;

  void event_key_down(const sf::Event &keyEvent) override;

  /**
   * @brief Re-create the WTokens from the data in board->tokens.
   */
  void update_tokens();

  /**
   * @brief Redraw the board with a new texture rect with pan and zoom applied.
   */
  void update_board_view();

  /**
   * @brief Set the zoom of the map. 1 = Zoomed out. Use powers of 2, e.g. 0.25.
   */
  void set_scale(float newScale);

  /**
   * @brief Use this to pan the map up/down/left/right.
   */
  void change_pan(int newX, int newY);

  /**
   * @brief When clicked, stop displaying the selected token.
   *
   * @return true, because this will always handle the click event
   */
  virtual bool event_clicked() override {
    display_token(nullptr);
    return true;
  }

 private:
  /**
   * @brief Set the displayed token. Set to nullptr to not display a token.
   */
  void display_token(const Token *token);

  /** @brief Pointer to the UIElement used to display the Token's information */
  std::shared_ptr<TokenUI> tokenUI;

  /** @brief The final texture of the board, with zoom and pan applied */
  sf::Texture texture;

  /** @brief Pointer to the Board which supplies the data */
  const Board *board;

  /** @brief Holds information about the FOV, i.e. which part of map is drawn on screen */
  sf::IntRect mapTextureRect;

  /** @brief Upper and lower zoom bounds */
  float minScale = 0.25f, maxScale = 1.0f;

  /** @brief Current zoom level, limited by the zoom bounds */
  float viewScale = 1.0f;

  /** @brief Pan requested by the user, adjusted each frame to fit inside the map */
  sf::Vector2i desiredPan = {0, 0};
};