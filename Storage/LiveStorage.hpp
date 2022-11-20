#pragma once

#include <fstream>
#include <map>
#include <string>

#include "../3rdParty/tinyxml2.hpp"
#include "../Util/Log.hpp"
#include "functional"

using std::string;
using std::vector;

enum class EStorageElementState : uint8_t {
  MISSING,
  LOCAL_READY,
  REMOTE_READY,
  REMOTE_REQUESTED,
};

typedef std::function<void(const std::string&)> StringFunction;
typedef std::function<EStorageElementState(const std::string&)> ResourceRequestFunction;

struct StorageElement {
  string data;
  EStorageElementState state;
  vector<StringFunction> updateListeners;
};

struct LiveDataCapsule {
  string key;
  string data;
  string to_msg();
  void from_msg(string str);
};

class LiveStorage {
 public:
  static inline std::map<string, StorageElement> storage = {};
  static inline bool useLocalFiles = false;
  static inline ResourceRequestFunction missingResourceHandler = {};

  static inline bool key_exists(const string& key) { return storage.count(key) > 0; }

  static string* create_entry(const string& key,
                              EStorageElementState state = EStorageElementState::MISSING,
                              vector<StringFunction> callbacks = {});
  static void read_file_to_storage(const char* file, const string& storageKey);
  static void request_resource_update(const string& storageKey);
  static bool insert_data_capsule(LiveDataCapsule capsule);
  static bool retrieve(const string& key, string& outData);
  static EStorageElementState get_state(const string& key) {
    return key_exists(key) ? storage[key].state : EStorageElementState::MISSING;
  }
};
