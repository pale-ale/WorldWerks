#pragma once
#include <fmt/core.h>

#include <iostream>
#include <string>

#include "Util.hpp"

#define LOG(REGION, LVL, MSG) Log::log_msg(__FILE__, __LINE__, REGION, LVL, MSG)
#define LOGDBG(REGION, MSG) \
  Log::log_msg(__FILE__, __LINE__, REGION, ELogLevel::DEBUG, MSG)
#define LOGINF(REGION, MSG) Log::log_msg(__FILE__, __LINE__, REGION, ELogLevel::INFO, MSG)
#define LOGWRN(REGION, MSG) \
  Log::log_msg(__FILE__, __LINE__, REGION, ELogLevel::WARNING, MSG)
#define LOGERR(REGION, MSG) \
  Log::log_msg(__FILE__, __LINE__, REGION, ELogLevel::ERROR, MSG)

enum class ELogLevel : int8_t { VDEBUG, DEBUG, INFO, WARNING, ERROR };

class Log {
 public:
  static inline ELogLevel logLevel = ELogLevel::DEBUG;

  static void log_msg(const char* file, int line, const char* regionHint,
                      ELogLevel msgLevel, const std::string& msg) {
    if (msgLevel < logLevel) {
      return;
    }
    if (logLevel == ELogLevel::VDEBUG) {
      auto ts = get_timestamp();
      std::cout << "(" << ts << ", " << file << ":" << line << ") [" << regionHint
                << "]: " << msg << std::endl;
      return;
    }
    std::cout << "[" << regionHint << "]: " << msg << std::endl;
  }
};
