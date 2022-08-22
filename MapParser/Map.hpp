#pragma once
#include <map>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
#include "Layer.hpp"
#include "ObjectGroup.hpp"
#include "Tileset.hpp"

namespace tmx {
/**
 * @brief All the types currently supported by this Parser.
 */
enum class ENodeType { Map, Layer, Tileset, Object, ObjectGroup };

/**
 * @brief Used to convert the ENodeType to a corresponding string.
 */
const std::map<ENodeType, std::string> NodeTypeNameMap{
    {ENodeType::Map, "map"},
    {ENodeType::Layer, "layer"},
    {ENodeType::Tileset, "tileset"},
    {ENodeType::Object, "object"},
    {ENodeType::ObjectGroup, "objectgroup"}};
/**
 * @brief Used to convert a string to the corresponding ENodeType.
 */
const std::map<std::string, ENodeType> NodeNameTypeMap{
    {"map", ENodeType::Map},
    {"layer", ENodeType::Layer},
    {"tileset", ENodeType::Tileset},
    {"object", ENodeType::Object},
    {"objectgroup", ENodeType::ObjectGroup}};
/**
 * @brief Contains Layers which themselves contain everything on the map.
 */
struct Map {
  /**
   * @brief Parse an XMLElement and extract it's values.
   * @param element The element to parse
   * @param documentPath The path from where all relative paths start
   * @return False if values were missing/of wrong type, true otherwise
   */
  bool parse(tinyxml2::XMLElement *element, const char *documentPath) {
    bool errors = false;
    errors |= (bool)element->QueryAttribute("width", &width);
    errors |= (bool)element->QueryAttribute("height", &height);
    errors |= (bool)element->QueryAttribute("tilewidth", &tilewidth);
    errors |= (bool)element->QueryAttribute("tileheight", &tileheight);
    errors |= (bool)element->QueryAttribute("nextlayerid", &nextlayerid);
    errors |= (bool)element->QueryAttribute("nextobjectid", &nextobjectid);

    for (auto &&child = element->FirstChildElement(); child;
         child = child->NextSiblingElement()) {
      auto type = NodeNameTypeMap.find(std::string(child->Name()));
      if (type == NodeNameTypeMap.end()) {
        printf("Invalid type while parsing map children: %s\n", child->Name());
        return false;
      }

      switch (type->second) {
        case ENodeType::Tileset: {
          auto t = new Tileset();
          if (!t->parse(child, documentPath)) {
            printf("Error parsing tileset.\n");
            return false;
          }
          tilesets.push_back(t);
          break;
        }

        case ENodeType::Layer: {
          Layer *layer = new Layer();
          layer->parse(child);
          layers.push_back(layer);
          break;
        }

        case ENodeType::ObjectGroup: {
          ObjectGroup *group = new ObjectGroup();
          group->parse(child);
          objectGroups.push_back(group);
          break;
        }

        default:
          break;
      }
    }
    return !errors;
  }

  int width, height;             /** @brief The size of the map in no. of tiles */
  int tilewidth, tileheight;     /** @brief The size of each tile in Tiled's units */
  int nextlayerid, nextobjectid; /** @brief Use this ID when creating new tiles/objects */
  std::vector<Layer *> layers;   /** @brief Tile Layers in this map */
  std::vector<ObjectGroup *> objectGroups; /** @brief Object Layers inthis map */
  std::vector<Tileset *> tilesets;         /** @brief Tilesets referenced by this map */
};
}  // namespace tmx