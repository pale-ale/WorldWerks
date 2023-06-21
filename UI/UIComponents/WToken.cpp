#include "WToken.hpp"

#include "../../TooDeeEngine/Util/Math.hpp"

TokenUIComponent::TokenUIComponent(UISystem *uiSystem, std::weak_ptr<UIElement> parent, const Token* token,
               Binding<sf::Vector2i> moveBinding)
    : UIComponent(uiSystem, parent), token{token}, moveBinding{moveBinding} {
  setup_sprite();
}

bool TokenUIComponent::is_mouse_inside(const sf::Vector2i &mousePos) {
  auto &pos = sprite.getPosition();
  float dx = mousePos.x - pos.x;
  float dy = mousePos.y - pos.y;
  float distance = length({dx, dy});
  return distance <= radius;
}

/**
 * @brief Change the color of the Widget's outline.
 */
void TokenUIComponent::set_outline_color(sf::Color color) {
  tokenShape.setOutlineColor(color);
  rtex->draw(tokenShape);
  rtex->display();
  sprite.setTexture(rtex->getTexture());
}

/**
 * @brief Initialize textures and the sprite.
 */
void TokenUIComponent::setup_sprite() {
  rtex = new sf::RenderTexture();
  if (!rtex->create(get_full_radius() * 2, get_full_radius() * 2)) {
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
  sprite.setTexture(rtex->getTexture(), true);
  sprite.setPosition((float)token->get_position().x, (float)token->get_position().y);
  sprite.setOrigin(get_full_radius(), get_full_radius());
}

void TokenUIComponent::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  UIComponent::draw(target, states);
  if (line) {
    target.draw(*line);
  }
}

bool TokenUIComponent::event_mouse_down(const sf::Vector2i &mousePos) {
  if (is_mouse_inside(mousePos)) {
    bBeingDragged = true;
    return true;
  }
  return false;
}

void TokenUIComponent::event_mouse_moved(const sf::Vector2i &mousePos) {
  if (bBeingDragged) {
    auto parentPos = get_parent_position();
    auto relativePos = mousePos - parentPos;
    update_position(relativePos);
  }
}

bool TokenUIComponent::event_mouse_up(const sf::Vector2i &mousePos) {
  if (bBeingDragged) {
    bBeingDragged = false;
    if (moveBinding.set) {
      moveBinding.set(get_parent_position() + relativePosition);
    }
    return true;
  }
  return false;
}
