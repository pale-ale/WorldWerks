#pragma once
#include <string>

#include "../3rdParty/tinyxml2.hpp"
#include "Map.hpp"

namespace tmx {

/**
 * @brief Use this class to build a tmx::Map out of a file or a string.
 */
class MapParser {
 public:
  /**
   * @brief Parse a file into a tmx::Map.
   *
   * @param path The path to the file we want to read
   * @return true if loading/parsing of the file was succesful, false otherwise
   */
  bool load_file(const std::string &path) {
    auto err = xmlDoc.LoadFile(path.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      printf("Error in XML Document: \n%s\n", xmlDoc.ErrorStr());
      return false;
    }
    return extract(path.c_str());
  };
  /**
   * @brief Parse a string into a tmx::Map.
   *
   * @param text The text we want to read
   * @param mapPath The path we use to link against Tilesets. Can be left empty.
   * @return true if loading/parsing of the string was succesful, false otherwise
   */
  bool load_text(const std::string &text, const char *mapPath = "") {
    auto err = xmlDoc.Parse(text.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      printf("Error in XML Document: \n%s\n", xmlDoc.ErrorStr());
      return false;
    }
    return extract(mapPath);
  };

  tmx::Map *map = nullptr; /** @brief Access the loaded tmx::Map after it's been parsed */

 private:
  tinyxml2::XMLDocument xmlDoc; /** @brief Needed to extract XML data */
  /**
   * @brief Fill our tmx::Map with data.
   *
   * @param path The path where relative paths are looked up from
   * @return true if there were no errors, false otherwise
   */
  bool extract(const char *path) {
    auto root = xmlDoc.RootElement();
    map = new Map();
    if (!map->parse(root, path)) {
      printf("Error parsing the map node.");
      return false;
    }
    return true;
  }
};

}  // namespace tmx