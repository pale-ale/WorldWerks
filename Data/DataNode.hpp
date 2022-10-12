#pragma once
#include <fmt/core.h>

#include <map>
#include <memory>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
#include "../Util/Log.hpp"
#include "DataChangeSource.hpp"

using tinyxml2::XMLElement;

class DataNode : public DataChangeSource {
 public:
  DataNode(XMLElement *element, DataNode *parent = nullptr)
      : element{element}, parent{parent} {}
  std::vector<std::unique_ptr<DataNode>> childern = {};

  /**
   * @brief Fill this class's members with data from the stored XMLElement.
   */
  virtual void update_data() = 0;

  /**
   * @brief Write this class's members to the XMLElement.
   */
  virtual void commit_data() = 0;

  /**
   * @brief Obtain data from the data tree.
   * 
   * @param key The key used to search for the according data
   * @param data Where the data will be written to
   * @return true on success, false if data could not be fetched
   */
  virtual bool fetch_data(const std::string &key, std::string &data) {
    if (!parent) {
      return false;
    }
    return parent->fetch_data(key, data);
  }

  /**
   * @brief Get the value of an attribute.
   * 
   * @tparam T The type of the attribute
   * @param fieldName The name of the attribute
   * @param out The attribute data will be found here
   * @param required If not required, this function will always return true
   * @param typeHint Used for debugging, displays the expected type
   * @return true on success, false if attribute missing or of wrong type
   */
  template <typename T>
  bool get_attribute(std::string fieldName, T *out, bool required = false,
                     const char *typeHint = "<Add a type hint!>") {
    auto ret = element->QueryAttribute(fieldName.c_str(), out);
    bool success = ret == tinyxml2::XML_SUCCESS;
    if (success || !required) {
      return success;
    }
    auto err = "";
    switch (ret) {
      case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
        err = "Wrong field type ({} required).", typeHint;
        break;
      case tinyxml2::XML_NO_ATTRIBUTE:
        err = "No such attribute.";
        break;

      default:
        break;
    }
    LOGERR("DataNode", fmt::format("Error reading required attribute '{}': {}.",
                                   fieldName.c_str(), err));
    return false;
  }

  /**
   * @brief Set the value of an attribute. Creates it if it does not already exist.
   *
   * @tparam T Supported types: int, const char*, float, and some other basic types
   * @param fieldName The name of the attribute we want to set/create
   * @param value The value the attribute will receive
   */
  template <typename T>
  void set_attribute(std::string fieldName, const T &value) {
    element->SetAttribute(fieldName.c_str(), value);
  }

 protected:
  /** @brief Keep a ptr to the parent */
  DataNode *parent = nullptr;

  /** @brief Allows editing the XML data to save it */
  XMLElement *element = nullptr;
};
