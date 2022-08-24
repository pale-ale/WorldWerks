#include "Board.hpp"

/**
 * @brief Fill \a tokens with the tokens inside tokenGroup.
 *
 * @param tokenGroup The layer containing the tokens we want to spawn on the map
 */
void Board::extract_tokens(const tmx::ObjectGroup &tokenGroup) {
  for (auto &&object : tokenGroup.objects) {
    sf::Vector2i pos{object->x, object->y};
    auto &token = tokens.emplace_back(object->id, pos, object->name);
    token.add_callback([this](auto *token) {
      if (this->mapDataChangedCallback) {
        (*this->mapDataChangedCallback)(token);
      }
    });
  }
}

/**
 * @brief Extract the background tile information.
 *
 * @param bgGroup The Layer with the background image objects
 */
void Board::extract_background(const tmx::ObjectGroup &bgGroup) {
  for (auto &&object : bgGroup.objects) {
    printf("\t BG Name: '%s'\n", object->name.c_str());
    sf::Vector2i pos{object->x, object->y};
    backgroundGraphics.push_back({pos, object->id});
  }
}

/**
 * @brief Extract the relevant information and store the different layers.
 */
void Board::parse_map() {
  tokens.clear();
  printf("Object Groups:\n");
  for (auto objGroup : map->objectGroups) {
    if (strcmp(objGroup->name, "Entities") == 0) {
      extract_tokens(*objGroup);
    } else if (strcmp(objGroup->name, "Background") == 0) {
      extract_background(*objGroup);
    } else {
      printf("Unknown group name: '%s'\n", objGroup->name);
    }
  }
}
