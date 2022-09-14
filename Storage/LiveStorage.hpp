#pragma once

#include <cstdio>
#include <filesystem>
#include <map>
#include <string>

namespace fs = std::filesystem;
using std::string;

class LiveStorage {
 public:
  static inline std::map<string, string> storage = {};

  static string* create_entry(const string& key) {
    if (storage.count(key) > 0) {
      printf("[Storage]: Cannot create storage space: Key '%s' already exists.\n",
             key.c_str());
      
      return nullptr;
    }
    return &storage[key];
  }
};
