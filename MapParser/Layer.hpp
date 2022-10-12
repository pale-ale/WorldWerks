#pragma once
#include <sstream>

#include "../Data/DataNode.hpp"

namespace tmx {
/**
 * @brief Layers are built like a grid and contain Tiles.
 */
struct Layer : DataNode {
  Layer(tinyxml2::XMLElement * element, DataNode * parent) : DataNode(element, parent) {}

  virtual void update_data() override {
    bool errors = false;
    errors |= (bool)element->QueryAttribute("id", &id);
    errors |= (bool)element->QueryAttribute("width", &width);
    errors |= (bool)element->QueryAttribute("height", &height);
    errors |= (bool)element->QueryAttribute("name", &name);
    auto child = element->FirstChildElement("data");
    errors |= child == nullptr;

    if (errors){
      LOGERR("Layer", "Could not read layer information.");
      return;
    }

    // Split data at ',' and insert into easy-to-use std::vector<int>
    std::string tileId;
    std::stringstream dataStream(child->GetText());
    while (getline(dataStream, tileId, ',')) {
      data.push_back(std::stoi(tileId));
    }
  }

  virtual void commit_data() {}

  /**
   * @brief Get the tileID of the tile at \a [x,y].
   *
   * @param x The x-coordinate
   * @param y The y-coordinate
   * @return int --- The tileID of the tile at \a [x,y]
   */
  int get_at(int x, int y) { return data[y * width + x]; }

  int id;                /** @brief The id of this layer */
  int width, height;     /** @brief The dimensions of this layer, in no. of tiles */
  const char* name;      /** @brief The name of this layer */
  std::vector<int> data; /** @brief A vector storing TileIDs of this layer */
};
}  // namespace tmx
