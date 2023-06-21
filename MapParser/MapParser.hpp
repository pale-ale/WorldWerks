#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "../TooDeeEngine/3rdParty/tinyxml2.hpp"
#include "Map.hpp"

namespace fs = std::filesystem;
namespace tmx {

/**
 * @brief Use this class to build a tmx::Map out of a file or a string.
 */
class MapParser {
 public:
  bool load_file(const std::string &path);
  bool load_text(const std::string &text, const char *mapPath = "");
  bool save_file(const char *savePath);
  std::vector<std::string> get_missing_resource_paths();
  const Tileset *get_tileset(const fs::path &tilesetPath);
  const std::string &get_data() const { return data; }

  /** @brief Access the loaded tmx::Map after it's been parsed */
  std::unique_ptr<tmx::Map> map = nullptr;

 private:
  bool extract(const char *path);

  /** @brief Needed to extract XML data */
  tinyxml2::XMLDocument xmlDoc;

  /** @brief Keep the data for easier access when saving **/
  std::string data;
};
}  // namespace tmx
