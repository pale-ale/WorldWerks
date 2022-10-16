#pragma once

#include <memory.h>

#include <SFML/Graphics.hpp>

#include "UIElement.hpp"
#include "WCanvas.hpp"

/**
 * @brief TheUISystem manages the widget tree, creates/removes widgets, etc..
 *
 */
class UISystem : public sf::Drawable {
 public:
  UISystem(sf::Vector2i viewportSize);

  /**
   * @brief Create a UIElement and add it to the widget tree to be managed by this
   * UISystem.
   *
   * @tparam C The class of the widget that will be created
   * @tparam Args Perfect forwarding of args
   * @param parent The parent this widget will become a child of. A widget must always
   * have a parent, unless it is the root.
   * @param args Args that will be forwarded to C's constructor
   * @return std::shared_ptr<C> --- The newly created widget.
   */
  template <class C, typename... Args>
  std::shared_ptr<C> create_widget(std::shared_ptr<UIElement> parent, Args &&...args) {
    std::shared_ptr<UIElement> newWidget =
        std::shared_ptr<C>(new C(this, parent, args...));
    newWidget->post_init();
    if (parent) {
      parent->add_child(newWidget, parent);
    }
    return std::dynamic_pointer_cast<C>(newWidget);
  };

  void request_focus(UIElement *widget);

  UIElement *get_focused_widget() { return focusedElement; }

  void event_callback(const sf::Event &event, const sf::Vector2i &mousePos);

  /**
   * @brief Get the root widget.
   *
   * @return std::shared_ptr<UIElement> --- The root widget
   */
  std::shared_ptr<UIElement> get_root() const { return root; }

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const;

 private:
  /** @brief The size of the Widget space. */
  sf::Vector2i viewportSize;

  /** @brief The Canvas at the base of the Widget tree. */
  std::shared_ptr<UIElement> root = nullptr;

  /** @brief The UIElement currently focused by the user, or nullptr. */
  UIElement *focusedElement = nullptr;
};
