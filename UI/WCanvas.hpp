#pragma once

#include "UIElement.hpp"

class WCanvas : public UIElement {
 public:
  WCanvas(UISystem *uiSystem, std::shared_ptr<UIElement> parent):UIElement(uiSystem, parent, "WCanvas"){
    if (parent){
      this->size = parent->size;
    }
  }
};