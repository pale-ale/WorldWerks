#include "Board.hpp"

/**
 * @brief Fill \a tokens with the tokens inside tokenGroup.
 *
 * @param tokenGroup The layer containing the tokens we want to spawn on the map
 */
void Board::extract_tokens(const tmx::ObjectGroup &tokenGroup) {
  if (!scene){ LOGERR("Board", "Scene may not be a nullptr!"); }
  for (auto object : tokenGroup.objects) {
    sf::Vector2i pos{object->x, object->y};
    auto token = scene->spawn_gamepiece<Token>(object->id, pos, object->name);
    tokens.push_back(token);
    object->nodeData->add_callback([this, object](auto *_){mapDataChangedCallback(object);});
    token->add_callback([this, token, object](auto *_) {
      object->name = token->get_title();
      object->x = token->get_position().x;
      object->y = token->get_position().y;
      object->commit_data();
    });
    object->add_callback([&token, &object](auto *_) {
      token->set_title(object->name);
      token->set_position(sf::Vector2i(object->x, object->y));
      token->send_data_changed();
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
    LOGDBG("Board", fmt::format("\t BG Name: '{}'.", object->name.c_str()));
    sf::Vector2i pos{object->x, object->y};
    backgroundGraphics.push_back({pos, object->id});
  }
}

/**
 * @brief Extract the relevant information and store the different layers.
 */
void Board::parse_map() {
  tokens.clear();
  LOGDBG("Board", "Extracting object groups...");
  for (auto && objGroup : map->objectGroups) {
    if (strcmp(objGroup->name, "Entities") == 0) {
      extract_tokens(*objGroup);
    } else if (strcmp(objGroup->name, "Background") == 0) {
      extract_background(*objGroup);
    } else {
      LOGWRN("Board", fmt::format("Unknown group name: '{}'.", objGroup->name));
    }
  }
}
