#pragma once

class MapParser;

/**
 * @brief Contains data that can be useful for scenes.
 *
 */
struct SceneContext {
 private:
  struct vec {
    int x;
    int y;
  };

 public:
  /** @brief The amount of pixels in x/y direction of the viewport */
  vec resolution;
  /** @brief The MapParser contains the data of a loaded map */
  MapParser* mapParser = nullptr;
  /** @brief Needed to create new UIElements */
  UISystem* uiSystem = nullptr;
};