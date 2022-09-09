#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

#include "MapParser/MapParser.hpp"
#include "Net/ServerEndpoint.hpp"
#include "Util/Util.hpp"

ServerEndpoint serverEp("127.0.0.1", 12345);
fs::path mapPath = get_home_dir().string() + std::string("/WorldWerksMaps/");
tmx::MapParser mp;

int main(int argc, char* argv[]) {
  std::error_code err;
  switch (argc) {
    case 1: {
      fs::create_directory(
          mapPath,
          err);  // If the directory is already present, err.value() will still be 0.
      if (err.value() != 0) {
        printf("[Server]: Error creating directory '%s': %s\n", mapPath.c_str(),
               err.message().c_str());
        exit(1);
      }

      mapPath += get_timestamp() + ".tmx";
      printf("[Server]: Created empty map at '%s'.\n", mapPath.c_str());
      std::ofstream{mapPath.c_str()};
      mp.load_file(mapPath);
      // TODO: Create the file & open it etc.
      break;
    }
    case 2: {
      mapPath = fs::canonical(argv[1], err);
      if (err.value() != 0) {
        printf("[Server]: Cannot load map at '%s': %s.\n", argv[1],
               err.message().c_str());
        exit(1);
      }
      if (!fs::is_regular_file(mapPath)) {
        printf("[Server]: Cannot load map at '%s': Not a regular file.\n",
               mapPath.c_str());
        exit(1);
      }
      printf("[Server]: Loading map at '%s'.\n", mapPath.c_str());
      mp.load_file(mapPath);
      // TODO: Open the file
      break;
    }

    default: {
      printf(
          "[Server]: I only need a single map path as argument, or no argument at all to "
          "create a new one.\n");
      break;
    }
  }

  for (int i = 0; i < 10; i++) {
    serverEp.accept_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
