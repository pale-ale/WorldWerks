#pragma once
#include <filesystem>

#include "../3rdParty/tinyxml2.hpp"
#include "../Data/DataNode.hpp"

using std::filesystem::canonical;

namespace tmx {
/**
 * @brief Contains the tiles, dimensions, source image etc. needed to display a tile.
 */
struct Tileset : public DataNode {
  Tileset(tinyxml2::XMLElement *element, DataNode *parent, std::filesystem::path mapPath)
      : DataNode(element, parent), mapPath{mapPath} {}
  virtual void commit_data() override {}
  virtual void update_data() override {
    // Obtain the relative tileset file path and it's first global tile id.
    bool errors = false;
    errors |= (bool)element->QueryAttribute("firstgid", &firstgid);
    errors |= (bool)element->QueryAttribute("source", &relativeTilesetPath);
    if (errors) {
      printf("[Tileset]: Errors in tileset node of map file.\n");
    }

    // Convert the relative path to an absolute one, abort if it doesn't exist.
    auto mapFolder = mapPath.remove_filename();
    tilesetPath = std::filesystem::weakly_canonical(
        mapFolder.append(std::string(relativeTilesetPath)));
    if (!std::filesystem::exists(tilesetPath)) {
      printf("[Tileset]: Error reading tileset at '%s': File does not exist.\n",
             tilesetPath.c_str());
      return;
    }

    // Open the tsx and read the tileset data.
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile(tilesetPath.c_str());
    auto tilesetElement = xmlDoc.FirstChildElement();
    errors |= !tilesetElement;
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
      imagePath = std::filesystem::canonical(tilesetFolder.append(relativeImagePath));
    }
    if (errors) {
      printf("[Tileset]: Errors in tileset file at '%s'.\n", imagePath.c_str());
    }
  }

  std::filesystem::path mapPath;
  std::string tilesetPath; /** @brief The absolute path to this tileset */

  // Tileset info inside the tmx
  int firstgid;                    /** @brief The first id in the global map context */
  const char *relativeTilesetPath; /** @param Used to write the path back later */

  // Tileset info inside the tsx
  int tilewidth, tileheight;     /** @brief Tile dimensions in Tiled units */
  int tilecount, columns;        /** @brief No. of tiles in each dimension */
  const char *relativeImagePath; /** @brief Relative path to the tileset's source image */
  std::string name;              /** @brief Name of this tileset */
  std::string imagePath;         /** @brief Absolute path to the source image. */
};
}  // namespace tmx
