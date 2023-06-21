#pragma once
#include <string>

#include "../TooDeeEngine/Scene/SceneBase.hpp"
#include "../TooDeeEngine/Util/Log.hpp"
#include "GameScene.hpp"

/**
 * @brief The scene shown at the game's startup.
 *
 */
class MainMenuScene : public SceneBase {
 public:
  MainMenuScene(SceneManager* sm, SceneContext* sceneContext);
  virtual void event_load_scene() override;
  virtual void event_unload_scene() override {
    ipTb->remove_self();
    portTb->remove_self();
    joinBtn->remove_self();
  }

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(bgShape);
    target.draw(*sceneContext->uiSystem, states);
  };

 private:
  void event_join_clicked();

  sf::RectangleShape bgShape;
  std::shared_ptr<WTextbox> ipTb;
  std::shared_ptr<WTextbox> portTb;
  std::shared_ptr<WButton> joinBtn;
  std::string ip = "127.0.0.1";
  std::string port = "12345";
};
