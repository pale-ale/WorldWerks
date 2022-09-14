#pragma once
#include <map>
#include <memory>
#include <vector>

#include "../3rdParty/tinyxml2.hpp"
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

  // return true on error
  virtual bool fetch_data(const std::string& key, std::string& data){
    if (!parent){
      return true;
    }
    return parent->fetch_data(key, data);
  }

  /**
   * @brief Get the value of an attribute.
   *
   * @return true: Success --- false: Missing or wrong field
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
        err = "Wrong field type (%s required).", typeHint;
        break;
      case tinyxml2::XML_NO_ATTRIBUTE:
        err = "No such attribute.";
        break;

      default:
        break;
    }
    printf("[DataNode]: Error reading required attribute '%s': %s\n", fieldName.c_str(),
           err);
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
