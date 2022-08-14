#pragma once

#include "InventoryItem.hpp"

class InventoryStack {
 public:
  InventoryStack(const InventoryItem &item, int count);
  const InventoryItem item;
  int itemCount;
  int get_weight();
  bool add_item(int count = 1);
  bool remove_item(int count = 1);
  void add_stack(InventoryStack* otherStack);
};
