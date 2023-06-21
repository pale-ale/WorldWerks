#pragma once

#include <climits>
#include <functional>
#include <tuple>
#include <vector>

class DataChangeSource;

typedef std::function<void(DataChangeSource*)> DataChangeCallback;

/**
 * @brief Inherit to allow easy callback triggers on value change.
 */
class DataChangeSource {
 public:
  void add_callback(DataChangeCallback callback);
  void remove_callback(DataChangeCallback callback);
  void send_data_changed();

 protected:
  std::vector<DataChangeCallback>::iterator find_callback(
      const DataChangeCallback& callback);

  /** @brief The callbacks currently registered and their groups. */
  std::vector<DataChangeCallback> callbacks = {};
};