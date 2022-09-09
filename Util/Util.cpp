#include "Util.hpp"

/**
 * @brief Get the home directory of the user running this process.
 *
 * @return fs::path --- The user's home directory, i.e. "/home/<username>/"
 */
fs::path get_home_dir() {
  wordexp_t expResult;
  const char* result;
  wordexp("~", &expResult, 0);
  fs::path path = expResult.we_wordv[0];
  wordfree(&expResult);
  return path;
}

/**
 * @brief Get a time stamp with format %Y-%m-%d_%H-%M-%S.
 *
 * @return std::string --- The timestamp
 */
std::string get_timestamp() {
  time_t t = time(0);
  struct tm* now = localtime(&t);
  char buffer[80];
  strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", now);
  return buffer;
}

/**
 * @brief Fill a string with zeros from the left to a certain length.
 * 
 * @param fillLen The desired padded length of the string.
 * @param inStr The string to be edited.
 * @return true: String is now of desired length.
 * @return false: String is too long.
 */
bool zfill(int fillLen, std::string& inStr) {
  auto length = inStr.length();
  if (length > fillLen) {
    return false;
  }
  inStr.insert(inStr.begin(), fillLen - length, '0');
  return true;
}