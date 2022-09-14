#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
#include "Map.hpp"

namespace fs = std::filesystem;
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
    printf("[MapParser]: Loading file '%s'.\n", path.c_str());
    std::ifstream is(path);
    std::stringstream buffer;
    buffer << is.rdbuf();
    data = buffer.str();

    auto err = xmlDoc.LoadFile(path.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      printf("[MapParser]: Error reading XML Document: \n%s\n", xmlDoc.ErrorStr());
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
    data = text;
    auto err = xmlDoc.Parse(text.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      printf("[MapParser]: Error reading XML Document: \n%s\n", xmlDoc.ErrorStr());
      return false;
    }
    return extract(mapPath);
  };

  /**
   * @brief Write the data in the data tree to disk.
   *
   * @param savePath The path where the file should be saved
   * @return true: Success ---
   * @return false: Could not save
   */
  bool save_file(const char *savePath) {
    auto err = xmlDoc.SaveFile(savePath);
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      printf("[MapParser]: Error saving XML Document: \n%s\n", xmlDoc.ErrorStr());
      return false;
    }
    printf("[MapParser]: Saved.\n");
    return true;
  }

  std::vector<std::string> get_missing_resource_paths() {
    std::vector<std::string> mrp;
    printf("[MapParser]: Tilesets:\n");
    for (auto &&tileset : map->tilesets) {
      if (!tileset->fileExists) {
        mrp.push_back(tileset->tilesetPath);
      }
      printf("TSX Name: '%s', Path: '%s', Exists: '%s'\n", tileset->name.c_str(),
              tileset->tilesetPath.c_str(), tileset->fileExists ? "Yes" : "No");
    }
    return mrp;
  }

  const Tileset* get_tileset(const fs::path &tilesetPath){
    std::error_code ec1;
    std::error_code ec2;
    auto mapPath = map->documentPath.remove_filename();
    for (auto&& tsx : map->tilesets){
      if (tsx->fileExists){
        auto p1 = fs::absolute(mapPath / tilesetPath, ec1);
        auto p2 = fs::absolute(tsx->tilesetPath, ec2);
        if(p1 == p2 && !ec1 && !ec2){
          return tsx;
        }
      }
    }
    printf("[MapParser]: No tileset found.\n");
    return nullptr;
  }

  const std::string &get_data() const { return data; }

  /** @brief Access the loaded tmx::Map after it's been parsed */
  tmx::Map *map = nullptr;

 private:
  /**
   * @brief Fill our tmx::Map with data.
   *
   * @param path The path where relative paths are looked up from
   * @return true if there were no errors, false otherwise
   */
  bool extract(const char *path) {
    printf("[MapParser]: Extracting map data...\n");
    auto root = xmlDoc.RootElement();
    map = new Map(root, path);
    map->update_data();
    return true;
  }

  /** @brief Needed to extract XML data */
  tinyxml2::XMLDocument xmlDoc;

  std::string data;
};
}  // namespace tmx
