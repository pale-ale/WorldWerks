#pragma once

#include "../Tabletop/Board.hpp"
#include "../TooDeeEngine/UI/UIElement.hpp"
#include "../TooDeeEngine/UI/UISystem.hpp"
#include "SemiPrimitives/Grid.hpp"
#include "TokenUI.hpp"
#include "Toolbar/WDistanceTool.hpp"
#include "Toolbar/WToolbar.hpp"
#include "UIComponents/WToken.hpp"

class WBoard : public UIElement {  
 public:
  WBoard(UISystem *uiSystem, std::weak_ptr<UIElement> parent, Board* board, const sf::Vector2i &size);
  void display_token(Token *token);
  void update_background();

  /** @brief WButton used to save the map */
  std::shared_ptr<WButton> saveButton;

 protected:
  void post_init() override;
  bool event_key_down(const sf::Event &keyEvent) override;
  void update_tokens();
  void update_board_view();
  void set_scale(int newScale);
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

  /** @brief Keep references to the token widgets */
  std::vector<TokenUIComponent *> tokens;

  /** @brief The image used as the background prior to scaling */
  sf::Image boardImage;

  /** @brief The final texture of the board, with zoom and pan applied */
  sf::Texture boardTexture;

  /** @brief The sprite using the above texure */
  sf::Sprite boardSprite;

  /** @brief Pointer to the Board which supplies the data */
  Board *board;

  /** @brief Upper and lower scale bounds */
  int maxScale = 4, minScale = 1;

  /** @brief Current scale, limited by the scale bounds */
  int viewScale = 1.;

  /** @brief Pan requested by the user, adjusted each frame to fit inside the map */
  sf::Vector2i desiredPan = {0, 0};

  /** @brief Should grid lines be drawn? **/
  bool bDrawGrid = false;

  /** @brief The grid lines **/
  GridSemiPrimitive grid;
};