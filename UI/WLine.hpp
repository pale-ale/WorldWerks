#pragma once
#include "../Util/Math.hpp"
#include "SFML/Graphics.hpp"

/**
 * @brief Draws a line betwen two 2D points. Can be configured to be dashed with different
 * colors.
 */
class WLine : public sf::Drawable {
 public:
  WLine(sf::Vector2i base, sf::Vector2i tip, sf::Color color1, sf::Color color2)
      : base{base}, tip{tip}, color1{color1}, color2{color2}{
    float r = 6.0f;
    circleBase.setFillColor(color1);
    circleBase.setRadius(r);
    circleBase.setOrigin(r,r);
    circleBase.setPosition({(float)base.x, (float)base.y});
    circleTip.setFillColor(color2);
    circleTip.setRadius(r);
    circleTip.setOrigin(r,r);
    circleTip.setPosition({(float)tip.x, (float)tip.y});
    update();
  }

  /**
   * @brief Call this after making a change to one of the line's properties to redraw it.
   */
  void update() {
    circleBase.setPosition({(float)base.x, (float)base.y});
    circleTip.setPosition({(float)tip.x, (float)tip.y});

    int segmentOpaqueLength = 6;
    int segmentTransparentLength = 4;
    int segmentLength = segmentOpaqueLength + segmentTransparentLength;
    sf::Vector2i delta = tip - base;
    double arrowLength = length(delta);
    int segmentCount = arrowLength / segmentLength;
    segments = sf::VertexArray(sf::PrimitiveType::Lines, segmentCount*4);
    sf::Vector2f opaqueVector = normalize(tip - base, segmentOpaqueLength);
    sf::Vector2f transparentVector = normalize(tip - base, segmentTransparentLength);
    sf::Vector2f segmentStart;
    sf::Vector2f offsetDirection = {(float)delta.y, (float)delta.x};
    // The outer loop draws 2 lines next to each other to make the line 2 px wide.
    for (int i = -1; i < 1; ++i) {
      segmentStart = sf::Vector2f{(float)base.x, (float)base.y} + normalize(offsetDirection, i);
      for (int j = 0; j < segmentCount; ++j) {
        segments.append({segmentStart, color1});
        segmentStart += opaqueVector;
        segments.append({segmentStart, color1});
        segments.append({segmentStart, color2});
        segmentStart += transparentVector;
        segments.append({segmentStart, color2});
      }
    }
  }

  /** @brief The start of the line. */
  sf::Vector2i base;

  /** @brief The end of the line. */
  sf::Vector2i tip;

  /** @brief The color of the first half of each segment, i.e. the color the line starts
   * with. */
  sf::Color color1;

  /** @brief The color of the second half of each segment. */
  sf::Color color2;

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(segments);
    target.draw(circleBase);
    target.draw(circleTip);
  }

 private:
  /** @brief The segments needed for drawing. */
  sf::VertexArray segments;

  /** @brief The circles at both ends of the line. */
  sf::CircleShape circleBase;
  sf::CircleShape circleTip;
};
