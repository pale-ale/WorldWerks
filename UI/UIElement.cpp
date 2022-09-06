#include "UIElement.hpp"

#include "UISystem.hpp"

UIElement::UIElement(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
                     sf::Vector2i size, sf::Vector2i pos)
    : uiSystem{uiSystem}, parent{parent}, size{size}, relativePosition{pos} {
  auto tex = new sf::RenderTexture();
  tex->create(size.x, size.y);
  auto rs = sf::RectangleShape({(float)size.x, (float)size.y});
  rs.setFillColor(sf::Color::White);
  tex->draw(rs);
  tex->display();
  sprite.setTexture(tex->getTexture(), true);
  update_position();
};

/**
 * @brief Adds a child to the widget tree.
 * 
 * @param child The child to add.
 * @param parent Who this child will be the child of, i.e. who it inherits the positoin and events from.
 */
void UIElement::add_child(std::shared_ptr<UIElement> child,
                          std::weak_ptr<UIElement> parent) {
  children.push_back(child);
  child->parent = parent;
}

/**
 * @brief Allows easy custom hit-testing for this widget.
 * 
 * @param mousePos The position of the mouse in pixel coordiantes.
 * @return true if the mouse is inside,
 * @return false otherwise.
 */
bool UIElement::is_mouse_inside(const sf::Vector2i &mousePos) {
  int mx = mousePos.x, my = mousePos.y, px = relativePosition.x, py = relativePosition.y;
  return px <= mx && mx <= px + size.x && py <= my && my <= py + size.y;
};

/**
 * @brief Get the root widget's size, which should equal the viewport's size.
 *
 * @return const sf::Vector2f& --- The viewport size
 */
const sf::Vector2i &UIElement::get_viewport_size() const {
  auto uiElement = this;
  while (!uiElement->parent.expired()) {
    uiElement = uiElement->parent.lock().get();
  }
  return uiElement->size;
}

/**
 * @brief Pass the event to this widget's children.
 *
 * @param event The event we wish to pass on to children
 * @param mousePos The global mouse position in in-game pixels at the time of the event
 * @return true --- The event was handled, discarding it.
 * @return false --- The event was not handled, allowing other widgets to receive it.
 */
bool UIElement::on_event_received(const sf::Event &event, const sf::Vector2i &mousePos) {
  switch (event.type) {
    case sf::Event::MouseMoved:
      event_mouse_moved(mousePos);
      if (!bMouseOver && is_mouse_inside(mousePos)) {
        bMouseOver = true;
        event_begin_mouse_over();
      } else if (bMouseOver && !is_mouse_inside(mousePos)) {
        bMouseOver = false;
        bContinuouslyPressed = false;
        event_end_mouse_over();
      }
      break;

    case sf::Event::MouseButtonPressed:
      if (bMouseOver && event.mouseButton.button == sf::Mouse::Button::Left) {
        /* To avoid clicking multiple buttons at once */
        for (auto &&child : children) {
          if (child->on_event_received(event, mousePos)) {
            return true;
          }
        }
        bContinuouslyPressed = true;
        if (event_mouse_down()) {
          return true;
        }
      }
      break;

    case sf::Event::MouseButtonReleased:
      if (event.mouseButton.button == sf::Mouse::Button::Left) {
        // To avoid triggering multiple releases at once
        for (auto &&child : children) {
          if (child->on_event_received(event, mousePos)) {
            return true;
          }
        }
        if (event_mouse_up() | (bContinuouslyPressed && event_clicked())) {
          return true;
        }
      }
      break;

    case sf::Event::TextEntered:
      event_text_input(event.text.unicode);

    case sf::Event::KeyPressed:
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

/**
 * @brief Draw this widget via it's sprite onto the render target.
 *
 * @param target Where this widget wil be drawn
 * @param states Contains shaders, blend functions, masks
 */
void UIElement::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite);
  for (auto &&child : children) {
    target.draw(*child);
  }
};

/**
 * @brief Get the parent's global position.
 *
 * @return const sf::Vector2f --- The parent's position in global coordinates
 */
sf::Vector2i UIElement::get_parent_position() const {
  if (parent.expired()) {
    return relativePosition;
  }
  auto p = parent.lock();
  return p->get_parent_position() + p->relativePosition;
}

/**
 * @brief Set a new local position and re-read the parent's position
 *
 * @param newRelativePosition The new position relative to the parent
 */
void UIElement::update_position(const sf::Vector2i &newRelativePosition) {
  relativePosition = newRelativePosition;
  event_position_updated();
  for (auto &&child : children) {
    child->update_position();
  }
}

/**
 * @brief Re-read the parent's position
 */
void UIElement::update_position() {
  event_position_updated();
  for (auto &&child : children) {
    child->update_position();
  }
}

void UIElement::event_position_updated() {
  auto pos = get_parent_position() + relativePosition;
  sprite.setPosition({(float)pos.x, (float)pos.y});
}
