#pragma once
#include <SFML/Graphics.hpp>

/**
 * @brief Tokens represent entities that can be directly targeted, moved, or interacted
 * with. This usually includes characters or certain objects like traps, quest
 * objectives, or other enemies.
 */
class Token {
 public:
  Token(int id, sf::Vector2i pos, std::string title)
      : tokenID{id}, position{pos}, title{title} {}
  Token() = delete;
  Token(Token&) = delete;
  Token(const Token&) = delete;

  /** @brief The position on the map in Tiled units */
  sf::Vector2i position;

  /** @brief The title given the token */
  std::string title;

  /** @brief The ID of this token */
  int tokenID;
};
