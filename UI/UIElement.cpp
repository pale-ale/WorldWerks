#include "UIElement.hpp"

#include "UISystem.hpp"

void UIElement::add_child(std::shared_ptr<UIElement> child,
                std::weak_ptr<UIElement> parent) {
  children.push_back(child);
  child->parent = parent;
}

bool UIElement::is_mouse_inside(const sf::Vector2i &mousePos) {
  int mx = mousePos.x, my = mousePos.y, px = relativePosition.x, py = relativePosition.y;
  return px <= mx && mx <= px + size.x && py <= my && my <= py + size.y;
};

const sf::Vector2f &UIElement::get_viewport_size() const {
  auto uiElement = this;
  while (!uiElement->parent.expired()) {
    uiElement = uiElement->parent.lock().get();
  }
  return uiElement->size;
}

bool UIElement::on_event_received(const sf::Event &event,
                                  const sf::Vector2i &mousePos) {
  switch (event.type) {
    case sf::Event::EventType::MouseMoved:
      event_mouse_moved(mousePos);
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
        for (auto &&child : children) {
          if (child->on_event_received(event, mousePos)) {
            return true;
          }
        }
        if (event_clicked()) {
          return true;
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
    if (child->on_event_received(event, mousePos)) {
      return true;
    }
  }
  return false;
}

void UIElement::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite);
  for (auto &&child : children) {
    target.draw(*child);
  }
};

const sf::Vector2f UIElement::get_parent_position() const{
  if (parent.expired()){
    return {0,0};
  }
  auto p = parent.lock();
  return p->get_parent_position() + p->relativePosition;
}

void UIElement::update_position(const sf::Vector2f &newRelativePosition){
  relativePosition = newRelativePosition;
  event_position_updated();
  for (auto&& child : children){
    child->update_position();
  }
}

void UIElement::event_position_updated() {
  sprite.setPosition(get_parent_position() + relativePosition);
}
