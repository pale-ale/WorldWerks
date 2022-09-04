#include <chrono>
#include <filesystem>
#include <string>
#include <thread>
#include "MapParser/MapParser.hpp"
#include <fstream>
#include <wordexp.h>

#include "Net/ServerEndpoint.hpp"

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

ServerEndpoint serverEp("127.0.0.1", 12345);
fs::path mapPath = get_home_dir().string() + std::string("/WorldWerksMaps/");
tmx::MapParser mp; 

int main(int argc, char* argv[]) {
  switch (argc) {
    case 1: {
      time_t t = time(0);
      struct tm* now = localtime(&t);
      char buffer[80];
      strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S.tmx", now);
      std::error_code err;
      fs::create_directory(mapPath, err); // If the directory is already present, err.value() will still be 0.
      if (err.value() != 0){
        printf("[Server]: Error creating directory '%s': %s\n", mapPath.c_str(), err.message().c_str());
        exit(1);
      }
      mapPath += std::string(buffer);
      printf("[Server]: Created empty map at '%s'.\n", mapPath.c_str());
      std::ofstream {mapPath.c_str()};
      mp.load_file(mapPath);
      // TODO: Create the file & open it etc.
      break;
    }
    case 2: {
      std::error_code ec;
      mapPath = fs::canonical(argv[1], ec);
      if (ec.value() != 0) {
        printf("[Server]: Cannot load map at '%s': %s.\n", argv[1], ec.message().c_str());
        exit(1);
      }
      if (!fs::is_regular_file(mapPath)){
        printf("[Server]: Cannot load map at '%s': Not a regular file.\n", mapPath.c_str());
        exit(1);
      }
      printf("[Server]: Loading map at '%s'.\n", mapPath.c_str());
      mp.load_file(mapPath);
      // TODO: Open the file
      break;
    }

    default: {
      printf("[Server]: I only need a single map path as argument, or no argument at all to create a new one.\n");
      break;
    }
  }

  serverEp.accept_connection();
}
