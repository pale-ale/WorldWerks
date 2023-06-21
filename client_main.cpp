#include <SFML/Graphics.hpp>

#include "../TooDeeEngine/Scene/SceneManager.hpp"
#include "../TooDeeEngine/UI/InputManager.hpp"
#include "../TooDeeEngine/UI/UISystem.hpp"
#include "MapParser/MapParser.hpp"
#include "Scene/GameScene.hpp"
#include "Scene/MainMenuScene.hpp"
#include "SpriteLoader/SpriteLoader.h"

bool RUNNING = true;
int PIXELSCALE = 2;
sf::Vector2i RESOLUTION(600, 350);
sf::Vector2i WINDOW_SIZE(RESOLUTION.x* PIXELSCALE, RESOLUTION.y* PIXELSCALE);

InputManager inputManager;
SceneManager sceneManager;
UISystem uiSystem(RESOLUTION);

int main() {
  SpriteLoader::spriteFolderPath = "/home/alba/projects/WorldWerks/Sprites/";

  sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "WorldWerks");
  window.setFramerateLimit(60);
  sf::View view(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));
  window.setView(view);
  sf::RenderTexture windowTexture;
  sf::Sprite windowSprite;
  windowTexture.create(WINDOW_SIZE.x, WINDOW_SIZE.y);
  sf::Event event;

  sceneManager.sceneContext.uiSystem = &uiSystem;
  sceneManager.sceneContext.resolution = {RESOLUTION.x, RESOLUTION.y};
  sceneManager.sceneContext.mapParser = new tmx::MapParser();
  sceneManager.load_scene<MainMenuScene>();

  auto cb = [](const sf::Event& event, const sf::Vector2i& mousePos) {
    uiSystem.event_callback(event, mousePos);
  };
  inputManager.register_callback(cb, InputEventType::KeyboardEvent);
  inputManager.register_callback(cb, InputEventType::MouseEvent);

  while (RUNNING) {
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        RUNNING = false;
      }
      sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
      auto mp = window.mapPixelToCoords(pixelPos);
      inputManager.process_event(event, {(int)mp.x, (int)mp.y});
    }
    windowTexture.draw(sceneManager);
    if (sceneManager.sceneContext.clientEp) {
      sceneManager.sceneContext.clientEp->digest_incoming();
    }
    windowTexture.display();
    windowSprite.setTexture(windowTexture.getTexture());
    window.draw(windowSprite);
    window.display();
  }
}