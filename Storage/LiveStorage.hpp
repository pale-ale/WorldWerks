#pragma once

#include <map>
#include <string>

#include "../Util/Log.hpp"

using std::string;

class LiveStorage {
 public:
  static inline std::map<string, string> storage = {};

  static string* create_entry(const string& key) {
    if (storage.count(key) > 0) {
      LOGERR("LiveStorage",
             fmt::format("Cannot create storage space: Key '{}' already exists.",
                         key.c_str()));
      return nullptr;
    }
    return &storage[key];
  }
};
