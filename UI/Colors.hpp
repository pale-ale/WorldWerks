#pragma once
#include <SFML/Graphics.hpp>

namespace colors{
  struct color {
    unsigned short r = 0;
    unsigned short g = 0;
    unsigned short b = 0;
    unsigned short a = 255;

    operator sf::Color() const{
      return sf::Color(r,g,b,a);
    }
  };

  const color STANDARD_DARK = {25,25,50};
};