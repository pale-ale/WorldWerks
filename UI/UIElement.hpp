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
            std::string name = "UIElement", sf::Vector2i size = {100, 100},
            sf::Vector2i pos = {0, 0});

  /**
   * @brief Called after the constructor finished and allows shared_from_this().
   */
  virtual void post_init(){};

 public:
  void add_child(std::shared_ptr<UIElement> child);
  bool remove_child(UIElement *child);
  void remove_self();
  void update_position(const sf::Vector2i &newRelativePosition);
  void update_position();
  void update_focus(bool newFocus);
  bool has_focus();
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

  /** @brief Called when the left mouse button is pressed anywhere. */
  virtual bool event_mouse_down(const sf::Vector2i &mousePos) { return false; }

  /** @brief Called when the left mouse button is released anywhere. */
  virtual bool event_mouse_up(const sf::Vector2i &mousePos) { return false; }

  /** @brief Called whene the mouse moved anywhere. */
  virtual void event_mouse_moved(const sf::Vector2i &mousePos) {}

  /** @brief Called when a key was pressed. */
  virtual bool event_key_down(const sf::Event &keyEvent) { return false; }

  /** @brief Called when a character was being typed */
  virtual bool event_text_input(const char &input) { return false; }

  /** @brief Called when this widget is focused */
  virtual void event_focus_gained() {}

  /** @brief Called when this widget is no longer focused */
  virtual void event_focus_lost() {}

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  /** @brief The sprite used to draw this widget. */
  sf::Sprite sprite;

  /** @brief True if the mouse is currently inside this widget. */
  bool bMouseOver = false;

  /** @brief True if LMB was pressed over this widget and mouse did not leave. */
  bool bContinuouslyPressed = false;
  
  /** @brief The UISystem is needed to create child widgets. */
  UISystem *uiSystem;

  /** @brief Name of this widget, for easier logging */
  std::string name;
};
