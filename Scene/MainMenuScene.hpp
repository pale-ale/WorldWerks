#pragma once
#include <string>

#include "../Util/Log.hpp"
#include "GameScene.hpp"
#include "SceneBase.hpp"

/**
 * @brief The scene shown at the game's startup.
 *
 */
class MainMenuScene : public SceneBase {
 public:
  MainMenuScene(SceneManager* sm, SceneContext* sceneContext);
  virtual void event_load_scene() override;

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
