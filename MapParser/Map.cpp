#include "Map.hpp"

namespace tmx {

void Map::update_data() {
  get_attribute("width", &width, true, "int");
  get_attribute("height", &height, true, "int");
  get_attribute("tilewidth", &tilewidth, true, "int");
  get_attribute("tileheight", &tileheight, true, "int");
  get_attribute("nextlayerid", &nextlayerid, true, "int");
  get_attribute("nextobjectid", &nextobjectid, true, "int");

  for (auto &&child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    auto type = NodeNameTypeMap.find(std::string(child->Name()));
    if (type == NodeNameTypeMap.end()) {
      printf("[Map]: Invalid type while parsing map children: %s\n", child->Name());
      continue;
    }

    switch (type->second) {
      case ENodeType::Tileset: {
        auto t = new Tileset(child, this, documentPath);
        t->update_data();
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
        ObjectGroup *group = new ObjectGroup(child, this);
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

}  // namespace tmx
