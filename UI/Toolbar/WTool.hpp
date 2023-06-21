#pragma once

#include "../../SpriteLoader/SpriteLoader.h"
#include "../../TooDeeEngine/UI/UIElement.hpp"

class WTool : public UIElement {
 public:
  WTool(UISystem* ui, std::weak_ptr<UIElement> parent)
      : UIElement(ui, parent, "Tool", {30, 30}) {
    sprite.setColor(sf::Color::Black);
  }

  sf::Color enabledColor = {0, 0, 0};
  sf::Color mouseoverColor = {25, 75, 25};
  sf::Color activeColor = {100, 200, 100};
  sf::Color disabledColor = {100, 100, 100};

  virtual void event_begin_mouse_over() override { sprite.setColor(mouseoverColor); }

  virtual void event_end_mouse_over() override { sprite.setColor(enabledColor); }

  virtual bool event_clicked() override {
    sprite.setColor(activeColor);
    return true;
  }
};
