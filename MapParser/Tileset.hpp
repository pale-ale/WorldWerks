#pragma once

#include <filesystem>

#include "../Data/DataNode.hpp"
#include "../TooDeeEngine/3rdParty/tinyxml2.hpp"
#include "../TooDeeEngine/Util/Log.hpp"

namespace fs = std::filesystem;

namespace tmx {
/**
 * @brief Contains the tiles, dimensions, source image etc. needed to display a tile.
 */
struct Tileset : public DataNode {
  Tileset(tinyxml2::XMLElement *element, DataNode *parent, fs::path mapPath,
          fs::path tsxPath, int firstGID)
      : DataNode(element, parent),
        mapPath{mapPath},
        relativeTilesetPath{tsxPath},
        firstgid{firstGID} {
    update_data();
  }

  virtual void commit_data() override {}
  virtual void update_data() override;

  fs::path mapPath;
  std::string data;
  fs::path tilesetPath; /** @brief The absolute path to this tileset */
  bool fileExists = false;

  // Tileset info inside the tmx
  int firstgid;                 /** @brief The first id in the global map context */
  fs::path relativeTilesetPath; /** @brief Used to write the path back later */

  // Tileset info inside the tsx
  int tilewidth, tileheight;     /** @brief Tile dimensions in Tiled units */
  int tilecount, columns;        /** @brief No. of tiles in each dimension */
  const char *relativeImagePath; /** @brief Relative path to the tileset's source image */
  std::string name;              /** @brief Name of this tileset */
};
}  // namespace tmx
