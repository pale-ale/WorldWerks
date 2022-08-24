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
   * @param map Contains info like backgrounds, entities.
   */
  Board(tmx::Map *map) : map{map} { parse_map(); }

  /**
   * @brief Return the tmx::Map used by this board.
   * 
   * @return tmx::Map* --- The stored map pointer
   */
  tmx::Map *get_map() const { return map; }

  /** @brief Every token present on the loaded map */
  std::list<Token> tokens = {};

  /** @brief Tiles on the background layer */
  std::vector<std::tuple<sf::Vector2i, int>> backgroundGraphics;

  /** @brief The folder the tmx::Map's file resides in */
  std::string mapFolder;

  /** @brief Callback used to process changes to the map */
  DataChangeCallback* mapDataChangedCallback = nullptr;

 private:
  void extract_tokens(const tmx::ObjectGroup &tokenGroup);
  void extract_background(const tmx::ObjectGroup &bgGroup);
  void parse_map();

  /** @brief Keep a pointer to the map, so that we can use it later as well */
  tmx::Map *map;
};
