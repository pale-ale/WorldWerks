#pragma once

#include <netinet/in.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Proto/Protocol.hpp"

/**
 * @brief Save some information about a player, i.e. his connection and username.
 *
 */
struct PlayerConnection {
  std::string playerName = "Unnamed";
  int clientFd = -1;
};

/**
 * @brief The server side of the connection.
 *
 */
class ServerEndpoint {
 private:
  typedef std::function<void(int, std::string)> EventFunc;

 public:
  std::map<wwnet::EMessageType, std::vector<EventFunc>> callbacks;
  std::vector<PlayerConnection> connections;
  struct sockaddr_in address;
  ServerEndpoint(const char* ipv4, int port);
  PlayerConnection accept_connection();
  void send_single(int clientFd, wwnet::EMessageType msgType, const char* data);
  void send_all(wwnet::EMessageType msgType, const char* data);
  void digest_incoming();
  std::pair<wwnet::EMessageType, std::string> rcv_single(int clientFd);

 private:
  char buffer[1024] = {0};
  int serverFd;
};
