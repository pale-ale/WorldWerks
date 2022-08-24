#pragma once
#include <SFML/Graphics.hpp>
#include "../Data/DataChangeSource.hpp"

/**
 * @brief Tokens represent entities that can be directly targeted, moved, or interacted
 * with. This usually includes characters or certain objects like traps, quest
 * objectives, or other enemies.
 */
class Token : public DataChangeSource {
 public:
  Token(int id, sf::Vector2i pos, std::string title)
      : tokenID{id}, position{pos}, title{title} {}
  Token() = delete;
  Token(Token&) = delete;
  Token(const Token&) = delete;

  /**
   * @brief Set the new position
   * 
   * @param newPosition 
   */
  void set_position(const sf::Vector2i &newPosition){
    position = newPosition;
    send_data_changed();
  }

  /**
   * @brief Get the position
   * 
   * @return const sf::Vector2i& 
   */
  const sf::Vector2i &get_position() const{
    return position;
  }

  /**
   * @brief Set the title
   * 
   * @param newTitle 
   */
  void set_title(const std::string &newTitle){
    title = newTitle;
    send_data_changed();
  }

  /**
   * @brief Get the title
   * 
   * @return const std::string& 
   */
  const std::string &get_title() const{
    return title;
  }

 private:
  /** @brief The position on the map in Tiled units */
  sf::Vector2i position;

  /** @brief The title given the token */
  std::string title;

  /** @brief The ID of this token */
  int tokenID;
};
