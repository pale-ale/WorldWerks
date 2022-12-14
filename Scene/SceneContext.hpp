#pragma once

namespace tmx {
class MapParser;
};
class ClientEndpoint;

/**
 * @brief Contains data that can be useful for scenes.
 *
 */
struct SceneContext {
 private:
  struct vec {
    int x = 0;
    int y = 0;
    operator sf::Vector2f() { return {(float)x, (float)y}; }
  };

 public:
  /** @brief The amount of pixels in x/y direction of the viewport */
  vec resolution;
  /** @brief The MapParser contains the data of a loaded map */
  tmx::MapParser* mapParser = nullptr;
  /** @brief Needed to create new UIElements */
  UISystem* uiSystem = nullptr;
  /** @brief Allows connecting/disconnecting */
  ClientEndpoint* clientEp = nullptr;
};