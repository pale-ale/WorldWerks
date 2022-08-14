#include "WToken.hpp"

#include "../Math.hpp"

WToken::WToken(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
               const Token &token)
    : UIElement(uiSystem, parent), token{token} {
  setup_sprite();
}

bool WToken::is_mouse_inside(const sf::Vector2i &mousePos) {
  float dx = mousePos.x - token.gridPosition.x;
  float dy = mousePos.y - token.gridPosition.y;
  float distance = length({dx, dy});
  return distance <= radius;
}

void WToken::set_outline_color(sf::Color color) {
  tokenShape.setOutlineColor(color);
  rtex->draw(tokenShape);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
}

void WToken::setup_sprite() {
  rtex = new sf::RenderTexture();
  if (!rtex->create(get_full_radius() * 2, get_full_radius() * 2)) {
    printf("Couldnt create render texture in TokenDisplay");
    exit(1);
  }
  rtex->clear(sf::Color::Transparent);
  tokenShape = sf::CircleShape(radius);
  tokenShape.setFillColor(sf::Color::Red);
  tokenShape.setOutlineColor(sf::Color::Black);
  tokenShape.setOutlineThickness(borderThickness);
  tokenShape.setPosition(borderThickness, borderThickness);
  rtex->draw(tokenShape);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
  sprite.setPosition(token.gridPosition);
  sprite.setOrigin(get_full_radius(), get_full_radius());
}