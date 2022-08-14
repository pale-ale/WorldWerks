#include "InventoryStack.hpp"

#include <cmath>

InventoryStack::InventoryStack(const InventoryItem &item, int count):item{item}{
  itemCount = count;
}

int InventoryStack::get_weight() { return item.weight * itemCount; }

bool InventoryStack::add_item(int count) {
  bool overfull = itemCount + count > item.maxstack;
  itemCount = std::min<int>(itemCount + count, item.maxstack);
  return !overfull;
}

bool InventoryStack::remove_item(int count) {
  bool underempty = itemCount - count < 0;
  itemCount = std::max<int>(itemCount - count, 0);
  return !underempty;
}

void InventoryStack::add_stack(InventoryStack* otherStack) {
  if (item.id != otherStack->item.id){
    return;
  }
  int transferCount = std::min<int>(item.maxstack - itemCount, otherStack->itemCount);
  if (transferCount > 0){
    otherStack->itemCount -= transferCount;
    itemCount += transferCount;
  }
}