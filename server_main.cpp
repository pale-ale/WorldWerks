#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

#include "MapParser/MapParser.hpp"
#include "Net/ServerEndpoint.hpp"
#include "Storage/LiveStorage.hpp"
#include "Util/Util.hpp"

ServerEndpoint serverEp("127.0.0.1", 12345);
fs::path mapPath = get_home_dir().string() + std::string("/WorldWerksMaps/");
tmx::MapParser mp;

int main(int argc, char* argv[]) {
  std::error_code err;
  switch (argc) {
    case 1: {
      fs::create_directory(mapPath, err);
      // If the directory is already present, err.value() will still be 0.
      if (err.value() != 0) {
        LOGERR("Server", fmt::format("Error creating directory '{}': {}.",
                                     mapPath.c_str(), err.message().c_str()));
        exit(1);
      }

      mapPath += get_timestamp() + ".tmx";
      LOGINF("Server", fmt::format("Created empty map at '{}'.", mapPath.c_str()));
      std::ofstream{mapPath.c_str()};
      mp.load_file(mapPath);
      // TODO: Create the file & open it etc.
      break;
    }
    case 2: {
      mapPath = fs::canonical(argv[1], err);
      if (err.value() != 0) {
        LOGERR("Server", fmt::format("Cannot load map at '{}': {}.", argv[1],
                                     err.message().c_str()));
        exit(1);
      }
      if (!fs::is_regular_file(mapPath)) {
        LOGERR("Server", fmt::format("Cannot load map at '{}': Not a regular file.",
                                     mapPath.c_str()));
        exit(1);
      }
      LOGINF("Server", fmt::format("Loading map at '{}'.", mapPath.c_str()));
      LiveStorage::useLocalFiles = true;
      LiveStorage::missingResourceHandler = [](auto srcName){
        LiveStorage::read_file_to_storage((mapPath.remove_filename() / srcName).c_str(), srcName);
        return EStorageElementState::LOCAL_READY;
      };
      mp.load_file(mapPath);
      break;
    }

    default: {
      LOGERR("Server",
             "I only need a single map path as argument, or no argument at all to create "
             "a new one.");
      break;
    }
  }

  LOGINF("Server", "Loaded map.");
  serverEp.callbacks[wwnet::EMessageType::REQ_MAP].push_back(
      [](int clientFd, const std::string& _) {
        serverEp.send_single(clientFd, wwnet::RES_MAP, mp.get_data().c_str());
      });
  serverEp.callbacks[wwnet::EMessageType::REQ_RES].push_back(
      [](int clientfd, const std::string& key) {
        std::string data;
        if (!LiveStorage::retrieve(key, data)) {
          LOGWRN("Server", "Client requested an unknown resource.");
          return;
        }
        LiveDataCapsule cap{key, data};
        serverEp.send_single(clientfd, wwnet::RES_RES, cap.to_msg().c_str());
      });

  for (int i = 0; i < 20; i++) {
    auto conn = serverEp.accept_connection();
    if (conn.clientFd != -1) {
      serverEp.connections.push_back(conn);
    }
    serverEp.digest_incoming();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  serverEp.send_all(wwnet::EMessageType::KICKED, "Server shutting down...");
}
