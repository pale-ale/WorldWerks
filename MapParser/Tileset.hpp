#pragma once

#include "../3rdParty/tinyxml2.hpp"
#include "../Data/DataNode.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace tmx {
/**
 * @brief Contains the tiles, dimensions, source image etc. needed to display a tile.
 */
struct Tileset : public DataNode {
  Tileset(tinyxml2::XMLElement *element, DataNode *parent, fs::path mapPath, fs::path tsxPath, int firstGID)
      : DataNode(element, parent), mapPath{mapPath}, relativeTilesetPath{tsxPath}, firstgid{firstGID} {}
  virtual void commit_data() override {}
  virtual void update_data() override {
    printf("[Tileset]: Loading tileset data...\n");
    // Open the tsx and read the tileset data.
    tinyxml2::XMLDocument xmlDoc;
    fileExists = !fetch_data(relativeTilesetPath, data);
    if (!fileExists) {
      printf("[Tileset]: Error reading tileset at '%s': File does not exist.\n",
             relativeTilesetPath.c_str());
    }
    tilesetPath = mapPath.remove_filename() / relativeTilesetPath;

    xmlDoc.Parse(data.c_str());
    auto tilesetElement = xmlDoc.FirstChildElement();
    bool errors = !tilesetElement;
    if (tilesetElement) {
      errors |= (bool)tilesetElement->QueryAttribute("tilewidth", &tilewidth);
      errors |= (bool)tilesetElement->QueryAttribute("tileheight", &tileheight);
      errors |= (bool)tilesetElement->QueryAttribute("tilecount", &tilecount);
      errors |= (bool)tilesetElement->QueryAttribute("columns", &columns);
      const char *cname;
      errors |= (bool)tilesetElement->QueryAttribute("name", &cname);
      name = cname;
      auto imageNode = tilesetElement->FirstChildElement();
      errors |= (bool)imageNode->QueryAttribute("source", &relativeImagePath);
      auto tilesetFolder = std::filesystem::path(tilesetPath).remove_filename();
      // TODO imagePath = std::filesystem::canonical(tilesetFolder.append(relativeImagePath));
    }
    if (errors) {
      printf("[Tileset]: Errors in tileset file at '%s'.\n", imagePath.c_str());
      return;
    }
    printf("[Tileset]: Loaded successfully.\n");
  }

  fs::path mapPath;
  std::string data;
  fs::path tilesetPath; /** @brief The absolute path to this tileset */
  bool fileExists = false;

  // Tileset info inside the tmx
  int firstgid;                    /** @brief The first id in the global map context */
  fs::path relativeTilesetPath; /** @param Used to write the path back later */

  // Tileset info inside the tsx
  int tilewidth, tileheight;     /** @brief Tile dimensions in Tiled units */
  int tilecount, columns;        /** @brief No. of tiles in each dimension */
  const char *relativeImagePath; /** @brief Relative path to the tileset's source image */
  std::string name;              /** @brief Name of this tileset */
  std::string imagePath;         /** @brief Absolute path to the source image. */
};
}  // namespace tmx
