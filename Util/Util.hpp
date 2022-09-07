#pragma once

#include <filesystem>
#include <wordexp.h>


namespace fs = std::filesystem;

/**
 * @brief Get the home directory of the user running this process.
 * 
 * @return fs::path --- The user's home directory, i.e. "/home/<username>/"
 */
fs::path get_home_dir(){
  wordexp_t expResult;
  const char* result;
  wordexp("~", &expResult, 0);
  fs::path path = expResult.we_wordv[0];
  wordfree(&expResult);
  return path;
}


std::string get_timetamp(){
  time_t t = time(0);
  struct tm* now = localtime(&t);
  char buffer[80];
  strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", now);
  return buffer;
}
