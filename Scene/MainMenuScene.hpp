#pragma once
#include <stdio.h>

#include "SceneBase.hpp"

/**
 * @brief The scene shown at the game's startup.
 *
 */
class MainMenuScene : public SceneBase {
 public:
  MainMenuScene(SceneContext* sceneContext) : SceneBase(sceneContext) {}
  virtual void event_load_scene() override { printf("[MainMenuScene]: Loaded.\n"); }
};
