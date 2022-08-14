#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

constexpr double length(const sf::Vector2f &a) {
  return sqrt(a.x*a.x + a.y*a.y);
};

double distance(const sf::Vector2f &a, const sf::Vector2f &b) {
  return length(b-a);
};
