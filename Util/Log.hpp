#pragma once
#include <fmt/core.h>
#include <fmt/color.h>

#include <iostream>
#include <string>
#include <map>
#include <fmt/core.h>
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
  static inline std::map<ELogLevel, std::pair<const char*, const char*>> colors {
    {ELogLevel::VDEBUG,   {"\e[1;0m", "\e[0m"}},
    {ELogLevel::DEBUG,    {"\e[1;0m", "\e[0m"}},
    {ELogLevel::INFO,     {"\e[1;94m", "\e[0m"}},
    {ELogLevel::WARNING,  {"\e[1;33m", "\e[0m"}},
    {ELogLevel::ERROR,    {"\e[1;31m", "\e[0m"}}
  };

  static inline ELogLevel logLevel = ELogLevel::DEBUG;

  static void log_msg(const char* file, int line, const char* regionHint,
                      ELogLevel msgLevel, const std::string& msg) {
    if (msgLevel < logLevel) {
      return;
    }
    auto s = fmt::format("{:>11}", regionHint);
    auto&& [left, right] = colors[msgLevel];
    if (logLevel == ELogLevel::VDEBUG) {
      auto ts = get_timestamp();
      std::cout << "(" << ts << ", " << file << ":" << line << ")";
    }
    std::cout << "[" << left << s << right << "] " << msg << std::endl;
  }
};
