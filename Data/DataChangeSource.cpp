#include "DataChangeSource.hpp"

#include "../TooDeeEngine/Util/Log.hpp"

/**
 * @brief Add a new callback, which is not already listening.
 *
 * @param callback The new callback to add
 * @todo Fail when callback is already present (see find_callback())
 */
void DataChangeSource::add_callback(DataChangeCallback callback) {
  callbacks.push_back(callback);
}

/**
 * @brief Remove a callback that is currently listening to this object.
 *
 * @param callback The callback to remove
 * @todo Fail when callback is not present (see find_callback())
 */
void DataChangeSource::remove_callback(DataChangeCallback callback) {
  LOGERR("DataChangeSource", "This method is not implemented!");
  exit(1);
}

/**
 * @brief Call the registered callbacks.
 */
void DataChangeSource::send_data_changed() {
  for (auto&& callback : callbacks) {
      callback(this);
  }
}

/**
 * @brief This method is currently not implemented, as I have no clue how to do this
 * properly. It should return the callback that is equal to the parameter. Unfortunately,
 * the equality operator for std::function::value() does not work for capturing lambdas,
 * as value() simply returns a nullptr.
 * @todo @deprecated Someone fix this please... :(
 *
 * @param callback The callback we are looking for.
 * @return std::vector<DataChangeCallback>::iterator --- Either ::end(), or the found
 * member.
 */
std::vector<DataChangeCallback>::iterator DataChangeSource::find_callback(
    const DataChangeCallback& callback) {
  LOGERR("DataChangeSource", "This method is not implemented!");
  exit(1);
  return callbacks.end();
}
