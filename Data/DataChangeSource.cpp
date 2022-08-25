#include "DataChangeSource.hpp"

/**
 * @brief Add a new callback, which is not already listening.
 *
 * @param callback --- The new callback to add.
 * @todo Fix find_callback()
 */
void DataChangeSource::add_callback(DataChangeCallback callback) {
  // if (find_callback(callback) != callbacks.end()) {
  //   printf("[IDataChangeSource]: Callback already present. Cannot add.\n");
  //   return;
  // }
  callbacks.push_back(callback);
}

/**
 * @brief Remove a callback that is currently listening to this object.
 *
 * @param callback --- The callback to remove.
 * @todo Fix find_callback()
 */
void DataChangeSource::remove_callback(DataChangeCallback callback) {
  printf("[DataChangeSource]: This method is not implemented!\n");
  exit(1);
  // auto test_l = find_callback(callback);
  // if (test_l == callbacks.end()) {
  //   printf("[IDataChangeSource]: Callback does not exist. Cannot remove.\n");
  //   return;
  // }
  // callbacks.erase(test_l);
}

/**
 * @brief Call the registered callbacks.
 */
void DataChangeSource::send_data_changed() {
  for (auto& callback : callbacks) {
    callback(this);
  }
}

/**
 * @brief This method is currently not implemented, as I have no clue how to do this
 * properly. It should return the callback that is equal to the parameter. Unfortunately,
 * the equality operator for std::function::value() does not work for capturing lambdas,
 * as value() simply returns nullptr.
 *
 * @param callback The callback we are searching for.
 * @return std::vector<DataChangeCallback>::iterator --- Either ::end(), or the found
 * member.
 */
std::vector<DataChangeCallback>::iterator DataChangeSource::find_callback(
    const DataChangeCallback& callback) {
  printf("[DataChangeSource]: This method is not implemented!\n");
  exit(1);
  return callbacks.end();
}
