#include <stdio.h>

#include <string>

#include "../Net/ClientEndpoint.hpp"
#include "../UI/WButton.hpp"
#include "../UI/WTextbox.hpp"
#include "GameScene.hpp"
#include "SceneBase.hpp"
#include "SceneManager.hpp"

MainMenuScene::MainMenuScene(SceneManager* sm, SceneContext* sceneContext)
    : SceneBase(sm, sceneContext) {
  bgShape = sf::RectangleShape(sceneContext->resolution);
  bgShape.setFillColor(sf::Color(25, 25, 50));
}

void MainMenuScene::event_load_scene() {
  auto uiSys = sceneContext->uiSystem;
  auto root = uiSys->get_root();
  sf::Vector2i ipSize{100, 25};
  sf::Vector2i portSize{50, 25};
  sf::Vector2i joinSize{100, 25};
  int y = 10;
  int startX = (sceneContext->resolution.x - (ipSize.x + portSize.x + 2)) / 2;
  sf::Vector2i ipPos(startX, y);
  sf::Vector2i portPos(startX + ipSize.x + 2, y);
  sf::Vector2i joinPos((sceneContext->resolution.x - joinSize.x) / 2, y + ipSize.y + 2);
  Binding<std::string> ipBinding{{[this]() { return this->ip; }},
                                 {[this](std::string text) { this->ip = text; }}};
  Binding<std::string> portBinding{{[this]() { return this->port; }},
                                   {[this](std::string text) { this->port = text; }}};

  ipTb = uiSys->create_widget<WTextbox>(root, ipBinding, ipSize, ipPos);
  ipTb = uiSys->create_widget<WTextbox>(root, portBinding, portSize, portPos);
  joinBtn = uiSys->create_widget<WButton>(root, joinSize, joinPos);
  joinBtn->buttonClickCallback = [this]() { this->event_join_clicked(); };
  printf("[MainMenuScene]: Loaded.\n");
}

void MainMenuScene::event_join_clicked() {
  auto ep = new ClientEndpoint();
  ep->start_connecting(ip.c_str(), std::stoi(port));
  sceneContext->clientEp = ep;
  if (ep->is_up() && ep->is_connected_and_buffer_empty()) {
    printf("[MainMenuScene]: Connected successfully.\n");
    sceneManager->load_scene<GameScene>();
  }
}
