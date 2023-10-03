#pragma once
#include <SFML/Graphics.hpp>
#include "../Data/DataChangeSource.hpp"
#include "../TooDeeEngine/Scene/GamePiece.hpp"

/**
 * @brief Tokens represent entities that can be directly targeted, moved, or interacted
 * with. This usually includes characters or certain objects like traps, quest
 * objectives, or other enemies.
 */
class Token : public GamePiece, public DataChangeSource {
 public:
  Token(SceneBase* scene, int id, sf::Vector2i pos, std::string title)
      : GamePiece(scene), DataChangeSource(), tokenID{id}, title{title} {name = title; set_position(pos);}
  Token() = delete;
  Token(Token&) = delete;
  Token(const Token&) = delete;

  void set_title(const std::string &newTitle);
  const std::string &get_title() const;

 private:
  
  /** @brief The title given the token */
  std::string title;

  /** @brief The ID of this token */
  int tokenID;
};
