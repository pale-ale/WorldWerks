#include "LiveStorage.hpp"
#include "../3rdParty/base64.h"

/**
 * @brief Get the string representation of this capsule's data.
 *
 * @return const char* --- The data in the form of an xml string
 */
const char* LiveDataCapsule::to_msg() {
  tinyxml2::XMLDocument doc;
  auto elem = doc.NewElement("FileData");
  elem->SetAttribute("Key", key.c_str());
  std::string&& encoded = macaron::Base64::Encode(data).c_str();
  elem->SetAttribute("Data", encoded.c_str());
  doc.InsertFirstChild(elem);
  auto printer = new tinyxml2::XMLPrinter();
  doc.Print(printer);
  return printer->CStr();
}

/**
 * @brief Fill this capsule with data from the supplied string.
 *
 * @param str The xml data to fill this capsule with
 */
void LiveDataCapsule::from_msg(string str) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(str.c_str()) != tinyxml2::XMLError::XML_SUCCESS) {
    LOGERR("LiveStorage", fmt::format("Error parsing resource data: {}", doc.ErrorStr()));
    return;
  }
  auto elem = doc.FirstChildElement("FileData");
  if (!elem) {
    LOGERR("LiveStorage", "Error parsing resource data.");
    return;
  }
  const char* keyStr = "";
  const char* dataStr = "";
  elem->QueryAttribute("Key", &keyStr);
  elem->QueryAttribute("Data", &dataStr);
  if (strcmp(keyStr, "") == 0 && strcmp(dataStr, "") == 0) {
    LOGWRN("LiveStorage", "Received a file missing the storage key or data.");
    return;
  }
  key = keyStr;
  macaron::Base64::Decode(dataStr, data);
}

/**
 * @brief Create a new entry to store a resource and manage it via network.
 *
 * @param key How to access the resource
 * @param state Whether the resource's origin is local and whether it is available or has
 * been requested
 * @param callback Called whenever the data for the entry changes
 * @return string* A writable reference to the data. Will not call the callback when
 * edited.
 */
string* LiveStorage::create_entry(const string& key, EStorageElementState state,
                                  vector<StringFunction> callbacks) {
  if (storage.count(key) > 0) {
    LOGERR("LiveStorage",
           fmt::format("Cannot create storage space: Key '{}' already exists.",
                       key.c_str()));
    return nullptr;
  }
  auto&& [elemData, elemState, elemCallbacks] = storage[key];
  elemState = state;
  elemCallbacks = callbacks;
  LOGINF("LiveStorage", fmt::format("Created key '{}'.", key));
  return &elemData;
}

/**
 * @brief Convenience method to quickly open, read, and store a file.
 *
 * @param file The path to the file
 * @param storageKey The key used to store it's contents
 */
void LiveStorage::read_file_to_storage(const char* file, const std::string& storageKey) {
  LOGINF("Storage", fmt::format("Reading file '{}' to '{}'.", file, storageKey));
  std::ifstream t(file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  *LiveStorage::create_entry(storageKey, EStorageElementState::LOCAL_READY) =
      buffer.str();
}

void LiveStorage::request_resource_update(const std::string& storageKey){
  if (key_exists(storageKey) && missingResourceHandler){
    missingResourceHandler(storageKey);
  } else {
    LOGERR("LiveStorage", fmt::format("Requested update of resource '{}' which does not exist.", storageKey));
  }
}


/**
 * @brief Add the key and data of a LiveDataCapsule into storage.
 *
 * @param capsule The capsule we want to add
 * @return true if the capsule was added, false on error
 */
bool LiveStorage::insert_data_capsule(LiveDataCapsule capsule) {
  auto&& key = capsule.key;
  if (!key_exists(key)) {
    LOGERR("LiveStorage",
           fmt::format("Cannot insert data capsule for nonexistent key '{}'.", key));
    return false;
  }
  auto&& [elemData, elemState, elemCallbacks] = storage[capsule.key];
  elemData = capsule.data;
  elemState = EStorageElementState::REMOTE_READY;
  for (auto&& elemCallback : elemCallbacks) {
    elemCallback(elemData);
  }
  return true;
}

/**
 * @brief Get the data of the associated key. If the key exists but the data is missing,
 * call the missing resource handler with the key as parameter to e.g. request it from a
 * server.
 *
 * @param key Needed to access the data
 * @param outData Where to write the data to
 * @return true if the key was found and the data is ready, false otherwise
 */
bool LiveStorage::retrieve(const string& key, string& outData) {
  LOGINF("LiveStorage", fmt::format("Looking up key '{}'...", key));
  if (!key_exists(key)) {
    LOGWRN("LiveStorage", fmt::format("Couldn't find key '{}'.", key));
    return false;
  }
  auto&& [data, state, _] = storage[key];
  switch (state) {
    case EStorageElementState::MISSING:
      if (missingResourceHandler) {
        LOGWRN("LiveStorage", fmt::format("Requesting '{}' from the server.", key));
        missingResourceHandler(key);
        state = EStorageElementState::REMOTE_REQUESTED;
      }
      /* Request the storage element from the server. */
      break;

    case EStorageElementState::REMOTE_REQUESTED:
      /* Do nothing; wait for it to be received. */
      break;

    case EStorageElementState::REMOTE_READY:
    case EStorageElementState::LOCAL_READY:
      LOGINF("LiveStorage", fmt::format("Found key '{}'.", key));
      outData = data;
      return true;

    default:
      return false;
  }
  return false;
}
