#include "WBoard.hpp"

#include "TokenUI.hpp"

WBoard::WBoard(UISystem* uiSystem, std::shared_ptr<UIElement> parent, Board* board,
               SpriteLoader* spriteloader, const sf::Vector2i& size)
    : UIElement(uiSystem, parent, "WBoard", size), board{board} {
  LiveStorage::storage["Background.tsx"].updateListeners.push_back([this](auto s) {
    LiveStorage::storage[get_bg_tileset_key()].updateListeners.push_back(
        [this](auto s) { update_background(); });
  });
}

string WBoard::get_bg_tileset_key() {
  for (auto&& tileset : board->get_map()->tilesets) {
    if (tileset->name == "Background") {
      return tileset->relativeImagePath;
    }
  }
  LOGWRN("Wboard", "Cannot find background tileset.");
  return "";
}

/**
 * @brief Reload the background from the map tileset.
 */
void WBoard::update_background() {
  sf::Texture boardTexture;
  LOGINF("WBoard", "Loading background...");
  for (auto&& tileset : board->get_map()->tilesets) {
    if (tileset->name == "Background") {
      std::string imageKey = tileset->relativeImagePath;
      std::string imageData;
      if (LiveStorage::get_state(imageKey) == EStorageElementState::REMOTE_READY) {
        LiveStorage::retrieve(imageKey, imageData);
        const char* rawData = imageData.c_str();
        boardTexture.loadFromMemory(rawData, imageData.size() * sizeof(char));
        LOGINF("WBoard", "Found background.");
        break;
      }
    }
  }

  sf::RenderTexture rtex;
  rtex.create(size.x, size.y);
  rtex.clear(sf::Color(60, 50, 25));
  rtex.draw(sf::Sprite(boardTexture));
  rtex.display();
  texture = rtex.getTexture();
  sprite.setTexture(texture);
  set_scale(viewScale);
}

void WBoard::post_init() {
  update_tokens();
  tokenUI = uiSystem->create_widget<TokenUI>(shared_from_this(),
                                             sf::Vector2i{size.x / 4, size.y});
  tokenUI->update_position({size.x - tokenUI->size.x, 0});
  sf::Vector2i saveButtonSize{100, 20};
  saveButton = uiSystem->create_widget<WButton>(shared_from_this(), saveButtonSize);
  saveButton->update_position(
      {(size.x - saveButtonSize.x) / 2, size.y - saveButtonSize.y});
  saveButton->buttonClickCallback = [this] {
    LOGWRN("WBoard", "Save button not bound.");
  };
  toolbar = uiSystem->create_widget<WToolbar>(shared_from_this());
  auto gridBtn = uiSystem->create_widget<WButton>(toolbar, sf::Vector2i(30, 30));
  toolbar->set_tools(std::vector<std::shared_ptr<UIElement>>{
      uiSystem->create_widget<WDistanceTool>(toolbar),
      gridBtn
  });
  gridBtn->buttonClickCallback = [this](){this->set_draw_grid(!bDrawGrid);};
}

/**
 * @brief Re-create the WTokens from the data in board->tokens.
 */
void WBoard::update_tokens() {
  children.clear();
  for (auto&& token : board->tokens) {
    auto positionSetter = [&token](const sf::Vector2i& v) { token.set_position(v); };
    Binding<sf::Vector2i> binding{{}, positionSetter};
    auto tokenButton =
        uiSystem->create_widget<WToken>(shared_from_this(), token, binding);
    tokenButton->buttonClickCallback = [this, &token]() { display_token(&token); };
  }
}

/**
 * @brief When clicked, stop displaying the selected token.
 *
 * @return true, because this will always handle the click event
 */
bool WBoard::event_clicked() {
  LOGWRN("WBoard", "Hiding TokenUI.");
  display_token(nullptr);
  return true;
}

/**
 * @brief Set the displayed token.
 *
 * @param token The new token to display. Pass a nullptr to not display a token.
 */
void WBoard::display_token(Token* token) {
  LOGWRN("WBoard", fmt::format("Displaying token '{}'.",
                               token ? token->get_title().c_str() : "null"));
  tokenUI->set_token(token);
}

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
      std::clamp(-zoomPanOffset.x, desiredPan.x, boardW - scaledW - zoomPanOffset.x);
  desiredPan.y =
      std::clamp(-zoomPanOffset.y, desiredPan.y, boardH - scaledH - zoomPanOffset.y);
  // Apply the pan to the texture rect.
  mapTextureRect.left = desiredPan.x + zoomPanOffset.x;
  mapTextureRect.top = desiredPan.y + zoomPanOffset.y;
  // Apply the new texturerect to the sprite.
  sprite.setTextureRect(mapTextureRect);
  // Scale up the sprite to fill the screen instead of just being smaller
  sprite.setScale(1 / viewScale, 1 / viewScale);

  // Draw the grid lines
  int squareSize = 25 * (1 / viewScale);
  int lineCountX = (boardW / squareSize) + 2; // +2 for panning
  int lineCountY = (boardH / squareSize) + 2;
  grid.setLineCount({lineCountX+2, lineCountY+2});
  grid.setLineSpace({squareSize, squareSize});
  auto scaledPan = desiredPan;
  int gridOffsetX = (scaledPan.x + zoomPanOffset.x) % squareSize;
  int gridOffsetY = (scaledPan.y + zoomPanOffset.y) % squareSize;
  grid.offset = -sf::Vector2f(gridOffsetX, gridOffsetY) / viewScale;
}

bool WBoard::event_key_down(const sf::Event& keyEvent) {
  if (!keyEvent.key.control) {
    return false;
  }
  int panStep = 1;
  switch (keyEvent.key.code) {
    case sf::Keyboard::Hyphen:
      set_scale(viewScale * 2);
      break;

    case sf::Keyboard::RBracket:
      set_scale(viewScale / 2);
      break;

    case sf::Keyboard::Up:
      change_pan(0, -panStep / viewScale);
      break;

    case sf::Keyboard::Down:
      change_pan(0, panStep / viewScale);
      break;

    case sf::Keyboard::Left:
      change_pan(-panStep / viewScale, 0);
      break;

    case sf::Keyboard::Right:
      change_pan(panStep / viewScale, 0);
      break;

    default:
      return false;
  }
  return true;
}

/**
 * @brief Use this to pan the map up/down/left/right.
 *
 * @param dx The horizontal translation, measured from the left.
 * @param dy The vertical translation, measured from the top.
 */
void WBoard::change_pan(int dx, int dy) {
  desiredPan.x += dx;
  desiredPan.y += dy;
  update_board_view();
}

/**
 * @brief Enable/Disable the grid
 * 
 * @param draw If true, enables the grid. Otherwise disables it
 */
void WBoard::set_draw_grid(bool draw) {
  bDrawGrid = draw;
  update_board_view();
}

/**
 * @brief Set the zoom of the map. 1 = Zoomed out. Use powers of 2, e.g. 0.25.
 *
 * @param newScale The new scale of the map between 0 and 1
 */
void WBoard::set_scale(float newScale) {
  viewScale = std::clamp(minScale, newScale, maxScale);
  LOGDBG("WBoard", fmt::format("Scale is now {}.", viewScale));
  update_board_view();
}

void WBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  UIElement::draw(target, states);
  if (bDrawGrid){
    target.draw(grid, states);
  }
}
