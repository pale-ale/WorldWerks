#include "WBar.hpp"

WBar::WBar(UISystem* uiSystem, std::weak_ptr<UIElement> parentElement,
           GetterFn<int> current, GetterFn<int> max, const sf::Color& fillColor,
           const sf::Color& bgColor, const sf::Vector2i& size, const sf::Vector2i& pos)
    : UIElement(uiSystem, parentElement, "WBar", size, pos),
      current{current},
      max{max},
      fillColor{fillColor},
      bgColor{bgColor} {
  fillShape.setFillColor(fillColor);
  fillShape.setPosition({(float)border, (float)border});
  bgShape.setFillColor(bgColor);
  bgShape.setSize({(float)size.x, (float)size.y});
  rtex = new sf::RenderTexture();
  redraw();
}

/**
 * @brief Set the source of the current value.
 *
 * @param newCurrent
 */
void WBar::set_current(GetterFn<int> newCurrent) {
  current = newCurrent;
  redraw();
}

/**
 * @brief Set the source of the max value.
 *
 * @param newMax
 */
void WBar::set_max(GetterFn<int> newMax) {
  max = newMax;
  redraw();
}

/**
 * @brief Calculate the width of the inner bar, indicating "fullness"
 *
 * @return float --- Width of the new bar in pixels
 */
float WBar::get_fill_width() {
  return (size.x - 2 * border) * ((float)current() / max());
}

/**
 * @brief Redraw this widget with new values applied.
 */
void WBar::redraw() {
  float width = (current && max) ? get_fill_width() : 0;
  fillShape.setSize({(float)width, (float)size.y - 2 * border});
  rtex->create(size.x, size.y);
  rtex->draw(bgShape);
  rtex->draw(fillShape);
  rtex->display();
  sprite.setTexture(rtex->getTexture(), true);
}
