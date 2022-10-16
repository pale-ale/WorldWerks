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
    client->callbacks[wwnet::EMessageType::RES_RES].push_back(
        [](const std::string& data) {
          LOGINF("GameScene", "Inserting received LiveData resource...");
          LiveDataCapsule ldc;
          ldc.from_msg(data);
          LiveStorage::insert_data_capsule(ldc);
        });
    LiveStorage::missingResourceHandler = [client](auto s) {
      LOGINF("GameScene", fmt::format("Requesting resource '{}'.", s));
      client->request_resource(s);
    };
    client->request_map();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    client->digest_incoming();
  }

  virtual void event_unload_scene() { boardWidget->remove_self(); }

  void preload_map(const std::string& mapData) {
    LOGINF("GameScene", "Pre-Loading map...");
    auto str = LiveStorage::create_entry("Map");
    if (!str) {
      LOGERR("GameScene", "Map storage key already exists.");
      return;
    }
    *str = mapData;
    LOGINF("GameScene", "Loading map data...");
    mp->load_text(mapData);
    LOGINF("GameScene", "Loaded map data.");
  }

  void load_map(const std::string& mapData) {
    LOGINF("GameScene", "Loading map...");
    preload_map(mapData);
    sceneContext->mapParser = mp;
    auto board = new Board(mp->map);
    auto res = sceneContext->resolution;
    auto uiSystem = sceneContext->uiSystem;
    boardWidget = uiSystem->create_widget<WBoard>(uiSystem->get_root(), board,
                                                  SpriteLoader::getInstance(),
                                                  sf::Vector2i{res.x, res.y});
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*sceneContext->uiSystem, states);
  };

 private:
  tmx::MapParser* mp = nullptr;
  std::shared_ptr<WBoard> boardWidget = nullptr;
};
