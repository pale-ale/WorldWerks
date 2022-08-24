#pragma once
#include <stdio.h>

#include "../MapParser/MapParser.hpp"
#include "../Tabletop/Board.hpp"
#include "../UI/WBoard.hpp"
#include "SceneBase.hpp"

/**
 * @brief The scene which contains the main game view.
 *
 */
class GameScene : public SceneBase {
 public:
  GameScene(SceneContext* sceneContext) : SceneBase(sceneContext) {}
  virtual void event_load_scene() override {
    tmx::MapParser mapParser;
    mapParser.load_file("/home/alba/WorldWerksMap/ExampleMap.tmx");
    auto res = sceneContext->resolution;
    auto uiSystem = sceneContext->uiSystem;
    if (!uiSystem) {
      printf("[GameScene]: SceneContext missing a UISystem.\n");
      return;
    } else {
      uiSystem->create_widget<WBoard>(uiSystem->get_root(), new Board(mapParser.map),
                                      SpriteLoader::getInstance(),
                                      sf::Vector2i{res.x, res.y});
      printf("[GameScene]: Loaded.\n");
    }
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*sceneContext->uiSystem, states);
  };
};
