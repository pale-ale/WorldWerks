#include "Token.hpp"

/**
   * @brief Set the position
   * 
   * @param newPosition 
   */
  void Token::set_position(const sf::Vector2i &newPosition){
    position = newPosition;
    send_data_changed();
  }

  /**
   * @brief Get the position
   * 
   * @return const sf::Vector2i& 
   */
  const sf::Vector2i &Token::get_position() const{
    return position;
  }

  /**
   * @brief Set the title
   * 
   * @param newTitle 
   */
  void Token::set_title(const std::string &newTitle){
    title = newTitle;
    send_data_changed();
  }

  /**
   * @brief Get the title
   * 
   * @return const std::string& 
   */
  const std::string &Token::get_title() const{
    return title;
  }