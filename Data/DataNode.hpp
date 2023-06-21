#pragma once
#include <fmt/core.h>

#include <map>
#include <memory>
#include <vector>

#include "../TooDeeEngine/3rdParty/tinyxml2.hpp"
#include "../TooDeeEngine/Util/Log.hpp"
#include "DataChangeSource.hpp"

using tinyxml2::XMLElement;

class NodeData : public DataChangeSource {
  public:
  NodeData(XMLElement *element) : element{element}{}

  /** @brief Allows editing the XML data to save it */
  XMLElement *element = nullptr;
};

/** 
 * @brief This class acts as a storage and access class for XML data.
 * It allows listeners for events like changes, and exposes functions
 * to read/write the data it contains.
 */

class DataNode : public DataChangeSource {
 public:
  DataNode(XMLElement *element, DataNode *parent = nullptr)
      : nodeData{std::make_unique<NodeData>(element)}, parent{parent} {}
  std::vector<DataNode *> children = {};

  /**
   * @brief Fill this class's members with data from the stored XMLElement.
   */
  virtual void update_data() = 0;

  /**
   * @brief Write this class's members to the XMLElement.
   */
  virtual void commit_data() = 0;

  std::string get_data() {
    tinyxml2::XMLPrinter printer;
    nodeData->element->Accept(&printer);
    return printer.CStr();
  }

  std::vector<size_t> get_hierarchy() {
    if (!parent) {
      return {};
    }
    auto hierarchy = parent->get_hierarchy();
    auto elem = std::find_if(parent->children.begin(), parent->children.end(),
                             [this](DataNode *dn) { return dn == this; });
    size_t idx = elem - parent->children.begin();
    if (idx >= parent->children.size()) {
      LOGERR("DataNode", "Node not found in registered parent.");
    }
    hierarchy.push_back(idx);
    return hierarchy;
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
    auto ret = nodeData->element->QueryAttribute(fieldName.c_str(), out);
    bool success = ret == tinyxml2::XML_SUCCESS;
    if (success || !required) {
      return success;
    }
    auto err = "";
    switch (ret) {
      case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
        err = "Wrong field type ({} required)", typeHint;
        break;
      case tinyxml2::XML_NO_ATTRIBUTE:
        err = "No such attribute";
        break;
      default:
        err = "Unknown error";
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
    nodeData->element->SetAttribute(fieldName.c_str(), value);
  }

  /** @brief Allows editing the XML data to save it */
  std::unique_ptr<NodeData> nodeData = nullptr;

  /** @brief Keep a ptr to the parent */
  DataNode *parent = nullptr;
};
