#pragma once

#include <list>

#include "../MapParser/Map.hpp"
#include "Token.hpp"

/**
 * @brief Manages the different maps and facilitates switching between them.
 */
class Board {
 public:
  /**
   * @brief Construct a board and extract certain entities from the tmx::Map.
   *
   * @param map Contains info like backgrounds, entities
   */
  Board(tmx::Map *map) : map{map} { parse_map(); }

  /**
   * @brief Return the tmx::Map used by this board.
   * @return tmx::Map* --- The stored map pointer
   */
  tmx::Map *get_map() const { return map; }

  /** @brief Every token present on the loaded map */
  std::list<Token> tokens = {};

  /** @brief Tiles on the background layer */
  std::vector<std::tuple<sf::Vector2i, int>> backgroundGraphics;

  /** @brief The folder the tmx::Map's file resides in */
  std::string mapFolder;

 private:
  /**
   * @brief Fill \a tokens with the tokens inside tokenGroup.
   *
   * @param tokenGroup The layer containing the tokens we want to spawn on the map
   */
  void extract_tokens(const tmx::ObjectGroup &tokenGroup) {
    for (auto &&object : tokenGroup.objects) {
      printf("\t Token Name: '%s'\n", object->name.c_str());
      sf::Vector2i pos{object->x, object->y};
      tokens.push_back(Token(object->id, pos, object->name));
    }
  }

  /**
   * @brief Extract the background tile information.
   *
   * @param bgGroup The Layer with the background image objects
   */
  void extract_background(const tmx::ObjectGroup &bgGroup) {
    for (auto &&object : bgGroup.objects) {
      printf("\t BG Name: '%s'\n", object->name.c_str());
      sf::Vector2i pos{object->x, object->y};
      backgroundGraphics.push_back({pos, object->id});
    }
  }

  /**
   * @brief Extract the relevant information and store the different layers.
   */
  void parse_map() {
    tokens.clear();
    printf("Object Groups:\n");
    for (auto objGroup : map->objectGroups) {
      if (strcmp(objGroup->name, "Entities") == 0) {
        extract_tokens(*objGroup);
      } else if (strcmp(objGroup->name, "Background") == 0) {
        extract_background(*objGroup);
      } else {
        printf("Group name: '%s'\n", objGroup->name);
      }
    }
  }

  /** @brief Keep a pointer to the map, so that we can use it later as well */
  tmx::Map *map;
};
