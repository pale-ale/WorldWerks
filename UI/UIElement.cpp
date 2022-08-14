#include "UIElement.hpp"
#include "UISystem.hpp"

bool UIElement::is_mouse_inside(const sf::Vector2i &mousePos) {
  int mx = mousePos.x, my = mousePos.y, px = position.x, py = position.y;
  return px <= mx && mx <= px + size.x && py <= my && my <= py + size.y;
};

const sf::Vector2f &UIElement::get_viewport_size() const {
  auto uiElement = this;
  while (!uiElement->parent.expired()) {
    uiElement = uiElement->parent.lock().get();
  }
  return uiElement->size;
}

void UIElement::on_event_received(const sf::Event &event,
                                  const sf::Vector2i &mousePos) {
  event_mouse_moved(mousePos);
  switch (event.type) {
    case sf::Event::EventType::MouseMoved:
      if (!bMouseOver && is_mouse_inside(mousePos)) {
        bMouseOver = true;
        event_begin_mouse_over();
      } else if (bMouseOver && !is_mouse_inside(mousePos)) {
        bMouseOver = false;
        event_end_mouse_over();
      }
      break;

    case sf::Event::EventType::MouseButtonPressed:
      if (bMouseOver && event.mouseButton.button == sf::Mouse::Button::Left) {
        /* To avoid clicking multiple buttons at once */
        if (event_clicked()) {  
          return;
        }
      }
      break;

    case sf::Event::EventType::KeyPressed:
      event_key_down(event);
      break;

    default:
      break;
  }
  for (auto &&child : children) {
    child->on_event_received(event, mousePos);
  }
}

void UIElement::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite);
  for (auto &&child : children) {
    target.draw(*child);
  }
};
