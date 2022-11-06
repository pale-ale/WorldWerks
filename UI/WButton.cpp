#include "WButton.hpp"
#include "UISystem.hpp"
#include "WTextbox.hpp"

/** @brief Use this to set the button's text. */
void WButton::set_text(const std::string& newText){
  auto textbox = text.lock();
  if (textbox){
    textbox->set_text_binding({[newText](){return newText;}});
  }
}

void WButton::post_init() {
  UIElement::post_init();
  auto textbox = uiSystem->create_widget<WTextbox>(shared_from_this(), Binding<std::string>(),
                                           size, sf::Vector2i{0, 0}, "ButtonText");
  textbox->bgColor = sf::Color::Transparent;
  textbox->textColor = sf::Color::Black;
  textbox->redraw();
  text = textbox;
}
