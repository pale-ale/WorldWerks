#pragma once
#include <filesystem>

#include "../3rdParty/tinyxml2.hpp"

using std::filesystem::canonical;

namespace tmx {
/**
 * @brief Contains the tiles, dimensions, source image etc. needed to display a tile.
 */
struct Tileset {
  /**
   * @brief Extract information from a .tsx tileset.
   *
   * @param element XML Data required to use the file
   * @param mapPath the path to the file
   * @return true if the file was parsed successfully, false otherwise
   */
  bool parse(tinyxml2::XMLElement *element, std::filesystem::path mapPath) {
    bool errors = false;
    errors |= (bool)element->QueryAttribute("firstgid", &firstgid);
    errors |= (bool)element->QueryAttribute("source", &relativeTilesetPath);
    if (errors) {
      printf("Errors in tileset node in the map file.\n");
      return false;
    }

    auto mapFolder = mapPath.remove_filename();
    tilesetPath = canonical(mapFolder.append(std::string(relativeTilesetPath)));
    if (!std::filesystem::exists(tilesetPath)) {
      printf("Tileset path '%s' does not exist.\n", tilesetPath.c_str());
      return false;
    }

    // Open the tsx that is referenced and read the tileset data.
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile(tilesetPath.c_str());
    auto tilesetElement = xmlDoc.FirstChildElement();
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
    if (errors) {
      printf("Errors in tileset file at '%s'.\n", imagePath.c_str());
    }
    return !errors;
  }
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
