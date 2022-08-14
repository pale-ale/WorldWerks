#pragma once

#include <memory.h>

#include <SFML/Graphics.hpp>

#include "UIElement.hpp"
#include "WCanvas.hpp"

class UISystem : public sf::Drawable {
 public:
  UISystem(sf::Vector2f viewportSize);

  /* The only way to create Widgets, to ensure they are managed correctly. */
  template <class C, typename... Args>
  std::shared_ptr<UIElement> create_widget(std::shared_ptr<UIElement> parent,
                                           Args &&...args) {
    std::shared_ptr<UIElement> newWidget =
        std::shared_ptr<C>(new C(this, parent, args...));
    newWidget->post_init();
    if (parent) {
      parent->add_child(newWidget, parent);
    }
    return newWidget;
  };

  /* Used to pass events into the UISystem to be distributed to the Widgets. */
  void event_callback(const sf::Event &event, const sf::Vector2i &mousePos);

  /* Return the root Widget. */
  std::shared_ptr<UIElement> get_root() const { return root; }

 protected:
  /* Draw the whole tree onto the RenderTarget. */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const;

 private:
  /* The size of the Widget space. */
  sf::Vector2f viewportSize;

  /* The Canvas at the base of the Widget tree. */
  std::shared_ptr<UIElement> root = nullptr;
};
