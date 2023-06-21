#include <catch2/catch.hpp>
#include <string>
#include "../TooDeeEngine/Scene/SceneManager.hpp"
#include "../Tabletop/Token.hpp"
#include "../Scene/GameScene.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Build Scene", "[BuildSceneSC]"){
  SceneManager sceneManager;
  sceneManager.load_scene<GameScene>();
  auto scene = sceneManager.currentScene.get();
  scene->spawn_gamepiece<Token>(0, sf::Vector2i{0,0}, "TestToken");
  for (auto&& gp : scene->gamePieces){
    LOGINF("Test", gp.get()->name);
  }
}
