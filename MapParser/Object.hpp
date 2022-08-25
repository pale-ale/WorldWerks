#pragma once
#include <map>
#include <string>

#include "../Data/DataNode.hpp"

namespace tmx {
/**
 * @brief Tiled objects contain a position, name, and other attributes.
 */
struct Object : public DataNode {
  Object(XMLElement * element, DataNode *parent) : DataNode(element, parent){}
  virtual void commit_data() override {
    set_attribute("id", id);
    // set_attribute("x", x);
    // set_attribute("y", y);
    // set_attribute("name", name.c_str());
    // set_attribute("width", width);
    // set_attribute("height", height);
    // set_attribute("gid", gid);
  }

  virtual void update_data() override {
    get_attribute("id", &id, true, "int");
    get_attribute("x", &x, true, "int");
    get_attribute("y", &y, true ,"int");
    const char* cname = "";
    get_attribute("name", &cname);
    get_attribute("width", &width);
    get_attribute("height", &height);
    get_attribute("gid", &gid);
    name = cname;
  }
  int id;                    /** @brief The id of this object in it's tileset */
  int gid;                   /** @brief The global id, spanning over tilesets */
  int x, y;                  /** @brief The object's position in Tiled units */
  int width = 0, height = 0; /** @brief The object's dimensions in Tiled units.*/
  std::string name = "";     /** @brief The name of this object */
};
}  // namespace tmx
