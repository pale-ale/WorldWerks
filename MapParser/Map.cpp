#include "Map.hpp"

namespace tmx {

void Map::update_data() {
  get_attribute("width", &width, true, "int");
  get_attribute("height", &height, true, "int");
  get_attribute("tilewidth", &tilewidth, true, "int");
  get_attribute("tileheight", &tileheight, true, "int");
  get_attribute("nextlayerid", &nextlayerid, true, "int");
  get_attribute("nextobjectid", &nextobjectid, true, "int");

  for (auto&& child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    auto type = NodeNameTypeMap.find(std::string(child->Name()));
    if (type == NodeNameTypeMap.end()) {
      LOGERR("Map", fmt::format("Invalid type while parsing map children: '{}'.",
                                child->Name()));
      continue;
    }

    switch (type->second) {
      case ENodeType::Tileset: {
        auto&&[tilesetPath, firstgid, errors] = get_tileset_info(child);
        if (errors){
          LOGERR("Map", "Error constructing tileset.");
          break;
        }
        tilesets.push_back(new Tileset(nullptr, this, documentPath, tilesetPath, firstgid));
        break;
      }

      case ENodeType::Layer: {
        Layer* layer = new Layer(child, this);
        layer->update_data();
        layers.push_back(layer);
        break;
      }

      case ENodeType::ObjectGroup: {
        ObjectGroup* group = new ObjectGroup(child, this);
        group->update_data();
        objectGroups.push_back(group);
        break;
      }

      default:
        break;
    }
  }
}

void Map::commit_data() {}

/**
 * @brief Obtain the source file and the firstgid of a tileset XMLElement in a tmx file.
 * 
 * @param element the XMLElement to get the values from
 * @return std::tuple<std::string, int, bool> i.e. the relative source file path, firstgid, and if errors were found
 */
std::tuple<std::string, int, bool> Map::get_tileset_info(XMLElement* element){
  bool errors = false;
  const char* tsxPath;
  int firstGID;
  errors |= element->QueryAttribute("source", &tsxPath);
  errors |= element->QueryAttribute("firstgid", &firstGID);
  return {tsxPath, firstGID, errors};
}
}  // namespace tmx
