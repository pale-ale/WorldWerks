#pragma once

#include "UIElement.hpp"

class Canvas : public UIElement {
 public:
  Canvas(UISystem *uiSystem, std::shared_ptr<UIElement> parent):UIElement(uiSystem, parent){
    if (parent){
      this->size = parent->size;
    }
  }
};