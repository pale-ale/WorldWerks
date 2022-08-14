#pragma once

#include <string>
#include "../Enums/ERarity.h"

class InventoryItem{
 public:
  InventoryItem(int id):id{id}{}
  // The weight of one unit of this item in kg
  int weight;
  // The volume, i.e. it's size in cm^3
  int volume;
  // The value, in copper
  int value;
  // ID used to determine stack compatability
  int id;
  // How many items per stack
  int maxstack;
  std::string name;
  std::string description;
  ERarirty rarity;
};
