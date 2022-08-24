#pragma once

#include "UIElement.hpp"

/**
 * @brief Base class for easy click event callback triggers.
 * Set buttonClickCallback to e.g. a lambda of choice to use the event.
 */
class WButton : public UIElement {
 public:
  WButton(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
          sf::Vector2i size = {100, 100}, sf::Vector2i pos = {0,0})
      : UIElement(uiSystem, parent, size, pos) {}

  virtual bool event_clicked() override {
    if (buttonClickCallback) {
      buttonClickCallback();
    }
    return true;
  }
  
  /**
   * @brief The functor to be called when this button is clicked.
   */
  std::function<void()> buttonClickCallback;
};
