#include "InputManager.hpp"

void InputManager::register_callback(EventFunc callback,
                                     InputEventType filter) {
  switch (filter) {
    case InputEventType::KeyboardEvent:
      keyboardCallbacks.push_back(callback);
      break;
    case InputEventType::MouseEvent:
      mouseCallbacks.push_back(callback);
      break;

    default:
      printf("Inputmanager callback register failed. How'd you even do that?");
      break;
  }
}

void InputManager::remove_callback(EventFunc callback, InputEventType filter) {
  printf("Not yet implemented!");
}

void InputManager::process_event(const sf::Event &event, const sf::Vector2i &mousePos) {
  bool bIsKbEvent;
  bool bIsMouseEvent;
  switch (event.type) {
    case sf::Event::EventType::KeyPressed:
      bIsKbEvent = true;
      break;
    case sf::Event::EventType::KeyReleased:
      bIsKbEvent = true;
      break;

    case sf::Event::EventType::MouseMoved:
      bIsMouseEvent = true;
      break;
    case sf::Event::EventType::MouseButtonPressed:
      bIsMouseEvent = true;
      break;
    case sf::Event::EventType::MouseButtonReleased:
      bIsMouseEvent = true;
      break;
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
