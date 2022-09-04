#pragma once
#include <string>

/**
 * @brief Allows tracking of modifiers for later use.
 * 
 */
struct StatModifier{
  std::string name = "A Modifier";
  std::string influencedStat = "";
  int amount;
  bool bIsPercentage;
};
