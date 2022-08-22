#pragma once
#include <map>
#include <string>

#include "IParseable.hpp"

namespace tmx {
/**
 * @brief Tiled objects contain a position, name, and other attributes.
 */
struct Object : IParseable {
  virtual bool parse(tinyxml2::XMLElement* element) override {
    bool errors = false;
    errors |= (bool)element->QueryAttribute("id", &id);
    errors |= (bool)element->QueryAttribute("x", &x);
    errors |= (bool)element->QueryAttribute("y", &y);
    const char* cname = "";
    element->QueryAttribute("name", &cname);
    element->QueryAttribute("width", &width);
    element->QueryAttribute("height", &height);
    element->QueryAttribute("gid", &gid);
    name = cname;
    if (errors) printf("Errors parsing tmx::Object '%s'.\n", name.c_str());
    return !errors;
  }
  int id;                    /** @brief The id of this object in it's tileset */
  int gid;                   /** @brief The global id, spanning over tilesets */
  int x, y;                  /** @brief The object's position in Tiled units */
  int width = 0, height = 0; /** @brief The object's dimensions in Tiled units.*/
  std::string name;          /** @brief The name of this object */
};
}  // namespace tmx
