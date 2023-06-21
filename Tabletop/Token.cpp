#include "Token.hpp"
#include "../Enums/ERPCTypes.h"

  /**
   * @brief Set the title
   * 
   * @param newTitle 
   */
  void Token::set_title(const std::string &newTitle){
    title = newTitle;
  }

  /**
   * @brief Get the title
   * 
   * @return const std::string& 
   */
  const std::string &Token::get_title() const{
    return title;
  }