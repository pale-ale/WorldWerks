#pragma once

/**
 * @brief Contains the remaining actions a unit may take this turn.
 * 
 */
struct ActionInfo{
  /**
   * @brief Standard actions are usually limited to 1 per turn.
   * Typical standard actions are: Swinging a sword, firing an arrow, casting a spell, interacting with objects, ...
   */
  int standardActions = 0;

  /**
   * @brief Move actions are used when a chracter moves. A character can usually take up to 2 per turn.
   * Typical move actions are: Walking/running, climbing a ladder or scaling other objects.
   */
  int moveActions = 0;

  /**
   * @brief These are not limited to a number per turn, but may prokove attacks of opportunity etc.
   */
  int freeActions = 0;

  /**
   * @brief Same as free actions, except they do not provoke anything and will trigger before any other effects.
   */
  int immediateActions = 0;
};
