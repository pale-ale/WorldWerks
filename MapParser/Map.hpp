#pragma once
#include <map>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
#include "../Data/DataNode.hpp"
#include "Layer.hpp"
#include "ObjectGroup.hpp"
#include "Tileset.hpp"
#include "filesystem"
#include "../Storage/LiveStorage.hpp"


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
struct Map : public DataNode {
  Map(XMLElement *element, const char *documentPath)
      : DataNode(element), documentPath{documentPath} {}
  virtual void update_data() override;
  virtual void commit_data() override;

  void update_tileset_data(const std::string& tsxData){
    printf("[Map]: Updating tileset data....\n");
    tinyxml2::XMLDocument doc;
    doc.Parse(tsxData.c_str());
    const char* tsxName;
    auto e = doc.FirstChildElement();
    if (!e){
      printf("[Map]: Error in tileset data: No first element.\n");
      return;
    }
    e->QueryAttribute("name", &tsxName);
    for (auto&& ts : tilesets){
      if (ts->name == tsxName){
        ts = new tmx::Tileset(e, this, documentPath, ts->tilesetPath, ts->firstgid);
        ts->update_data();
      }
    }
  }

  virtual bool fetch_data(const std::string& key, std::string& data) override{
    if (LiveStorage::storage.count(key) > 0){
      data = LiveStorage::storage[key];
      return false;
    }
    printf("[Map]: Cannot fetch key %s: Key does not exist.\n", key.c_str());
    return true;
  }

  /** @brief The width of the map in no. of tiles */
  int width;

  /** @brief The height of the map in no. of tiles */
  int height;

  /** @brief The width of each tile in Tiled's units */
  int tilewidth;

  /** @brief The height of each tile in Tiled's units */
  int tileheight;

  /** @brief Use this ID when creating new layers */
  int nextlayerid;

  /** @brief Use this ID when creating new objects */
  int nextobjectid;

  /** @brief Needed to use the relative paths provided by Tiled's map files */
  std::filesystem::path documentPath;

  /** @brief Tile Layers in this map */
  std::vector<Layer *> layers;

  /** @brief Object Layers in this map */
  std::vector<ObjectGroup *> objectGroups;

  /** @brief Tilesets referenced by this map */
  std::vector<Tileset *> tilesets;
};
}  // namespace tmx
