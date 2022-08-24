#pragma once

#include <SFML/Graphics.hpp>

#include "UIElement.hpp"
#include <functional>

typedef std::function<std::string()> StrGetter;
typedef std::function<void (const std::string &)> StrSetter;
struct StrBinding {StrGetter get; StrSetter set;};

/**
 * @brief Displays some text in a rectangle with a background.
 */
class Textbox : public UIElement {
 public:
  Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
          StrBinding binding, sf::Vector2i size = {100, 50},
          sf::Vector2i pos = {0, 0});
  void redraw();
  void set_text_binding(StrBinding newBinding);
  virtual void event_text_input(const char &input) override;

 private:
  /** @brief The text source */
  StrBinding binding;
  /** @brief Font needed to draw the text */
  sf::Font font;
  /** @brief Essentially a sf::Texture, needs to be kept in memory */
  sf::Text textImage;
  /** @brief Also needs to be kept in memory to be drawn */
  std::shared_ptr<sf::RenderTexture> rtex;
};
