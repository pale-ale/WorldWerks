#pragma once

#include <functional>
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

 protected:
  void send_data_changed();
  std::vector<DataChangeCallback>::iterator find_callback(
      const DataChangeCallback& callback);

  /** @brief The callbacks currently registered. */
  std::vector<DataChangeCallback> callbacks;
};