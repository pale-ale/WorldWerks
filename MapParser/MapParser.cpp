#include "MapParser.hpp"

namespace fs = std::filesystem;
namespace tmx {
  /**
   * @brief Parse a file into a tmx::Map.
   *
   * @param path The path to the file we want to read
   * @return true if loading/parsing of the file was succesful, false otherwise
   */
  bool MapParser::load_file(const std::string &path) {
    LOGINF("MapParser", fmt::format("Loading file '{}'.", path.c_str()));
    std::ifstream is(path);
    std::stringstream buffer;
    buffer << is.rdbuf();
    data = buffer.str();
    auto err = xmlDoc.LoadFile(path.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      LOGERR("MapParser",
             fmt::format("Error reading XML Document: {}.", xmlDoc.ErrorStr()));
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
  bool MapParser::load_text(const std::string &text, const char *mapPath) {
    data = text;
    auto err = xmlDoc.Parse(text.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      LOGERR("MapParser",
             fmt::format("Error reading XML Document: {}.", xmlDoc.ErrorStr()));
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
  bool MapParser::save_file(const char *savePath) {
    auto err = xmlDoc.SaveFile(savePath);
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
      LOGERR("MapParser",
             fmt::format("Error saving XML Document: {}.", xmlDoc.ErrorStr()));
      return false;
    }
    LOGINF("MapParser", "Saved.");
    return true;
  }

  std::vector<std::string> MapParser::get_missing_resource_paths() {
    std::vector<std::string> mrp;
    LOGDBG("MapParser", "Tilesets:");
    for (auto &&tileset : map->tilesets) {
      if (!tileset->fileExists) {
        mrp.push_back(tileset->relativeTilesetPath);
      }
      LOGDBG("MapParser", fmt::format("\tTSX Name: '{}', Path: '{}', Exists: '{}'",
                                      tileset->name.c_str(), tileset->relativeTilesetPath.c_str(),
                                      tileset->fileExists ? "Yes" : "No"));
    }
    return mrp;
  }

  const Tileset *MapParser::get_tileset(const fs::path &tilesetPath) {
    std::error_code ec1;
    std::error_code ec2;
    auto mapPath = map->documentPath.remove_filename();
    for (auto &&tsx : map->tilesets) {
      if (tsx->fileExists) {
        auto p1 = fs::absolute(mapPath / tilesetPath, ec1);
        auto p2 = fs::absolute(tsx->tilesetPath, ec2);
        if (p1 == p2 && !ec1 && !ec2) {
          return tsx.get();
        }
      }
    }
    LOGWRN("MapParser",
           fmt::format("Tileset at '{}' not found.", (mapPath / tilesetPath).c_str()));
    return nullptr;
  }

  /**
   * @brief Fill our tmx::Map with data.
   *
   * @param path The path where relative paths are looked up from
   * @return true if there were no errors, false otherwise
   */
  bool MapParser::extract(const char *path) {
    LOGINF("MapParser", "Extracting map data...");
    auto root = xmlDoc.RootElement();
    map = std::move(std::make_unique<Map>(root, path));
    map->update_data();
    return true;
  }
} // namespace tmx
