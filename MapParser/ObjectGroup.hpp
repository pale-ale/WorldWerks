#pragma once
#include <string>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
#include "Object.hpp"

namespace tmx {
/**
 * @brief Essentially an Object layer. It contains objects.
 */
struct ObjectGroup {
  bool parse(tinyxml2::XMLElement* element) {
    bool errors = false;
    errors |= (bool)element->QueryAttribute("id", &id);
    errors |= (bool)element->QueryAttribute("name", &name);

    for (auto&& child = element->FirstChildElement(); child;
         child = child->NextSiblingElement()) {
      Object* obj = new Object();
      obj->parse(child);
      objects.push_back(obj);
    }

    return !errors;
  }

  int id;                       /** @brief The id of the ObjectGroup */
  const char* name;             /** @brief The name of the ObjectGroup */
  std::vector<Object*> objects; /** @brief The Objects contained */
};
}  // namespace tmx
