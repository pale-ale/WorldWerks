#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <list>

enum InputEventType{
  MouseEvent,
  KeyboardEvent
};

class InputManager{
  typedef std::function<void(const sf::Event&, const sf::Vector2i& mousePos)> EventFunc;
  public:
  void register_callback(EventFunc callback, InputEventType filter);
  void remove_callback(EventFunc callback, InputEventType filter);
  void process_event(const sf::Event &event, const sf::Vector2i &mousePos);

  private:
  std::list<EventFunc> keyboardCallbacks = {};
  std::list<EventFunc> mouseCallbacks = {};
};