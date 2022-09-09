#pragma once

#include <wordexp.h>

#include <filesystem>

namespace fs = std::filesystem;

extern fs::path get_home_dir();
extern std::string get_timestamp();
extern bool zfill(int fillLen, std::string& inStr);