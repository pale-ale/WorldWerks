#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

class UISystem;

/* Base class of anything that fits into the Widget tree. */
class UIElement : public sf::Drawable,
                  public std::enable_shared_from_this<UIElement> {
  friend UISystem;

 protected:
  UIElement(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
            sf::Vector2f size = {100, 100}, sf::Vector2f pos = {0, 0})
      :uiSystem{uiSystem}, parent{parent}, size{size}, relativePosition{pos} {};

  /* Called after the constructor finished, allows shared_from_this(). */
  virtual void post_init(){};

 public:
  /* The size of the widget. */
  sf::Vector2f size;

  /* The position of the widget in world coordinates. */
  sf::Vector2f relativePosition;

  /* Widgets contained inside this widget. */
  std::list<std::shared_ptr<UIElement>> children;

  /* The parent of this widget. The root widget's parent is a nullptr. */
  std::weak_ptr<UIElement> parent;

 public:
  /* Adds a child to the widget tree, requires a weak_ptr to this. */
  void add_child(std::shared_ptr<UIElement> child,
                 std::weak_ptr<UIElement> parent);

  /* Returns the root widget's size, which should equal the viewport's size. */
  const sf::Vector2f &get_viewport_size() const;

  /* Return the parent's global position. */
  const sf::Vector2f get_parent_position() const;

  /* Set a new local position and re-read the parent's position */
  void update_position(const sf::Vector2f &newRelativePosition = {0, 0});

  ////// Events and other hooks, meant to be overridden. //////

  /* Called when a parent of this Widet moves to keep it's relative position */
  virtual void event_position_updated();

  /* Called when the mouse enters this widget's hitbox. */
  virtual void event_begin_mouse_over() {}

  /* Called when the mouse leaves this widget's hitbox. */
  virtual void event_end_mouse_over() {}

  /* Called when the mouse clicks on this widget. */
  virtual bool event_clicked() { return false; }

  /* Called whenever the mouse moves anywhere. */
  virtual void event_mouse_moved(const sf::Vector2i &mousePos) {}

  /* Called whenever the mouse moves anywhere. */
  virtual void event_key_down(const sf::Event &keyEvent) {}

  /* Allows for easy editing of the hit-testing for this widget. */
  virtual bool is_mouse_inside(const sf::Vector2i &mousePos);

  /* Used to pass along events to children */
  virtual bool on_event_received(const sf::Event &event,
                                 const sf::Vector2i &mousePos);

 protected:
  /* The sprite used to draw this widget. */
  sf::Sprite sprite;

  /* True if the mouse is currently inside this widget. */
  bool bMouseOver = false;

  /* Draw this widget via it's sprite onto the render target. */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  /* The UISystem is needed to create child widgets. */
  UISystem *uiSystem;
};
