#include <SFML/Graphics.hpp>

#include "SpriteLoader/SpriteLoader.h"
#include "Tabletop/Board.hpp"
#include "UI/InputManager.hpp"
#include "UI/TokenUI.hpp"
#include "UI/UISystem.hpp"
#include "UI/Textbox.hpp"
#include "UI/WBoard.hpp"

bool RUNNING = true;
int PIXELSCALE = 2;
sf::Vector2f RESOLUTION(600, 350);
sf::Vector2f WINDOW_SIZE(RESOLUTION.x* PIXELSCALE, RESOLUTION.y* PIXELSCALE);

InputManager inputManager;
UISystem uiSystem(RESOLUTION);

int main() {
  SpriteLoader::spriteFolderPath = "/home/alba/projects/WorldWerks/Sprites/";
  SpriteLoader::spriteKeys = {{"MapImage", "parchmentmap.png"}};
  
  sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y),
                          "WorldWerks");
  window.setFramerateLimit(60);
  sf::View view(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));
  window.setView(view);
  sf::RenderTexture windowTexture;
  sf::Sprite windowSprite;
  windowTexture.create(WINDOW_SIZE.x, WINDOW_SIZE.y);
  sf::Event event;

  Board board;
  Token token(0, {150, 200}, "Token, Destroyer of Friendships");
  board.tokens.push_back(token);
  uiSystem.create_widget<WBoard>(uiSystem.get_root(), const_cast<const Board*>(&board), SpriteLoader::getInstance(), RESOLUTION);

  auto cb = [](const sf::Event &event, const sf::Vector2i &mousePos){uiSystem.event_callback(event, mousePos);};
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
    windowTexture.draw(uiSystem);
    windowTexture.display();
    windowSprite.setTexture(windowTexture.getTexture());
    window.draw(windowSprite);
    window.display();
  }
}