#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

#include "MapParser/MapParser.hpp"
#include "Net/ServerEndpoint.hpp"
#include "Util/Util.hpp"
#include "Storage/LiveStorage.hpp"

ServerEndpoint serverEp("127.0.0.1", 12345);
fs::path mapPath = get_home_dir().string() + std::string("/WorldWerksMaps/");
tmx::MapParser mp;

void read_file_to_storage(const char* file, std::string storageKey){
  std::ifstream t(file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  *LiveStorage::create_entry(storageKey) = buffer.str();
}

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
      read_file_to_storage("/home/alba/WorldWerksMap/Background.tsx", "Background.tsx");
      read_file_to_storage("/home/alba/WorldWerksMap/Tiles.tsx", "Tiles.tsx");
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

  printf("[Server]: Loaded map.\n");
  serverEp.callbacks[wwnet::EMessageType::REQ_MAP].push_back([](int clientFd,
                                                                const std::string &_) {
    serverEp.send_single(clientFd, wwnet::RES_MAP, mp.get_data().c_str());
  });
  serverEp.callbacks[wwnet::EMessageType::REQ_TSX].push_back([](int clientFd,
                                                                const std::string &tsxPath) {
    auto tsx = mp.get_tileset(tsxPath);
    printf("test: %s\n", tsx->data.c_str());
    serverEp.send_single(clientFd, wwnet::RES_TSX, tsx->data.c_str());
  });

  for (int i = 0; i < 20; i++) {
    auto conn = serverEp.accept_connection();
    if (conn.clientFd != -1) {
      serverEp.connections.push_back(conn);
    }
    serverEp.digest_incoming();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}
