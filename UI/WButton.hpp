#pragma once

#include "UIElement.hpp"
#include "WTextbox.hpp"

/**
 * @brief Base class for easy click event callback triggers.
 * Set buttonClickCallback to e.g. a lambda of choice to use the event.
 */
class WButton : public UIElement {
 public:
  WButton(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
          sf::Vector2i size = {100, 100}, sf::Vector2i pos = {0,0}, std::string name = "WButton")
      : UIElement(uiSystem, parent, name, size, pos) {}

  void set_text(const std::string& newText);

  virtual void post_init() override;

  virtual bool event_clicked() override {
    if (buttonClickCallback) {
      buttonClickCallback();
    }
    return true;
  }

  virtual void event_begin_mouse_over() override {
    sprite.setColor({200,200,200});
  }

  virtual void event_end_mouse_over() override {
    sprite.setColor({255,255,255});
  }
  
  /**
   * @brief The functor to be called when this button is clicked.
   */
  std::function<void()> buttonClickCallback;
  std::weak_ptr<WTextbox> text;
};
