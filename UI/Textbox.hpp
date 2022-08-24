#pragma once

#include <SFML/Graphics.hpp>

#include "UIElement.hpp"

/**
 * @brief Displays some text in a rectangle with a background.
 */
class Textbox : public UIElement {
 public:
  Textbox(UISystem *uiSystem, std::shared_ptr<UIElement> parent,
          std::string *text = nullptr, sf::Vector2i size = {100, 50},
          sf::Vector2i pos = {0, 0});

  /**
   * @brief Set a new address to read/write the contents from.
   *
   * @param target - Address of the contents.
   */
  void set_text_binding(std::string *target);

  /**
   * @brief Update the sprite's texture with the new changes.
   */
  void redraw();

  virtual void event_text_input(const char &input) override {
    if (input == '\b') {
      if (text->length() > 0) text->pop_back();
    } else {
      *text += input;
    }
    redraw();
  }

 private:
  /** @brief The text source */
  std::string *text;
  /** @brief Font needed to draw the text */
  sf::Font font;
  /** @brief Essentially a sf::Texture, needs to be kept in memory */
  sf::Text textImage;
  /** @brief Also needs to be kept in memory to be drawn */
  std::shared_ptr<sf::RenderTexture> rtex;
};
