#include "WBoard.hpp"

#include "TokenUI.hpp"

constexpr float clamp(float a, float x, float b) { return std::min(std::max(a, x), b); }

WBoard::WBoard(UISystem* uiSystem, std::shared_ptr<UIElement> parent, Board* board,
               SpriteLoader* spriteloader, const sf::Vector2i& size)
    : UIElement(uiSystem, parent, size), board{board} {
  sf::Texture boardTexture;
  for (auto&& tileset : board->get_map()->tilesets) {
    if (tileset->name == "Background") {
      std::string path = tileset->imagePath;
      boardTexture.loadFromFile(path);
    }
  }

  sf::RenderTexture rtex;
  rtex.create(size.x, size.y);
  rtex.clear(sf::Color(60, 50, 25));
  rtex.draw(sf::Sprite(boardTexture));
  rtex.display();
  texture = rtex.getTexture();
  sprite.setTexture(texture);
  set_scale(1.0f);
}

void WBoard::post_init() {
  update_tokens();
  tokenUI = uiSystem->create_widget<TokenUI>(shared_from_this(),
                                             sf::Vector2i{size.x / 4, size.y});
  tokenUI->update_position({size.x - tokenUI->size.x, 0});
  sf::Vector2i saveButtonSize{100,20};
  saveButton = uiSystem->create_widget<WButton>(shared_from_this(), saveButtonSize);
  saveButton->update_position({(size.x - saveButtonSize.x) / 2, size.y - saveButtonSize.y});
  saveButton->buttonClickCallback = [this]{printf("[WBoard]: Save button not bound.\n");};
}

/**
 * @brief Re-create the WTokens from the data in board->tokens.
 */
void WBoard::update_tokens() {
  children.clear();
  for (auto&& token : board->tokens) {
    auto tokenButton = uiSystem->create_widget<WToken>(shared_from_this(), token);
    tokenButton->buttonClickCallback = [this, &token]() {display_token(&token); };
  }
}

/**
 * @brief When clicked, stop displaying the selected token.
 *
 * @return true, because this will always handle the click event
 */
bool WBoard::event_clicked() {
  display_token(nullptr);
  return true;
}

/**
 * @brief Set the displayed token. Set to nullptr to not display a token.
 */
void WBoard::display_token(Token* token) { tokenUI->set_token(token); }

/**
 * @brief Redraw the board with a new texture rect with pan and zoom applied.
 */
void WBoard::update_board_view() {
  int boardW = texture.getSize().x, boardH = texture.getSize().y;
  int scaledW = boardW * viewScale, scaledH = boardH * viewScale;
  int diffW = boardW - scaledW, diffH = boardH - scaledH;

  // This makes the screen zoom into the center rather than the top left.
  sf::Vector2i zoomPanOffset = {diffW / 2, diffH / 2};
  // Set the texture rect's new side lengths.
  mapTextureRect.width = scaledW;
  mapTextureRect.height = scaledH;
  // Adjust the desired pan to be compatible with the offset.
  desiredPan.x =
      clamp(-zoomPanOffset.x, desiredPan.x, boardW - scaledW - zoomPanOffset.x);
  desiredPan.y =
      clamp(-zoomPanOffset.y, desiredPan.y, boardH - scaledH - zoomPanOffset.y);
  // Apply the pan to the texture rect.
  mapTextureRect.left = desiredPan.x + zoomPanOffset.x;
  mapTextureRect.top = desiredPan.y + zoomPanOffset.y;
  // Apply the new texturerect to the sprite.
  sprite.setTextureRect(mapTextureRect);
  // Scale up the sprite to fill the screen instead of just being smaller
  sprite.setScale(1 / viewScale, 1 / viewScale);
}

void WBoard::event_key_down(const sf::Event& keyEvent) {
  if (!keyEvent.key.control) {
    return;
  }
  switch (keyEvent.key.code) {
    case sf::Keyboard::Hyphen:
      set_scale(viewScale * 2);
      break;

    case sf::Keyboard::RBracket:
      set_scale(viewScale / 2);
      break;

    case sf::Keyboard::Up:
      change_pan(0, -10 / viewScale);
      break;

    case sf::Keyboard::Down:
      change_pan(0, 10 / viewScale);
      break;

    case sf::Keyboard::Left:
      change_pan(-10 / viewScale, 0);
      break;

    case sf::Keyboard::Right:
      change_pan(10 / viewScale, 0);
      break;

    default:
      break;
  }
}

/**
 * @brief Use this to pan the map up/down/left/right.
 */
void WBoard::change_pan(int dx, int dy) {
  desiredPan.x += dx;
  desiredPan.y += dy;
  update_board_view();
}

/**
 * @brief Set the zoom of the map. 1 = Zoomed out. Use powers of 2, e.g. 0.25.
 */
void WBoard::set_scale(float newScale) {
  viewScale = clamp(minScale, newScale, maxScale);
  printf("WBoard.cpp: Scale is now %f.\n", viewScale);
  update_board_view();
}
