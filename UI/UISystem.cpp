#include "UISystem.hpp"

UISystem::UISystem(sf::Vector2i viewportSize) : viewportSize{viewportSize} {
  root = create_widget<WCanvas>(nullptr);
  root->sprite.setColor(sf::Color::Transparent);
  root->size = viewportSize;
}

/**
 * @brief Request a certain widget be focused or nullptr to not focus a widget at all.
 * 
 * @param widget The widget that should gain focus, or nullptr
 */
void UISystem::request_focus(UIElement *widget) {
  if (focusedElement) focusedElement->update_focus(false);
  if (widget) widget->update_focus(true);
  focusedElement = widget;
}

/**
 * @brief Used to pass events into the UISystem to be distributed to the Widgets.
 *
 * @param event The sf::Event we want to propagate
 * @param mousePos The mouse position in game pixels when the event occured
 */
void UISystem::event_callback(const sf::Event &event, const sf::Vector2i &mousePos) {
  if (focusedElement && focusedElement->on_event_received(event, mousePos)){
    return;
  }
  if (root) {
    root->on_event_received(event, mousePos);
  }
}

/**
 * @brief Draw the whole tree onto the RenderTarget.
 */
void UISystem::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (root) {
    target.draw(*root, states);
  }
}
