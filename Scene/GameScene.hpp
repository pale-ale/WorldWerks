#pragma once
#include <chrono>
#include <string>
#include <thread>

#include "../MapParser/MapParser.hpp"
#include "../Net/ClientEndpoint.hpp"
#include "../Net/Proto/Protocol.hpp"
#include "../Storage/LiveStorage.hpp"
#include "../Tabletop/Board.hpp"
#include "../UI/WBoard.hpp"
#include "../Util/Log.hpp"
#include "SceneBase.hpp"

/**
 * @brief The scene which contains the main game view.
 *
 */
class GameScene : public SceneBase {
 public:
  GameScene(SceneManager* sm, SceneContext* sceneContext)
      : SceneBase(sm, sceneContext), mp{sceneContext->mapParser} {}
  virtual void event_load_scene() override {
    if (!sceneContext->mapParser) {
      LOGERR("GameScene", "SceneContext missing a MapParser.");
      return;
    }
    if (!sceneContext->uiSystem) {
      LOGERR("GameScene", "SceneContext missing a UISystem.");
      return;
    }
    auto client = sceneContext->clientEp;
    if (!client) {
      LOGERR("GameScene", "SceneContext missing a ClientEndpoint.");
      return;
    }
    client->callbacks[wwnet::EMessageType::RES_MAP].push_back(
        [this](const std::string& data) { this->load_map(data); });
    client->request_map();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    client->digest_incoming();
  }

  void preload_map(const std::string& mapData) {
    LOGINF("GameScene", "Pre-Loading map...");
    auto str = LiveStorage::create_entry("Map");
    if (!str) {
      LOGERR("GameScene", "Map storage key already exists.");
      return;
    }
    *str = mapData;
    LOGINF("GameScene", "Added map entry to storage.");
    mp->load_text(mapData);
    auto missingTilesetPaths = mp->get_missing_resource_paths();
    LOGINF("GameScene", "Requesting missing tilesets...");
    auto client = sceneContext->clientEp;
    auto cb = [this](const std::string& tsxData) { receive_tsx_data(tsxData); };
    client->callbacks[wwnet::EMessageType::RES_TSX].push_back(cb);
    for (auto&& missingTilesetPath : missingTilesetPaths) {
      LiveStorage::create_entry(missingTilesetPath);
      client->request_tileset(missingTilesetPath);
    }
  }

  void receive_tsx_data(const std::string& tsxData) {
    sceneContext->mapParser->map->update_tileset_data(tsxData);
  }

  void load_map(const std::string& mapData) {
    LOGINF("GameScene", "Loading map...");
    preload_map(mapData);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    sceneContext->clientEp->digest_incoming();
    sceneContext->mapParser = mp;
    auto res = sceneContext->resolution;
    auto uiSystem = sceneContext->uiSystem;
    auto boardWidget = uiSystem->create_widget<WBoard>(
        uiSystem->get_root(), new Board(mp->map), SpriteLoader::getInstance(),
        sf::Vector2i{res.x, res.y});
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*sceneContext->uiSystem, states);
  };

 private:
  tmx::MapParser* mp;
};
