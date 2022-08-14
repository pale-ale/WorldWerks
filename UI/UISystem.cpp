#include "UISystem.hpp"

UISystem::UISystem(sf::Vector2f viewportSize) : viewportSize{viewportSize} {
  root = create_widget<Canvas>(nullptr);
  root->size = viewportSize;
  printf("Root: %p\n", root.get());
}

void UISystem::event_callback(const sf::Event &event,
                              const sf::Vector2i &mousePos) {
  if (root) {
    root->on_event_received(event, mousePos);
  }
}

void UISystem::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (root) {
    target.draw(*root);
  }
}
