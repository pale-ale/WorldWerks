#include "UIElement.hpp"

#include "../Util/Log.hpp"
#include "UISystem.hpp"

UIElement::UIElement(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
                     std::string name, sf::Vector2i size, sf::Vector2i pos)
    : uiSystem{uiSystem}, parent{parent}, name{name}, size{size}, relativePosition{pos} {
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
 */
void UIElement::add_child(std::shared_ptr<UIElement> child) {
  auto start = children.begin(), end = children.end();
  if (std::find(start, end, child) != end){
    LOGERR(name.c_str(), fmt::format("Cannot add UIElement '{}': already a child.", child->name));
    return;
  }
  children.push_back(child);
  child->parent = shared_from_this();
}

/**
 * @brief Allows easy custom hit-testing for this widget.
 *
 * @param mousePos The position of the mouse in pixel coordiantes.
 * @return true if the mouse is inside,
 * @return false otherwise.
 */
bool UIElement::is_mouse_inside(const sf::Vector2i &mousePos) {
  auto parentPos = get_parent_position();
  int posx = parentPos.x + relativePosition.x, posy = parentPos.y + relativePosition.y;
  int mx = mousePos.x, my = mousePos.y;
  return posx <= mx && mx <= posx + size.x && posy <= my && my <= posy + size.y;
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
  for (auto child : children) {
    if (child->on_event_received(event, mousePos)) {
      return true;
    }
  }
  switch (event.type) {
    // This event cannot be handled by any widget - everyone gets to know the mouse
    // position.
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

    // Handled by buttons etc.
    case sf::Event::MouseButtonPressed:
      if (event.mouseButton.button == sf::Mouse::Button::Left) {
        if (bMouseOver) bContinuouslyPressed = true;
        if (event_mouse_down(mousePos)) return true;
      }
      break;

    case sf::Event::MouseButtonReleased:
      if (event.mouseButton.button == sf::Mouse::Button::Left) {
        // To avoid triggering multiple releases at once
        bool clickHandled = false;
        if (bMouseOver) {
          if (bContinuouslyPressed) {
            clickHandled = event_clicked();
          }
        }
        bContinuouslyPressed = false;
        return clickHandled | event_mouse_up(mousePos);
      }
      break;

    case sf::Event::KeyPressed:
      if (event_key_down(event)) return true;
      break;

    case sf::Event::TextEntered:
      if (event_text_input(event.text.unicode)) return true;
      break;

    default:
      break;
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

bool UIElement::remove_child(UIElement *child) {
  return children.remove_if([child](auto e) { return e.get() == child; }) == 1;
}

/**
 * @brief Remove this widget from the parent, effectively disabling and destroying it.
 */
void UIElement::remove_self() {
  LOGDBG(name.c_str(), "Removing self...");
  if (parent.expired()) {
    LOGERR(name.c_str(), "Tried to remove widget from nonexistent parent.");
    return;
  }
  if (!parent.lock()->remove_child(this)) {
    LOGERR(name.c_str(), "Removed a number of widgets != 1.");
  }
}

/**
 * @brief Call the event functions for gain/loss of focus.
 *
 * @param newFocus Whether the widget is now focused or not
 */
void UIElement::update_focus(bool newFocus) {
  if (newFocus) {
    event_focus_gained();
  } else {
    event_focus_lost();
  }
}

/**
 * @brief Is this widget currently focused in the UISystem?
 *
 * @return true if widget is focused, false otherwise
 */
bool UIElement::has_focus() { return uiSystem->get_focused_widget() == this; }
