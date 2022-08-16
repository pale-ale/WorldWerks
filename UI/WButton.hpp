#pragma once

#include "UIElement.hpp"

/*
  Base class for easy click event callback triggers.
  Set buttonClickCallback to a lambda of choice to use the event.
*/
class WButton : public UIElement {
 public:
  WButton(UISystem *uiSystem, std::weak_ptr<UIElement> parent,
          sf::Vector2f size = {100, 100})
      : UIElement(uiSystem, parent, size) {}

  std::function<void()> buttonClickCallback;

  virtual bool event_clicked() override {
    if (buttonClickCallback) {
      buttonClickCallback();
    }
    return true;
  }
};
