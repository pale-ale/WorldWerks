#pragma once

#include "../3rdParty/tinyxml2.hpp"

/**
 * @brief Requires the parse method, accepting XML data.
 */
struct IParseable {
  /**
   * @brief Parse the XMLElement and save it's values.
   *
   * @param element The XML Node we want to get the data from
   * @return \a true if errors were encountered during parsing, \a false
   * otherwise.
   */
  virtual bool parse(tinyxml2::XMLElement* element) = 0;
};
