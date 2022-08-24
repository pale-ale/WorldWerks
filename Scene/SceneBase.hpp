#pragma once
#include <SFML/Graphics.hpp>

#include "SceneContext.hpp"

/**
 * @brief Inherit from this class to allow it to be managed by the SceneManager.
 *
 */
class SceneBase : public sf::Drawable {
 public:
  SceneBase(SceneContext *sceneContext) : sceneContext{sceneContext} {}
  /**
   * @brief Called when the scene was loaded.
   */
  virtual void event_load_scene() {}

  /**
   * @brief Called when the scene is about to be unloaded.
   */
  virtual void event_unload_scene() {}

 protected:
  /** @brief Contains the UISystem, scene size, and other useful tidbits. */
  SceneContext *sceneContext = nullptr;
};
