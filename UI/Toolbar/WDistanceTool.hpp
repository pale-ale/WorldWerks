#pragma once
#include "../../Util/Math.hpp"
#include "../WLine.hpp"
#include "../WTextbox.hpp"
#include "WTool.hpp"

class WDistanceTool : public WTool {
 public:
  WDistanceTool(UISystem *ui, std::weak_ptr<UIElement> parent) : WTool(ui, parent) {}

  virtual void post_init() override {
    WTool::post_init();
    line = std::make_unique<WLine>(sf::Vector2i{0, 0}, sf::Vector2i{0, 0}, sf::Color::Red,
                                   sf::Color::White);
    Binding<string> distanceBinding(
        [this]() { return std::to_string(distance(line->base, line->tip)); });
    textbox = std::make_unique<WTextbox>(uiSystem, shared_from_this(), distanceBinding,
                                         sf::Vector2i{50, 20});
  }

  virtual bool event_clicked() override {
    inUse = true;
    sprite.setColor({100, 0, 0});
    return true;
  }

  virtual bool event_mouse_down(const sf::Vector2i &mousePos) override {
    if (inUse) {
      line->base = mousePos;
      line->tip = mousePos;
      line->update();
      baseSet = true;
      return true;
    }
    return false;
  }

  virtual bool event_mouse_up(const sf::Vector2i &mousePos) override { return false; }

  virtual void event_mouse_moved(const sf::Vector2i &mousePos) override {
    if (inUse && baseSet) {
      line->tip = mousePos;
      line->update();
      textbox->update_position(-get_parent_position() - relativePosition +
                               (line->base + line->tip) / 2);
      textbox->redraw();
    }
  }

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    WTool::draw(target, states);
    if (inUse && baseSet) {
      target.draw(*line, states);
      target.draw(*textbox, states);
    }
  }

 private:
  std::unique_ptr<WLine> line;
  std::unique_ptr<WTextbox> textbox;
  bool inUse = false;
  bool baseSet = false;
};
