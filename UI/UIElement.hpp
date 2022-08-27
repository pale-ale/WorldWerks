#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

class UISystem;

/**
 * @brief Use this as base class of anything that fits into the UIsystem's widget tree.
 */
class UIElement : public sf::Drawable, public std::enable_shared_from_this<UIElement> {
  friend UISystem;

 protected:
  UIElement(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
            sf::Vector2i size = {100, 100}, sf::Vector2i pos = {0, 0});

  /**
   * @brief Called after the constructor finished and allows shared_from_this().
   */
  virtual void post_init(){};

 public:
  void add_child(std::shared_ptr<UIElement> child, std::weak_ptr<UIElement> parent);
  void update_position(const sf::Vector2i &newRelativePosition);
  void update_position();
  virtual bool is_mouse_inside(const sf::Vector2i &mousePos);
  virtual bool on_event_received(const sf::Event &event, const sf::Vector2i &mousePos);
  sf::Vector2i get_parent_position() const;
  const sf::Vector2i &get_viewport_size() const;

  /** @brief The size of the widget */
  sf::Vector2i size;

  /** @brief The position of the widget in world coordinates */
  sf::Vector2i relativePosition;

  /** @brief Widgets contained inside this widget */
  std::list<std::shared_ptr<UIElement>> children;

  /** @brief The parent of this widget. The root widget's parent is a nullptr */
  std::weak_ptr<UIElement> parent;

  ////// Events and other hooks, meant to be overridden. //////

  /** @brief Called when a parent of this Widet moves to keep it's relative position */
  virtual void event_position_updated();

  /** @brief Called when the mouse enters this widget's hitbox. */
  virtual void event_begin_mouse_over() {}

  /** @brief Called when the mouse leaves this widget's hitbox. */
  virtual void event_end_mouse_over() {}

  /** @brief Called when the mouse clicked on this widget. */
  virtual bool event_clicked() { return false; }

  /** @brief Called whene the mouse moved anywhere. */
  virtual void event_mouse_moved(const sf::Vector2i &mousePos) {}

  /** @brief Called when a key was pressed. */
  virtual void event_key_down(const sf::Event &keyEvent) {}

  /** @brief Called when a character was being typed */
  virtual void event_text_input(const char &input) {}

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  /** @brief The sprite used to draw this widget. */
  sf::Sprite sprite;

  /** @brief True if the mouse is currently inside this widget. */
  bool bMouseOver = false;

  /** @brief The UISystem is needed to create child widgets. */
  UISystem *uiSystem;
};
