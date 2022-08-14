#pragma once
#include <SFML/Graphics.hpp>

class Token{
 public:
  Token(int id, sf::Vector2f pos):tokenID{id}, gridPosition{pos}{}

  sf::Vector2f gridPosition;
  int tokenID;
};
