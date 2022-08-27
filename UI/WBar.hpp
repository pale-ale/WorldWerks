#pragma once
#include "UIBinding.hpp"
#include "UIElement.hpp"

/**
 * @brief This Widgets displays a percentage as a bar.
 *
 */
class WBar : public UIElement {
 public:
  WBar(UISystem* uiSystem, std::weak_ptr<UIElement> parentElement, GetterFn<int> current,
       GetterFn<int> max, const sf::Color& fillColor, const sf::Color& bgColor,
       const sf::Vector2i& size, const sf::Vector2i& pos);
  void set_current(GetterFn<int> newCurrent);
  void set_max(GetterFn<int> newMax) ;

  /** @brief The color of the outer rect */
  sf::Color bgColor;

  /** @brief The color of the inner rect */
  sf::Color fillColor;

 private:
  float get_fill_width();
  void redraw();

  /** @brief Keep a ptr to the RenderTexture in memory */
  sf::RenderTexture* rtex;

  /** @brief The width of the border */
  int border = 2;

  /** @brief The inner shape */
  sf::RectangleShape fillShape;

  /** @brief The outer shape */
  sf::RectangleShape bgShape;

  /** @brief The source of the current value */
  GetterFn<int> current;

  /** @brief The source of the maximum value */
  GetterFn<int> max;
};
