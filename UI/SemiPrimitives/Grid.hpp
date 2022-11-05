#pragma once

#include <SFML/Graphics.hpp>

class GridSemiPrimitive : public sf::Drawable {
 public:
  GridSemiPrimitive(){}
  GridSemiPrimitive(sf::Vector2i lineCount, sf::Vector2i lineSpace, sf::Color lineColor = {0,0,0})
      : lineCount{lineCount}, lineSpace{lineSpace}, lineColor{lineColor} {
    lineArray = calc_lines(lineCount.x, lineCount.y, lineSpace.x, lineSpace.y);
  }

  void setLineCount(const sf::Vector2i& lineCount){
    this->lineCount = lineCount;
    lineArray = calc_lines(lineCount.x, lineCount.y, lineSpace.x, lineSpace.y);
  }
  
  void setLineSpace(const sf::Vector2i& lineSpace){
    this->lineSpace = lineSpace;
    lineArray = calc_lines(lineCount.x, lineCount.y, lineSpace.x, lineSpace.y);
  }
  
  void setLineColor(const sf::Color& lineColor){
    this->lineColor = lineColor;
    lineArray = calc_lines(lineCount.x, lineCount.y, lineSpace.x, lineSpace.y);
  }

  sf::Vector2f offset;

  protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    states.transform.translate(offset);
    target.draw(lineArray, states);
  }
  
  private:
  /**
   * @brief Populate a sf::VertexArray with lines in a grid-wise manner
   * 
   * @param linesX Number of vertical lines, i.e. line count in X-direction
   * @param linesY Number of horizontal lines, i.e. line count in Y-direction
   * @param spaceX Space between vertical lines
   * @param spaceY Space between horizontal lines
   * @return sf::VertexArray The array with vertices that make up the grid
   */
  sf::VertexArray calc_lines(int linesX, int linesY, float spaceX, float spaceY) {
    sf::VertexArray lineVerts(sf::PrimitiveType::Lines, (linesX + linesY) * 2);
    float left = 0, right = linesX * spaceX;
    float top = 0, bottom = linesY * spaceY;
    // Vertical lines, starting from the left
    for (int x = 0; x < linesX * 2; x += 2) {
      lineVerts[x]     = sf::Vertex({spaceX * x, top},    lineColor);
      lineVerts[x + 1] = sf::Vertex({spaceX * x, bottom}, lineColor);
    }
    // Horizontal lines, starting from the top
    for (int y = 0; y < linesY * 2; y += 2) {
      lineVerts[linesX * 2 + y]     = sf::Vertex({left,  y * spaceY}, lineColor);
      lineVerts[linesX * 2 + y + 1] = sf::Vertex({right, y * spaceY}, lineColor);
    }
    return lineVerts;
  }
  
  sf::Color lineColor;
  sf::Vector2i lineCount;
  sf::Vector2i lineSpace;
  sf::VertexArray lineArray;
};