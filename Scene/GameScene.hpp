#pragma once
#include <stdio.h>

#include <chrono>
#include <thread>

#include "../MapParser/MapParser.hpp"
#include "../Net/ClientEndpoint.hpp"
#include "../Net/Proto/Protocol.hpp"
#include "../Tabletop/Board.hpp"
#include "../UI/WBoard.hpp"
#include "SceneBase.hpp"

/**
 * @brief The scene which contains the main game view.
 *
 */
class GameScene : public SceneBase {
 public:
  GameScene(SceneManager* sm, SceneContext* sceneContext) : SceneBase(sm, sceneContext) {}
  virtual void event_load_scene() override {
    auto client = sceneContext->clientEp;
    client->callbacks[wwnet::EMessageType::RES_MAP].push_back(
        [this](std::string data) { load_map(data); });
    client->request_map();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    client->digest_incoming();
  }

  void load_map(std::string mapData) {
    mp = new tmx::MapParser();
    mp->load_text(mapData, "/home/alba/WorldWerksMap/");
    sceneContext->mapParser = mp;
    auto res = sceneContext->resolution;
    auto uiSystem = sceneContext->uiSystem;
    if (!uiSystem) {
      printf("[GameScene]: SceneContext missing a UISystem.\n");
      return;
    }
    auto boardWidget = uiSystem->create_widget<WBoard>(
        uiSystem->get_root(), new Board(mp->map), SpriteLoader::getInstance(),
        sf::Vector2i{res.x, res.y});
    printf("[GameScene]: Loaded.\n");
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*sceneContext->uiSystem, states);
  };

 private:
  tmx::MapParser* mp;
};
