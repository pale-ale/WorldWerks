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
#include "../Storage/LiveStorage.hpp"
#include <string>

/**
 * @brief The scene which contains the main game view.
 *
 */
class GameScene : public SceneBase {
 public:
  GameScene(SceneManager* sm, SceneContext* sceneContext) : SceneBase(sm, sceneContext), mp{sceneContext->mapParser} {}
  virtual void event_load_scene() override {
    if (!sceneContext->mapParser) {
      printf("[GameScene]: SceneContext missing a MapParser.\n");
      return;
    }
    if (!sceneContext->uiSystem) {
      printf("[GameScene]: SceneContext missing a UISystem.\n");
      return;
    }
    auto client = sceneContext->clientEp;
    if (!client) {
      printf("[GameScene]: SceneContext missing a ClientEndpoint.\n");
      return;
    }
    client->callbacks[wwnet::EMessageType::RES_MAP].push_back(
        [this](const std::string& data) { this->load_map(data); });
    client->request_map();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    client->digest_incoming();
  }

  void preload_map(const std::string &mapData){
    printf("[GameScene]: Pre-Loading map...\n");
    auto str = LiveStorage::create_entry("Map");
    if (!str){
      printf("[GameScene]: Map storage key already exists.\n");
      return;
    }
    *str = mapData;
    printf("[GameScene]: Added map entry to storage.\n");
    mp->load_text(mapData);
    auto missingTilesetPaths = mp->get_missing_resource_paths();
    printf("[GameScene]: Requesting missing tilesets...\n");
    auto client = sceneContext->clientEp;
    auto cb = [this](const std::string& tsxData){receive_tsx_data(tsxData);};
    client->callbacks[wwnet::EMessageType::RES_TSX].push_back(cb);
    for (auto&& missingTilesetPath : missingTilesetPaths){
      LiveStorage::create_entry(missingTilesetPath);
      client->request_tileset(missingTilesetPath);
    }
  }

  void receive_tsx_data(const std::string& tsxData){
    sceneContext->mapParser->map->update_tileset_data(tsxData);
  }

  void load_map(const std::string &mapData) {
    preload_map(mapData);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("[GameScene]: Loading map...\n");
    sceneContext->clientEp->digest_incoming();
    sceneContext->mapParser = mp;
    auto res = sceneContext->resolution;
    auto uiSystem = sceneContext->uiSystem;
    auto boardWidget = uiSystem->create_widget<WBoard>(
        uiSystem->get_root(), new Board(mp->map), SpriteLoader::getInstance(),
        sf::Vector2i{res.x, res.y});
    printf("[GameScene]: Loaded map.\n");
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*sceneContext->uiSystem, states);
  };

 private:
  tmx::MapParser* mp;
};
