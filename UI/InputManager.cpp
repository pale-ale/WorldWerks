#include "InputManager.hpp"

#include "../Util/Log.hpp"

void InputManager::register_callback(EventFunc callback, InputEventType filter) {
  switch (filter) {
    case InputEventType::KeyboardEvent:
      keyboardCallbacks.push_back(callback);
      break;
    case InputEventType::MouseEvent:
      mouseCallbacks.push_back(callback);
      break;

    default:
      LOGERR("InputManager", "Cannot register callback: Unknown callback type.");
      break;
  }
}

void InputManager::remove_callback(EventFunc callback, InputEventType filter) {
  LOGWRN("InputManager", "remove_callback() is not yet implemented!");
}

void InputManager::process_event(const sf::Event &event, const sf::Vector2i &mousePos) {
  bool bIsKbEvent;
  bool bIsMouseEvent;
  switch (event.type) {
    case sf::Event::EventType::KeyPressed:
    case sf::Event::EventType::KeyReleased:
    case sf::Event::EventType::TextEntered:
      bIsKbEvent = true;
      break;

    case sf::Event::EventType::MouseMoved:
    case sf::Event::EventType::MouseButtonPressed:
    case sf::Event::EventType::MouseButtonReleased:
    case sf::Event::EventType::MouseWheelScrolled:
      bIsMouseEvent = true;
      break;

    default:
      break;
  }
  if (bIsKbEvent) {
    for (auto &&callback : keyboardCallbacks) {
      callback(event, mousePos);
    }
  } else if (bIsMouseEvent) {
    for (auto &&callback : mouseCallbacks) {
      callback(event, mousePos);
    }
  }
}
