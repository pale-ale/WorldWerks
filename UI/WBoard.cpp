#include "WBoard.hpp"

#include "TokenUI.hpp"

WBoard::WBoard(UISystem* uiSystem, std::weak_ptr<UIElement> parent, Board* board,
               const sf::Vector2i& size)
    : UIElement(uiSystem, parent, "WBoard", size), board{board} {
  name = "WBoard";
  LiveStorage::storage["Background.tsx"].updateListeners.push_back([this](auto s) {
    LiveStorage::storage[get_bg_tileset_key()].updateListeners.push_back(
        [this](auto s) { update_background(); });
  });
  sprite.setColor({25, 25, 50});
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
  LOGINF("WBoard", "Loading background...");
  for (auto&& tileset : board->get_map()->tilesets) {
    if (tileset->name == "Background") {
      std::string imageKey = tileset->relativeImagePath;
      std::string imageData;
      if (LiveStorage::get_state(imageKey) == EStorageElementState::REMOTE_READY) {
        LiveStorage::retrieve(imageKey, imageData);
        const char* rawData = imageData.c_str();
        boardImage.loadFromMemory(rawData, imageData.size() * sizeof(char));
        LOGINF("WBoard", "Found background.");
        break;
      }
    }
  }
  boardTexture.loadFromImage(boardImage);
  boardSprite.setTexture(boardTexture, true);
  boardSprite.setOrigin(boardTexture.getSize().x / 2, boardTexture.getSize().y / 2);
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
  gridBtn->set_text("Grid");
  toolbar->set_tools(std::vector<std::shared_ptr<UIElement>>{
      uiSystem->create_widget<WDistanceTool>(toolbar), gridBtn});
  gridBtn->buttonClickCallback = [this]() { this->set_draw_grid(!bDrawGrid); };
}

/**
 * @brief Re-create the WTokens from the data in board->tokens.
 */
void WBoard::update_tokens() {
  children.clear();
  for (auto token : board->tokens) {
    auto positionSetter = [&token](const sf::Vector2i& v) { token->set_position(v); };
    Binding<sf::Vector2i> binding{{}, positionSetter};
    auto tokenWidget =
        uiSystem->create_widget<TokenUIComponent>(shared_from_this(), token, binding);
    token->add_callback([&token, tokenWidget](auto* _) {
      tokenWidget->update_position(token->get_position());
    });
    tokenWidget->onClickCallback = [this, token]() { display_token(token); };
    tokens.push_back(tokenWidget.get());
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
  std::for_each(tokens.begin(), tokens.end(), [this](TokenUIComponent* token) {
    auto t = this->boardSprite.getTransform();
    token->update_position(
        sf::Vector2i(t.transformPoint((sf::Vector2f(token->token->get_position())))));
  });
}

bool WBoard::event_key_down(const sf::Event& keyEvent) {
  if (!keyEvent.key.control) {
    return false;
  }
  int panStep = 3;
  switch (keyEvent.key.code) {
    case sf::Keyboard::Hyphen:
      set_scale(viewScale / 2);
      break;

    case sf::Keyboard::RBracket:
      set_scale(viewScale * 2);
      break;

    case sf::Keyboard::Up:
      change_pan(0, panStep * viewScale);
      break;

    case sf::Keyboard::Down:
      change_pan(0, -panStep * viewScale);
      break;

    case sf::Keyboard::Left:
      change_pan(panStep * viewScale, 0);
      break;

    case sf::Keyboard::Right:
      change_pan(-panStep * viewScale, 0);
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
  sf::Vector2i mapSizeScaled{(int)boardSprite.getGlobalBounds().width,
                             (int)boardSprite.getGlobalBounds().height};
  sf::Vector2i mapOverhang{mapSizeScaled - size};
  desiredPan.x = std::clamp(desiredPan.x, std::min(-mapOverhang.x / 2, 0),
                            std::max(mapOverhang.x / 2, 0));
  desiredPan.y = std::clamp(desiredPan.y, std::min(-mapOverhang.y / 2, 0),
                            std::max(mapOverhang.y / 2, 0));
  boardSprite.setPosition(desiredPan.x + size.x / 2, desiredPan.y + size.y / 2);

  // Update the grid lines
  int squareSize = 25 * viewScale;
  int lineCountX = (mapSizeScaled.x / squareSize) + 2;  // +2 for panning
  int lineCountY = (mapSizeScaled.y / squareSize) + 2;
  grid.setLineCount({lineCountX + 2, lineCountY + 2});
  grid.setLineSpace({squareSize, squareSize});
  auto scaledPan = desiredPan;
  int gridOffsetX = (-scaledPan.x / viewScale + mapOverhang.x / 2) % squareSize;
  int gridOffsetY = (-scaledPan.y / viewScale + mapOverhang.y / 2) % squareSize;
  auto offset = -sf::Vector2f(gridOffsetX, gridOffsetY) * (float)viewScale;
  grid.offset = offset;

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
void WBoard::set_scale(int newScale) {
  viewScale = std::clamp(newScale, minScale, maxScale);
  boardSprite.setScale(viewScale, viewScale);
  change_pan(0, 0);
  LOGDBG("WBoard", fmt::format("Scale is now {}.", viewScale));
}

void WBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  UIElement::draw(target, states);
  target.draw(boardSprite, states);
  if (bDrawGrid) {
    target.draw(grid, states);
  }
  auto transform = boardSprite.getTransform();
  for (auto&& wtoken : tokens) {
    target.draw(*wtoken, states);
  }
  target.draw(*tokenUI, states);
  target.draw(*toolbar, states);
  target.draw(*saveButton, states);
}
