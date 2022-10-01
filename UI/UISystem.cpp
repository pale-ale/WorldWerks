#include "UISystem.hpp"

UISystem::UISystem(sf::Vector2i viewportSize) : viewportSize{viewportSize} {
  root = create_widget<Canvas>(nullptr);
  root->sprite.setColor(sf::Color::Transparent);
  root->size = viewportSize;
}

/**
 * @brief Used to pass events into the UISystem to be distributed to the Widgets.
 *
 * @param event The sf::Event we want to propagate
 * @param mousePos The mouse position in game pixels when the event occured
 */
void UISystem::event_callback(const sf::Event &event, const sf::Vector2i &mousePos) {
  if (root) {
    root->on_event_received(event, mousePos);
  }
}

/**
 * @brief Draw the whole tree onto the RenderTarget.
 */
void UISystem::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (root) {
    target.draw(*root);
  }
}
