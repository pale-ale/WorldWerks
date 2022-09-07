#pragma once
#include "../MapParser/MapParser.hpp"
#include "MainMenuScene.hpp"
#include "SceneBase.hpp"

/**
 * @brief The Scenemanager loads/unloads scenes.
 */
class SceneManager : public sf::Drawable {
 public:
  /**
   * @brief Create a new scene that will be loaded.
   *
   * @tparam T The type of the new scene. Has to inherit from SceneBase
   */
  template <class T>
  void load_scene() {
    if (currentScene) {
      currentScene->event_unload_scene();
    }
    currentScene = new T(this, &sceneContext);
    currentScene->event_load_scene();
  }

  /** @brief Allows other scenes to access relevant info, like scene size */
  SceneContext sceneContext;

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    if (currentScene) {
      target.draw(*currentScene, states);
    }
  };

 private:
  /** @brief The currently loaded scene */
  SceneBase* currentScene = nullptr;
};
