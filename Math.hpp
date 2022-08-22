#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

/**
 * @brief Return the Euclidean length of a 2D vector.
 *
 * @param a The 2D vector whose length we want
 * @return constexpr double --- the length of \a a
 */
constexpr double length(const sf::Vector2f &a) { return sqrt(a.x * a.x + a.y * a.y); };

/**
 * @brief Get the Euclidean distance between 2 2D Vectors.
 *
 * @param a A 2D vector
 * @param b A 2D vector
 * @return double -- the distance between \a a and \a b
 */
double distance(const sf::Vector2f &a, const sf::Vector2f &b) { return length(b - a); };
