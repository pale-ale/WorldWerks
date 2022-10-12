#pragma once
#include <map>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
#include "../Data/DataNode.hpp"
#include "../Storage/LiveStorage.hpp"
#include "../Util/Log.hpp"
#include "Layer.hpp"
#include "ObjectGroup.hpp"
#include "Tileset.hpp"
#include "filesystem"

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
  Map(XMLElement* element, const char* documentPath)
      : DataNode(element), documentPath{documentPath} {}
  virtual void update_data() override;
  virtual void commit_data() override;
  std::tuple<std::string, int, bool> get_tileset_info(XMLElement* element);

  virtual bool fetch_data(const std::string& key, std::string& data) override {
    return LiveStorage::retrieve(key, data);
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
  std::vector<Layer*> layers;

  /** @brief Object Layers in this map */
  std::vector<ObjectGroup*> objectGroups;

  /** @brief Tilesets referenced by this map */
  std::vector<Tileset*> tilesets;
};
}  // namespace tmx
