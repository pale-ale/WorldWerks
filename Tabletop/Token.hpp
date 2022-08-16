#pragma once
#include <SFML/Graphics.hpp>

/*
  Tokens represent entities that can be directly targeted, moved, or interacted
  with. This usually includes characters or certain objects like traps, quest
  objectives, or other enemies.
*/
class Token {
 public:
  Token(int id, sf::Vector2f pos, std::string title) : tokenID{id}, gridPosition{pos}, title{title} {}

  sf::Vector2f gridPosition;
  std::string title;
  int tokenID;
};
