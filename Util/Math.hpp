#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

/**
 * @brief Return the Euclidean length of a 2D vector.
 *
 * @param a The 2D vector whose length we want
 * @return constexpr double --- the length of \a a
 */
template <typename T = sf::Vector2f>
constexpr auto length(const T &a) { return sqrt(a.x * a.x + a.y * a.y); };

/**
 * @brief Get the Euclidean distance between 2 2D Vectors.
 *
 * @param a A 2D vector
 * @param b A 2D vector
 * @return auto -- the distance between \a a and \a b
 */
template <typename T, typename U>
auto distance(const T &a, const U &b) { return length(b - a); };

/**
 * @brief Scale the vector \a a to be of length \a n, default for \a n is 1.0f.
 *
 * @param a A 2D vector
 * @param n The scalar the vector will be scaled to.
 * @return sf::vector2f --- The vector, scaled to be of length \a n.
 */
template <typename T, typename U = float>
sf::Vector2f normalize(const T &a, const float &n = 1.0f) {
  auto len = length(a);
  return {(float)( (a.x / len) * n ), (float)( (a.y / len) * n )};
}
