#pragma once

#include "../Tabletop/Board.hpp"
#include "TokenUI.hpp"
#include "UIElement.hpp"
#include "UISystem.hpp"
#include "WToken.hpp"
#include "Toolbar/WToolbar.hpp"
#include "Toolbar/WDistanceTool.hpp"
#include "SemiPrimitives/Grid.hpp"

class WBoard : public UIElement {
  friend UISystem;

 public:
  void display_token(Token *token);
  void update_background();
  
  /** @brief WButton used to save the map */
  std::shared_ptr<WButton> saveButton;

 protected:
  WBoard(UISystem *uiSystem, std::shared_ptr<UIElement> parent, Board *board,
         SpriteLoader *spriteloader, const sf::Vector2i &size);
  void post_init() override;
  bool event_key_down(const sf::Event &keyEvent) override;
  void update_tokens();
  void update_board_view();
  void set_scale(float newScale);
  void change_pan(int newX, int newY);
  void set_draw_grid(bool draw);
  string get_bg_tileset_key();
  virtual bool event_clicked() override;
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  /** @brief UIElement used to display the Token's information */
  std::shared_ptr<TokenUI> tokenUI;

  /** @brief Displays a set of tools available on the map */
  std::shared_ptr<WToolbar> toolbar;

  /** @brief The final texture of the board, with zoom and pan applied */
  sf::Texture texture;

  /** @brief Pointer to the Board which supplies the data */
  Board *board;

  /** @brief Holds information about the FOV, i.e. which part of map is drawn on screen */
  sf::IntRect mapTextureRect;

  /** @brief Upper and lower zoom bounds */
  float minScale = 0.25f, maxScale = 1.0f;

  /** @brief Current zoom level, limited by the zoom bounds */
  float viewScale = 1.0f;

  /** @brief Pan requested by the user, adjusted each frame to fit inside the map */
  sf::Vector2i desiredPan = {0, 0};

  /** @brief Should grid lines be drawn? **/
  bool bDrawGrid = false;
  
  /** @brief The grid lines **/
  GridSemiPrimitive grid;
};