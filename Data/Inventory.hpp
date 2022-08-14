#pragma once

#include <list>
#include "InventoryItem.hpp"
#include "InventoryStack.hpp"

class Inventory{
 public:
  const std::list<const InventoryStack> getContents();

};
