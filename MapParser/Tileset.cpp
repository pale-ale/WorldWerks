#include "Tileset.hpp"
#include "../Storage/LiveStorage.hpp"

void tmx::Tileset::update_data() {
    // Open the tsx and read the tileset data.
    LOGINF("Tileset", "Updating data...");
    tinyxml2::XMLDocument xmlDoc;
    auto reloadFunc = [this](auto s){this->update_data();};
    if (!LiveStorage::key_exists(relativeTilesetPath)){
      LOGINF("Tileset", fmt::format("Creating key for tileset '{}'.", relativeTilesetPath.c_str()));
      LiveStorage::create_entry(relativeTilesetPath, EStorageElementState::MISSING, {reloadFunc});
      LOGINF("Tileset", fmt::format("Requesting tileset '{}'...", relativeTilesetPath.c_str()));
      LiveStorage::retrieve(relativeTilesetPath, data);
      return;
    }
    if (!LiveStorage::retrieve(relativeTilesetPath, data)){
      LOGINF("Tileset", fmt::format("Tileset key exists but data missing: '{}'.",
                                    relativeTilesetPath.c_str()));
      return;
    }
    tilesetPath = mapPath.remove_filename() / relativeTilesetPath;
    LOGINF("Tileset", fmt::format("Loading tileset file at '{}'.", tilesetPath.c_str()));

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
      
      auto imagePath = tilesetFolder / relativeImagePath;
      if (!LiveStorage::key_exists(relativeImagePath)){
        if (LiveStorage::useLocalFiles){
          LiveStorage::read_file_to_storage(imagePath.c_str(), relativeImagePath);
        } else {
          LiveStorage::create_entry(relativeImagePath, EStorageElementState::MISSING, {reloadFunc});
          LiveStorage::request_resource_update(relativeImagePath);
        }
        return;
      }
    }
    if (errors) {
      LOGERR("Tileset",
             fmt::format("Errors in tileset file at '{}'.", relativeTilesetPath.c_str()));
      return;
    }
    LOGINF("Tileset", "Loaded successfully.");
  }
