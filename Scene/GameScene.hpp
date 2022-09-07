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
  GameScene(SceneManager *sm, SceneContext* sceneContext) : SceneBase(sm, sceneContext) {}
  virtual void event_load_scene() override {
    auto load_path = "/home/alba/WorldWerksMap/Save_ExampleMap.tmx";
    auto save_path = "/home/alba/WorldWerksMap/Save_ExampleMap.tmx";
    mp = new tmx::MapParser();
    mp->load_file(load_path);
    auto res = sceneContext->resolution;
    auto uiSystem = sceneContext->uiSystem;
    if (!uiSystem) {
      printf("[GameScene]: SceneContext missing a UISystem.\n");
      return;
    }
    auto boardWidget = uiSystem->create_widget<WBoard>(uiSystem->get_root(), new Board(mp->map),
                                    SpriteLoader::getInstance(),
                                    sf::Vector2i{res.x, res.y});
    printf("[GameScene]: Loaded.\n");
    boardWidget->saveButton->buttonClickCallback = [this, save_path]{mp->save_file(save_path);};
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*sceneContext->uiSystem, states);
  };

 private:
 tmx::MapParser *mp;
};
