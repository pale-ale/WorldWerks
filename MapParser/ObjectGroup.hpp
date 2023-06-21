#pragma once
#include <string>
#include <vector>

#include "../TooDeeEngine/3rdParty/tinyxml2.hpp"

#include "../Data/DataNode.hpp"
#include "Object.hpp"

namespace tmx {
/**
 * @brief Essentially an Object layer. It contains objects.
 */
struct ObjectGroup : public DataNode {
  ObjectGroup(XMLElement* element, DataNode* parent) : DataNode(element, parent) {}
  virtual void commit_data() override {}
  virtual void update_data() override {
    get_attribute("id", &id, true, "int");
    get_attribute("name", &name, true, "string");

    for (auto&& child = nodeData->element->FirstChildElement(); child;
         child = child->NextSiblingElement()) {
      Object* obj = new Object(child, this);
      obj->update_data();
      objects.push_back(obj);
      children.push_back(obj);
    }
  }

  int id;                       /** @brief The id of the ObjectGroup */
  const char* name = "";        /** @brief The name of the ObjectGroup */
  std::vector<Object*> objects; /** @brief The Objects contained */
};
}  // namespace tmx
