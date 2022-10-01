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
      LOG("Map", ELogLevel::ERROR,
          fmt::format("Invalid type while parsing map children: '{}'.", child->Name()));
      continue;
    }

    switch (type->second) {
      case ENodeType::Tileset: {
        LOG("Map", ELogLevel::DEBUG, "Constructing tileset...");
        bool errors = false;
        const char* tsxPath;
        int firstGID;
        errors |= child->QueryAttribute("source", &tsxPath);
        errors |= child->QueryAttribute("firstgid", &firstGID);
        if (errors) {
          LOG("Map", ELogLevel::ERROR, "Error constructing tileset.");
          return;
        }
        auto t = new Tileset(child, this, documentPath, tsxPath, firstGID);
        t->update_data();
        tilesets.push_back(t);
        break;
      }

      case ENodeType::Layer: {
        Layer* layer = new Layer();
        layer->parse(child);
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
 * @brief Build a tileset with the data in tsxData.
 *
 * @param tsxData Contains the necessary data and paths to create a tileset
 */
void Map::update_tileset_data(const std::string& tsxData) {
  LOGINF("Map", "Updating tileset data...");
  tinyxml2::XMLDocument doc;
  doc.Parse(tsxData.c_str());
  const char* tsxName;
  auto e = doc.FirstChildElement();
  if (!e) {
    LOGERR("Map", "Error in tileset data: No first element.");
    return;
  }
  e->QueryAttribute("name", &tsxName);
  for (auto&& ts : tilesets) {
    if (ts->name == tsxName) {
      ts = new tmx::Tileset(e, this, documentPath, ts->tilesetPath, ts->firstgid);
      ts->update_data();
    }
  }
}
}  // namespace tmx
