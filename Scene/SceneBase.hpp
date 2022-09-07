#pragma once
#include <SFML/Graphics.hpp>

#include "SceneContext.hpp"

class SceneManager;

/**
 * @brief Inherit from this class to allow it to be managed by the SceneManager.
 *
 */
class SceneBase : public sf::Drawable {
 public:
  SceneBase(SceneManager* sceneManager, SceneContext *sceneContext) : sceneManager{sceneManager}, sceneContext{sceneContext} {}
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

  /** @brief Needed to change scenes again */
  SceneManager *sceneManager = nullptr;
};
